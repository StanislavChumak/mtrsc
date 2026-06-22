#include "scn/Entity.hpp"

#include "util/from_json.hpp"
#include "util/hash.hpp"
#include "util/LogSystem.hpp"

#include <fstream>

bool Entity::from_json(
    simdjson::ondemand::object &obj,
    std::string scene_name,
    std::vector<DynamicBuffer> &dynamic_buffers)
{
    for(auto field : obj)
    {
        std::string key = std::string(get_result_json<std::string_view>(field.unescaped_key()));
        if(key == "Name")
        {
            set_in_var_json<std::string_view>(_name, field.value());
            continue;
        }
        auto obj = get_var_json<simdjson::ondemand::object>(field.value());
        Component component{};
        if(!component.from_json(obj, key, dynamic_buffers))
        {
            continue;
        }
        _size += component.size();
        _components.push_back(std::move(component));
        auto hash = hash_string(key);
    }

    if(_name == "")
    {
        LogSystem::print_err("The entity has no \"Name\"");
        return false;
    }
    if(_size == sizeof(_id) * 2)
    {
        LogSystem::print_err("The entity is empty");
        return false;
    }
    _id = hash_string(scene_name);
    _id = hash_string(_name, _id);
    return true;
}

bool Entity::to_file_mtscn(std::ofstream &file)
{
    uint64_t offset_to_next_entity = static_cast<uint64_t>(file.tellp()) + _size;

    FILE_WRITE(file, _id);
    LogSystem::print_parameter("  "+_name, std::to_string(_id), sizeof(_id), file.tellp());

    LogSystem::print_variable("\tsize", std::to_string(_size));

    LOG_WRITE(file, offset_to_next_entity, "\toffset_to_next_entity");

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