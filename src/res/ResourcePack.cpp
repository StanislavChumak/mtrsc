#include "res/ResourcePack.hpp"

#include "util/fun/prs/json.hpp"
#include "util/fun/math/hash.hpp"
#include "util/fun/msg/mtrs_file_message.hpp"

#include <fstream>

namespace mtrs::res
{

ResourcePack::ResourcePack(simdjson::ondemand::object &pack_json, uint64_t cache_lifetime, std::string name)
: _cache_lifetime(cache_lifetime)
{
    for(auto group_json : pack_json)
    {
        auto type = prs::get_result_json<std::string_view>(group_json.unescaped_key());
        auto array = prs::get_value_json<simdjson::ondemand::array>(group_json.value());
        ResourceGroup group{array, (std::string)type, _deferred_data};
        if(!group.is_init())
        {
            msg::mtrs_error("In ResourcePack \"", name, '\"');
            continue;
        }
        _size += group.size();
        _groups.push_back(std::move(group));
    }

    if(_groups.empty())
    {
        msg::mtrs_error("The ResourcePack \"", name, "\" is empty =!");
        _is_init = false;
    }

    _deferred_data_size = _size;
    for(auto &ddata : _deferred_data)
    {
        ddata.field[0] = _deferred_data_size;
        _deferred_data_size += ddata.field[1];
    }
    _deferred_data_size -= _size;
}

bool ResourcePack::to_file_mtrs(std::ofstream &file)
{
    if(!file) return false;

    uint64_t file_size = _size + _deferred_data_size;
    msg::variable_message(0, "file_size", file_size);

    // Header
    file.write(_magic, sizeof(_magic));
    msg::parameter_message(0, "header", _magic, 8, file.tellp());

    // Resources
    msg::variable_message(0, "resource_groups", _groups.size());

    for(auto &group : _groups)
    {
        group.to_file_mtscn(file, 2);
    }

    // Deferred Data Block
    msg::variable_message(0, "deferred_data_block", _deferred_data_size);

    for(auto &ddata : _deferred_data)
    {
        file.write(ddata.data, ddata.field[1]);
        msg::parameter_message(2, "data", ddata.field[1], ddata.field[1], file.tellp());
    }

    msg::verification_message("end_file", file_size, (uint64_t)file.tellp());

    return true;
}

ResourcePack::ResourcePack(ResourcePack &&other) noexcept
{
    _is_init = other._is_init;
    _is_init = false;
    _cache_lifetime = other._cache_lifetime;
    other._cache_lifetime = 0;
    _size = other._size;
    other._size = 0;
    _deferred_data_size = other._deferred_data_size;
    other._deferred_data_size = 0;
    _groups = std::move(other._groups);
    _deferred_data = std::move(other._deferred_data);
}

ResourcePack &ResourcePack::operator=(ResourcePack &&other) noexcept
{
    if(this != &other)
    {
        _is_init = other._is_init;
        _is_init = false;
        _cache_lifetime = other._cache_lifetime;
        other._cache_lifetime = 0;
        _size = other._size;
        other._size = 0;
        _deferred_data_size = other._deferred_data_size;
        other._deferred_data_size = 0;
        _groups = std::move(other._groups);
        _deferred_data = std::move(other._deferred_data);
    }
    return *this;
}

}