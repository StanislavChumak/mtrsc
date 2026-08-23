#include "scn/Entity.hpp"

#include "util/fun/prs/json.hpp"
#include "util/fun/math/hash.hpp"
#include "util/fun/msg/mtrs_file_message.hpp"

#include <fstream>

namespace mtrs::comp
{

Entity::Entity(simdjson::ondemand::object &obj, std::string scene_name,
    std::vector<prs::DeferredData> &deferred_data)
{
    for(auto field : obj)
    {
        std::string key = std::string(prs::get_result_json<std::string_view>(field.unescaped_key()));
        if(key == "Name")
        {
            _name = prs::get_value_json<std::string_view>(field.value());
            continue;
        }
        auto obj = prs::get_value_json<simdjson::ondemand::object>(field.value());
        Component component{obj, key, deferred_data};
        if(!component.is_init())
        {
            continue;
        }
        _size += component.size();
        _components.push_back(std::move(component));
        auto hash = math::hash64(key);
    }

    if(_name == "")
    {
        msg::mtrs_error("The entity has no \"Name\"");
        _is_init = false;
    }

    if(_size == sizeof(uint64_t) * 2)
    {
        msg::mtrs_error("The entity is empty");
        _is_init = false;
    }
}

bool Entity::to_file_mtscn(std::ofstream &file, size_t msg_offset)
{
    uint64_t id = math::hash64(_name);
    uint64_t next_entity = static_cast<uint64_t>(file.tellp()) + _size;

    file.write(reinterpret_cast<char*>(&id), sizeof(id));
    msg::parameter_message(msg_offset, _name, id, sizeof(id), file.tellp());
    msg_offset += 2;

    msg::variable_message(msg_offset, "size", _size);

    file.write(reinterpret_cast<char*>(&next_entity), sizeof(next_entity));
    msg::parameter_message(msg_offset, "next_entity", next_entity, sizeof(next_entity), file.tellp());

    for(auto &comp : _components)
    {
        comp.to_file_mtscn(file, msg_offset + 2);
    }

    return true;
}

Entity::Entity(Entity &&other) noexcept
{
    _name = std::move(other._name);
    _is_init = other._is_init;
    other._is_init = false;
    _size = other._size;
    other._size = 0;
    _components = std::move(other._components);
}

Entity &Entity::operator=(Entity &&other) noexcept
{
    if(this != &other)
    {
        _name = std::move(other._name);
        _is_init = other._is_init;
        other._is_init = false;
        _size = other._size;
        other._size = 0;
        _components = std::move(other._components);
    }
    return *this;
}

}