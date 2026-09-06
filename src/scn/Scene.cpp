#include "scn/Scene.hpp"

#include "util/fun/prs/json.hpp"
#include "util/fun/msg/mtrs_file_message.hpp"

#include <fstream>

namespace mtrs::comp
{

Scene::Scene(simdjson::ondemand::array &json_scene, uint64_t cache_lifetime, std::string name)
: _cache_lifetime(cache_lifetime)
{
    for(simdjson::ondemand::object obj : json_scene)
    {
        Entity entity{obj, name, _deferred_data};
        if(!entity.is_init()) continue;

        _entities_size += entity.size();
        _entities.push_back(std::move(entity));
    }

    if(_entities.empty())
    {
        msg::mtrs_error("The Scene is empty. It is not compiled");
        _is_init = false;
    }

    _entity_count = _entities.size();
    _ddata_offset = _entity_offset + _entities_size;

    _deferred_data_size = _ddata_offset;
    for(auto &ddata : _deferred_data)
    {
        ddata.field[0] = _deferred_data_size;
        _deferred_data_size += ddata.field[1];
    }
    _deferred_data_size -= _ddata_offset;
}

bool Scene::to_file_mtsc(std::ofstream &file)
{
    if(!file) return false;

    uint64_t file_size = HEADER_SCENE_SIZE + _entities_size + _deferred_data_size;
    msg::variable_message(0, "file_size", file_size);

    // Header
    file.write(_magic, sizeof(_magic));
    msg::parameter_message(0, "header", _magic, 8, file.tellp());
    file.write(reinterpret_cast<char*>(&_cache_lifetime), sizeof(_cache_lifetime));
    msg::parameter_message(0, "cache_lifetime", _cache_lifetime, sizeof(_cache_lifetime), file.tellp());

    file.write(reinterpret_cast<char*>(&_entity_count), sizeof(_entity_count));
    msg::parameter_message(0, "entity_count", _entity_count, sizeof(_entity_count), file.tellp());
    file.write(reinterpret_cast<char*>(&_entity_offset), sizeof(_entity_offset));
    msg::parameter_message(0, "entity_offset", _entity_offset, sizeof(_entity_offset), file.tellp());
    file.write(reinterpret_cast<char*>(&_ddata_offset), sizeof(_ddata_offset));
    msg::parameter_message(0, "deferred_data_offset", _ddata_offset, sizeof(_ddata_offset), file.tellp());

    // Entity
    msg::verification_message("entity_offset", _entity_offset, (uint32_t)file.tellp());
    msg::variable_message(0, "entities", _entities.size());

    for(Entity &entity : _entities)
    {
        entity.to_file_mtscn(file, 2);
    }

    // Deferred Block
    msg::verification_message("deferred_data_offset", _ddata_offset, (uint32_t)file.tellp());
    msg::variable_message(0, "deferred_data_block", _deferred_data_size);

    for(auto &ddata : _deferred_data)
    {
        file.write(ddata.data, ddata.field[1]);
        msg::parameter_message(2, "data", ddata.field[1], ddata.field[1], file.tellp());
    }

    msg::verification_message("end_file", file_size, (uint64_t)file.tellp());

    return true;
}

Scene::Scene(Scene &&other) noexcept
{
    _entity_count = other._entity_count;
    other._entity_count = 0;

    _cache_lifetime = other._cache_lifetime;
    other._cache_lifetime = 0;
    _ddata_offset = other._ddata_offset;
    other._ddata_offset = 0;

    _entities = std::move(other._entities);
    _deferred_data = std::move(other._deferred_data);
}

Scene &Scene::operator=(Scene &&other) noexcept
{
    if(this != &other)
    {
        _entity_count = other._entity_count;
        other._entity_count = 0;

        _cache_lifetime = other._cache_lifetime;
        other._cache_lifetime = 0;
        _ddata_offset = other._ddata_offset;
        other._ddata_offset = 0;

        _entities = std::move(other._entities);
        _deferred_data = std::move(other._deferred_data);
    }
    return *this;
}

}