#include "scn/Scene.hpp"

#include "util/from_json.hpp"
#include "util/LogSystem.hpp"

#include <fstream>

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
        LogSystem::print_err("The Scene is empty. It is not compiled");
        return false;
    }

    _entity_count = _entities.size();
    _free_for_dynamic_date_offset = _entity_offset + _entities_size;
    _dynamic_date_offset = _free_for_dynamic_date_offset + FREE_FOR_DYNAMIC_DATE_SIZE;

    _dynamic_date_size = _dynamic_date_offset;
    for(auto dynamic : _dynamic_buffers)
    {
        *(dynamic.p_offset) = _dynamic_date_size;
        _dynamic_date_size += dynamic.size;
    }
    _dynamic_date_size -= _dynamic_date_offset;
    
    return true;
}

bool Scene::to_file_mtsc(std::ofstream &file)
{
    if(!file) return false;

    uint64_t file_size = HEADER_SCENE_SIZE + _entities_size + FREE_FOR_DYNAMIC_DATE_SIZE + _dynamic_date_size;
    LogSystem::print_variable("file_size", std::to_string(file_size));

    // Header
    file.write(_magic, sizeof(_magic));
    FILE_WRITE(file, _version);
    LogSystem::print_header(_magic, _version);

    LOG_WRITE(file, _entity_count, "entity_cout");
    LOG_WRITE(file, _entity_offset, "entity_offset");
    LOG_WRITE(file, _free_for_dynamic_date_offset, "free_for_dynamic_date_offset");
    LOG_WRITE(file, _dynamic_date_offset, "dynamic_date_offset");

    // Entity
    LogSystem::check_offset("entity_offset", _entity_offset, file.tellp());
    LogSystem::print_variable("entities", std::to_string(_entities.size()));

    for(Entity &entity : _entities)
    {
        entity.to_file_mtscn(file);
    }

    // Free For Dynamic date
    LogSystem::check_offset("free_for_dynamic_date_offset", _free_for_dynamic_date_offset, file.tellp());
    char zero[FREE_FOR_DYNAMIC_DATE_SIZE];
    std::memset(zero, 0, FREE_FOR_DYNAMIC_DATE_SIZE);
    FILE_WRITE(file, zero);
    LogSystem::print_parameter("free_dynamic_date", std::to_string(sizeof(zero)), sizeof(zero), file.tellp());

    // Dynamic Block
    LogSystem::check_offset("dynamic_date_offset", _dynamic_date_offset, file.tellp());
    LogSystem::print_variable("dynamic_data_block", std::to_string(_dynamic_date_size));
    for(auto dynamic : _dynamic_buffers)
    {
        file.write(dynamic.date, dynamic.size);
        LogSystem::print_parameter("  dynamic_date", "---", dynamic.size, file.tellp());
    }

    LogSystem::check_offset("end_file", file_size, file.tellp());

    return true;
}

Scene::Scene(float version)
: _version(version)
{}

Scene::Scene(Scene &&other) noexcept
{
    _entity_count = other._entity_count;
    other._entity_count = 0;
    _version = other._version;
    other._version = 0.f;

    _free_for_dynamic_date_offset = other._free_for_dynamic_date_offset;
    other._free_for_dynamic_date_offset = 0;
    _dynamic_date_offset = other._dynamic_date_offset;
    other._dynamic_date_offset = 0;

    _entities = std::move(other._entities);
    _dynamic_buffers = std::move(other._dynamic_buffers);
}

Scene &Scene::operator=(Scene &&other) noexcept
{
    if(this != &other)
    {
        _entity_count = other._entity_count;
        other._entity_count = 0;
        _version = other._version;
        other._version = 0.f;

        _free_for_dynamic_date_offset = other._free_for_dynamic_date_offset;
        other._free_for_dynamic_date_offset = 0;
        _dynamic_date_offset = other._dynamic_date_offset;
        other._dynamic_date_offset = 0;

        _entities = std::move(other._entities);
        _dynamic_buffers = std::move(other._dynamic_buffers);
    }
    return *this;
}