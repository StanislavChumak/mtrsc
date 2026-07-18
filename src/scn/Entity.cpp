#include "scn/Entity.hpp"

#include "util/from_json.hpp"
#include "util/hash.hpp"
#include "util/mtrsc_message.hpp"

#include <fstream>

namespace mtrs::comp
{

bool Entity::from_json(
    simdjson::ondemand::object &obj,
    std::string scene_name,
    std::vector<util::DynamicBuffer> &dynamic_buffers)
{
    for(auto field : obj)
    {
        std::string key = std::string(util::get_result_json<std::string_view>(field.unescaped_key()));
        if(key == "Name")
        {
            util::set_in_var_json<std::string_view>(_name, field.value());
            continue;
        }
        auto obj = util::get_var_json<simdjson::ondemand::object>(field.value());
        Component component{};
        if(!component.from_json(obj, key, dynamic_buffers))
        {
            continue;
        }
        _size += component.size();
        _components.push_back(std::move(component));
        auto hash = util::hash_string<uint64_t>(key);
    }

    if(_name == "")
    {
        util::mtrsc_error("The entity has no \"Name\"");
        return false;
    }
    if(_size == sizeof(_id) * 2)
    {
        util::mtrsc_error("The entity is empty");
        return false;
    }

    _id = util::hash_string<uint64_t>(_name);
    
    return true;
}

bool Entity::to_file_mtscn(std::ofstream &file)
{
    uint64_t offset_to_next_entity = static_cast<uint64_t>(file.tellp()) + _size;

    file.write(reinterpret_cast<char*>(&_id), sizeof(_id));
    util::parameter_message(2, _name, _id, sizeof(_id), file.tellp());

    util::variable_message(4, "size", _size);

    LOG_WRITE(file, 4, offset_to_next_entity, "offset_to_next_entity");

    for(auto &comp : _components)
    {
        comp.to_file_mtscn(file);
    }

    return true;
}

Entity::Entity(Entity &&other) noexcept
{
    _id = other._id;
    other._id = 0;
    _size = other._size;
    other._size = 0;
    _components = std::move(other._components);
    _name = std::move(other._name);
}

Entity &Entity::operator=(Entity &&other) noexcept
{
    if(this != &other)
    {
        _id = other._id;
        other._id = 0;
        _size = other._size;
        other._size = 0;
        _components = std::move(other._components);
        _name = std::move(other._name);
    }
    return *this;
}

}