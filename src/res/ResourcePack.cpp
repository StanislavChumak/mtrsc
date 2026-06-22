#include "res/ResourcePack.hpp"

#include "util/from_json.hpp"
#include "util/hash.hpp"
#include "util/LogSystem.hpp"

#include <fstream>

bool ResourcePack::from_json(simdjson::ondemand::object &pack_json, std::string name)
{
    for(auto res : pack_json)
    {
        std::string key = std::string(get_result_json<std::string_view>(res.unescaped_key()));
        auto array = get_var_json<simdjson::ondemand::array>(res.value());
        ResourceType type;
        if(!type.from_json(array, key, _dynamic_buffers))
        {
            continue;
        }
        _size += type.size();
        _resource_types.push_back(std::move(type));
    }

    if(_resource_types.empty())
    {
        LogSystem::print_err(LogSystem::args_to_str("The ResourcePack \"", name, "\" is empty =!"));
        return false;
    }

    _dynamic_date_size = _size;
    for(auto dynamic : _dynamic_buffers)
    {
        *dynamic.p_offset = _dynamic_date_size;
        _dynamic_date_size += dynamic.size;
    }
    _dynamic_date_size -= _size;

    return true;
}

bool ResourcePack::to_file_mtrs(std::ofstream &file)
{
    if(!file) return false;

    LogSystem::print_variable("file_size", std::to_string(_size + _dynamic_date_size));

    // Header
    file.write(_magic, sizeof(_magic));
    FILE_WRITE(file, _version);
    LogSystem::print_header(_magic, _version);

    // Resources
    LogSystem::print_variable("resource_types", std::to_string(_resource_types.size()));

    for(auto &res_type : _resource_types)
    {
        res_type.to_file_mtscn(file);
    }

    // Dynamic Data Block
    LogSystem::print_variable("dynamic_data_block", std::to_string(_dynamic_date_size));

    for(auto dynamic : _dynamic_buffers)
    {
        file.write(dynamic.date, dynamic.size);
        LogSystem::print_parameter("  data", std::to_string(dynamic.size), dynamic.size, file.tellp());
    }

    return true;
}

ResourcePack::ResourcePack(float version)
: _version(version)
{}

ResourcePack::ResourcePack(ResourcePack &&other) noexcept
{
    _version = other._version;
    other._version = 0.f;

    _resource_types = std::move(other._resource_types);
    _dynamic_buffers = std::move(other._dynamic_buffers);
}

ResourcePack &ResourcePack::operator=(ResourcePack &&other) noexcept
{
    if(this != &other)
    {
        _version = other._version;
        other._version = 0.f;
        _resource_types = std::move(other._resource_types);
        _dynamic_buffers = std::move(other._dynamic_buffers);
    }
    return *this;
}