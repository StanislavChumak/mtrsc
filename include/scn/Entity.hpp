#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Component.hpp"

class Entity
{
    uint64_t _id;
    uint32_t _size = sizeof(_id) * 2;
    std::vector<Component> _components;

    std::string _name = "";
    
public:
    Entity() = default;
    Entity(Entity &) = delete;
    Entity &operator=(const Entity &) = delete;
    Entity(Entity &&other) noexcept;
    Entity &operator=(Entity &&other) noexcept;
    ~Entity() = default;

    uint32_t size() { return _size; }

    bool from_json(
        simdjson::ondemand::object &obj,
        std::string scene_name,
        std::vector<DynamicBuffer> &dynamic_buffers);
    bool to_file_mtscn(std::ofstream &file);
};


#endif
