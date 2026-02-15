#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"

class Entity
{
    uint64_t id;
    uint32_t size = 0;
    std::vector<Component> components;
    
public:
    Entity() = default;
    Entity(Entity &) = delete;
    Entity &operator=(const Entity &) = delete;
    Entity(Entity &&other) noexcept;
    Entity &operator=(Entity &&other) noexcept;
    ~Entity() = default;

    uint32_t get_size() { return size; }

    bool from_json(
        simdjson::ondemand::object &obj,
        std::string fileName,
        std::vector<DynamicDataBuffer> &dynamicDataBuffer);
    bool to_file_mtscn(std::ofstream &file, uint32_t &dataOffset);
};


#endif
