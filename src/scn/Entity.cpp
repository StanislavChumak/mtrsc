#include "scn/Entity.h"

#include "util/jsonUtils.h"
#include "util/hash.h"

#include <fstream>
#include <iostream>

bool Entity::from_json(
    simdjson::ondemand::object &obj,
    std::string fileName,
    std::vector<DynamicDataBuffer> &dynamicDataBuffer)
{
    std::string name = "";
    for(auto field : obj)
    {
        std::string key = std::string(get_result_json<std::string_view>(field.unescaped_key()));
        auto obj = get_var_json<simdjson::ondemand::object>(field.value());
        if(key == "Name")
        {
            name = std::string(get_var_json<std::string_view>(obj["value"]));
            continue;
        }
        Component component{};
        if(!component.from_json(obj, key, dynamicDataBuffer))
            continue;
        size += component.get_size();
        components.push_back(std::move(component));
        auto hash = hash_string(key);
    }
    if(name == "")
    {
        std::cerr << "!= The entity has no \"Name\" =!" << std::endl;
        return false;
    }
    if(size == 0)
    {
        std::cerr << "!= The entity is empty =!" << std::endl;
        return false;
    }
    id = hash_string(fileName);
    id = hash_string(name, id);
    return true;
}

bool Entity::to_file_mtscn(std::ofstream &file, uint32_t &dataOffset)
{
    file.write(reinterpret_cast<char*>(&id), sizeof(id));
    file.write(reinterpret_cast<char*>(&dataOffset), sizeof(dataOffset));
    file.write(reinterpret_cast<char*>(&size), sizeof(size));

    std::streampos entityCursor = file.tellp();
    file.seekp(dataOffset, std::ios::beg);

    for(auto &comp : components)
        comp.to_file_mtscn(file);

    file.seekp(entityCursor, std::ios::beg);
    dataOffset += size;
    return true;
}

Entity::Entity(Entity &&other) noexcept
{
    id = other.id;
    other.id = 0;
    size = other.size;
    other.size = 0;
    components = std::move(other.components);
}

Entity &Entity::operator=(Entity &&other) noexcept
{
    if(this != &other)
    {
        id = other.id;
        other.id = 0;
        size = other.size;
        other.size = 0;
        components = std::move(other.components);
    }
    return *this;
}