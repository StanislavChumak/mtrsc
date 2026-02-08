#include "structure/Entity.h"

#include "utilities/jsonUtils.h"

#include "utilities/hash.h"

#include <iostream>

bool Entity::from_json(std::string patchJson, std::vector<DynamicDataBuffer> &dynamicDataBuffer, simdjson::ondemand::object obj)
{
    std::string name = "";
    for(auto field : obj)
    {
        std::string key = std::string(get_result_json<std::string_view>(field.unescaped_key()));
        if(key == "Name")
        {
            auto obj = get_var_json<simdjson::ondemand::object>(field.value());
            name = std::string(get_var_json<std::string_view>(obj["value"]));
            continue;
        }
        Component component{};
        if(component.from_json(key, dynamicDataBuffer, get_var_json<simdjson::ondemand::object>(field.value())))
        {
            size += component.size;
            components.push_back(std::move(component));
            auto hash = hash_string(key);
        }
        else
        {
            printf("Component not defined\n");
        }
    }
    if(name == "")
    {
        printf("!! The entity has no \"Name\"");
        return false;
    }
    if(size == 0)
    {
        printf("!! The entity is empty");
        return false;
    }
    size += sizeof(size) + sizeof(id);
    id = make_scene_entity_id(patchJson, name);
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

Entity::~Entity()
{
    components.clear();
}