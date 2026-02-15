#include "res/ResourcePack.h"

#include "util/jsonUtils.h"
#include "util/hash.h"

#include <fstream>

bool ResourcePack::from_json(simdjson::ondemand::object &jsonRes, std::string name)
{
    for(auto res : jsonRes)
    {
        std::string key = std::string(get_result_json<std::string_view>(res.unescaped_key()));
        auto array = get_var_json<simdjson::ondemand::array>(res.value());
        ResourceType type{};
        if(!type.from_json(array, key, dynamicDataBuffer))
            continue;
        resourceTypes.push_back(std::move(type));
    }

    if(resourceTypes.empty())
    {
        std::cerr << "!= The ResourcePack \"" << name << "\" is empty =!\n" << std::endl;
        return false;
    }

    return true;
}

bool ResourcePack::to_file_mtrs(std::ofstream &file)
{
    if(!file) return false;
    // Header
    file.write(magic, sizeof(magic));
    file.write(reinterpret_cast<char*>(&version), sizeof(version));
    file.write(reinterpret_cast<char*>(&flags), sizeof(flags));

    file.write(reinterpret_cast<char*>(&resourceDataOffset), sizeof(resourceDataOffset));
    file.write(reinterpret_cast<char*>(&dynamicDateOffset), sizeof(dynamicDateOffset));

    // Resources
    for(auto &resType : resourceTypes)
        resType.to_file_mtscn(file);

    // Dynamic Data Block
    uint32_t dynamicOffset = dynamicDateOffset;
    for(auto dynamic : dynamicDataBuffer)
    {
        file.write(dynamic.date, dynamic.sizeData);
        *dynamic.pOffset = dynamicOffset;
        dynamicOffset += dynamic.sizeData;
    }

    return true;
}

ResourcePack::ResourcePack(uint16_t version)
: version(version)
{}

ResourcePack::ResourcePack(ResourcePack &&other) noexcept
{
    version = other.version;
    flags = other.flags;
    other.flags = 0;
}

ResourcePack &ResourcePack::operator=(ResourcePack &&other) noexcept
{
    if(this != &other)
    {
        version = other.version;
        flags = other.flags;
        other.flags = 0;
    }
    return *this;
}