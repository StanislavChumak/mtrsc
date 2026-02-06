#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include <vector>

struct Entity
{
    uint64_t id;
    uint32_t size = 0;
    std::vector<Component> components;

    bool from_json(std::string patchJson, simdjson::ondemand::object obj);
    Entity() = default;
    Entity(Entity &) = delete;
    Entity &operator=(const Entity &) = delete;
    Entity(Entity &&other) noexcept;
    Entity &operator=(Entity &&other) noexcept;
    ~Entity();
};


#endif
