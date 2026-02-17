#include "res/ResourceType.h"

#include <fstream>

bool ResourceType::from_json(
    simdjson::ondemand::array &array,
    std::string &resName,
    std::vector<DynamicDataBuffer> &dynamicDataBuffer)
{
    name = resName;
    sizeName = static_cast<uint32_t>(name.size());
    for(simdjson::ondemand::object obj : array)
    {
        Resource res{};
        if(!res.from_json(obj, resName, dynamicDataBuffer))
            continue;
        sizeBlockResources += res.get_size();
        resources.push_back(std::move(res));
    }

    if(resources.empty())
    {
        std::cerr << "!= The ResourceType \"" << name << "\" is empty =!" << std::endl;
        return false;
    }
    
    return true;
}

bool ResourceType::to_file_mtscn(std::ofstream &file)
{
    file.write(reinterpret_cast<char*>(&sizeName), sizeof(sizeName));
    file.write(name.data(), sizeName);
    file.write(reinterpret_cast<char*>(&sizeBlockResources), sizeof(sizeBlockResources));

    for(auto &res : resources)
        res.to_file_mtscn(file);

    return true;
}

ResourceType::ResourceType(ResourceType &&other) noexcept
{
    name = std::move(other.name);
    resources = std::move(other.resources);
}

ResourceType &ResourceType::operator=(ResourceType &&other) noexcept
{
    if(this != &other)
    {
        name = std::move(other.name);
        resources = std::move(other.resources);
    }
    return *this;
}