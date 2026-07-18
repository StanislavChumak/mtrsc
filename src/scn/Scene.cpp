#include "scn/Scene.hpp"

#include "util/from_json.hpp"
#include "util/mtrsc_message.hpp"

#include <fstream>

namespace mtrs::comp
{

bool Scene::from_json(simdjson::ondemand::array &json_scene, std::string name)
{
    for(simdjson::ondemand::object obj : json_scene)
    {
        Entity entity{};
        if(!entity.from_json(obj, name, _dynamic_buffers))
            continue;
        _entities_size += entity.size();
        _entities.push_back(std::move(entity));
    }

    if(_entities.empty())
    {
        util::mtrsc_error("The Scene is empty. It is not compiled");
        return false;
    }

    _entity_count = _entities.size();
    _free_for_dynamic_data_offset = _entity_offset + _entities_size;
    _dynamic_data_offset = _free_for_dynamic_data_offset + FREE_FOR_DYNAMIC_DATA_SIZE;

    _dynamic_data_size = _dynamic_data_offset;
    for(auto dynamic : _dynamic_buffers)
    {
        *(dynamic.p_offset) = _dynamic_data_size;
        _dynamic_data_size += dynamic.size;
    }
    _dynamic_data_size -= _dynamic_data_offset;
    
    return true;
}

bool Scene::to_file_mtsc(std::ofstream &file)
{
    if(!file) return false;

    uint64_t file_size = HEADER_SCENE_SIZE + _entities_size + FREE_FOR_DYNAMIC_DATA_SIZE + _dynamic_data_size;
    util::variable_message(0, "file_size", file_size);

    // Header
    file.write(_magic, sizeof(_magic));
    util::parameter_message(0, "header", _magic, 8, file.tellp());

    LOG_WRITE(file, 0, _entity_count, "entity_cout");
    LOG_WRITE(file, 0, _entity_offset, "entity_offset");
    LOG_WRITE(file, 0, _free_for_dynamic_data_offset, "free_for_dynamic_data_offset");
    LOG_WRITE(file, 0, _dynamic_data_offset, "dynamic_data_offset");

    // Entity
    util::verification_message("entity_offset", _entity_offset, (uint32_t)file.tellp());
    util::variable_message(0, "entities", _entities.size());

    for(Entity &entity : _entities)
    {
        entity.to_file_mtscn(file);
    }

    // Free For Dynamic data
    util::verification_message("free_for_dynamic_data_offset",
        _free_for_dynamic_data_offset, (uint32_t)file.tellp());
    char zero[FREE_FOR_DYNAMIC_DATA_SIZE];
    std::memset(zero, 0, FREE_FOR_DYNAMIC_DATA_SIZE);
    file.write(zero, FREE_FOR_DYNAMIC_DATA_SIZE);
    util::parameter_message(0, "free_dynamic_data", sizeof(zero), sizeof(zero), file.tellp());

    // Dynamic Block
    util::verification_message("dynamic_data_offset", _dynamic_data_offset, (uint32_t)file.tellp());
    util::variable_message(0, "dynamic_data_block", _dynamic_data_size);
    for(auto dynamic : _dynamic_buffers)
    {
        file.write(dynamic.data, dynamic.size);
        util::parameter_message(2, "dynamic_data", dynamic.size, dynamic.size, file.tellp());
        delete dynamic.data;
    }

    util::verification_message("end_file", file_size, (uint64_t)file.tellp());

    return true;
}

Scene::Scene(Scene &&other) noexcept
{
    _entity_count = other._entity_count;
    other._entity_count = 0;

    _free_for_dynamic_data_offset = other._free_for_dynamic_data_offset;
    other._free_for_dynamic_data_offset = 0;
    _dynamic_data_offset = other._dynamic_data_offset;
    other._dynamic_data_offset = 0;

    _entities = std::move(other._entities);
    _dynamic_buffers = std::move(other._dynamic_buffers);
}

Scene &Scene::operator=(Scene &&other) noexcept
{
    if(this != &other)
    {
        _entity_count = other._entity_count;
        other._entity_count = 0;

        _free_for_dynamic_data_offset = other._free_for_dynamic_data_offset;
        other._free_for_dynamic_data_offset = 0;
        _dynamic_data_offset = other._dynamic_data_offset;
        other._dynamic_data_offset = 0;

        _entities = std::move(other._entities);
        _dynamic_buffers = std::move(other._dynamic_buffers);
    }
    return *this;
}

}