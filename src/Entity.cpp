#include "Entity.h"

#include "jsonUtils.h"
#include "hash.h"

#include <iostream>

bool Entity::from_json(std::string patchJson, simdjson::ondemand::object obj)
{
    std::string name = "";
    for(auto field : obj)
    {
        std::string key = std::string(getResultJSON<std::string_view>(field.unescaped_key()));
        if(key == "Name")
        {
            name = std::string(getResultJSON<std::string_view>(field.value()));
            continue;
        }
        Component component{};
        u_int64_t id = hash_string(key);
        if(component.from_json(id, getVarJSON<simdjson::ondemand::object>(field.value())))
        {
            size += component.size;
            components.push_back(std::move(component));
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