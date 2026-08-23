#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Component.hpp"

namespace mtrs::comp
{

class Entity
{
    std::string _name = "";
    bool _is_init = true;

    uint32_t _size = sizeof(uint64_t) * 2;
    std::vector<Component> _components;

public:
    Entity() = delete;
    Entity(Entity &) = delete;
    Entity &operator=(const Entity &) = delete;
    Entity(Entity &&other) noexcept;
    Entity &operator=(Entity &&other) noexcept;
    ~Entity() = default;
    
    Entity(simdjson::ondemand::object &obj, std::string scene_name,
        std::vector<prs::DeferredData> &deferred_data);

    inline uint32_t size() { return _size; }
    inline bool is_init() { return _is_init; }

    bool to_file_mtscn(std::ofstream &file, size_t msg_offset);
};

}

#endif
