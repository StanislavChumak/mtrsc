#include "res/ResourcePack.hpp"

#include "util/from_json.hpp"
#include "util/hash.hpp"
#include "util/mtrsc_message.hpp"

#include <fstream>

namespace mtrs::res
{

bool ResourcePack::from_json(simdjson::ondemand::object &pack_json, std::string name)
{
    for(auto res : pack_json)
    {
        std::string key = std::string(util::get_result_json<std::string_view>(res.unescaped_key()));
        auto array = util::get_var_json<simdjson::ondemand::array>(res.value());
        ResourceType type;
        if(!type.from_json(array, key, _dynamic_buffers))
        {
            util::mtrsc_error("In ResourcePack \"", name, '\"');
            continue;
        }
        _size += type.size();
        _resource_types.push_back(std::move(type));
    }

    if(_resource_types.empty())
    {
        util::mtrsc_error("The ResourcePack \"", name, "\" is empty =!");
        return false;
    }

    _dynamic_data_size = _size;
    for(auto dynamic : _dynamic_buffers)
    {
        *dynamic.p_offset = _dynamic_data_size;
        _dynamic_data_size += dynamic.size;
    }
    _dynamic_data_size -= _size;

    return true;
}

bool ResourcePack::to_file_mtrs(std::ofstream &file)
{
    if(!file) return false;

    uint64_t file_size = _size + _dynamic_data_size;
    util::variable_message(0, "file_size", file_size);

    // Header
    file.write(_magic, sizeof(_magic));
    util::parameter_message(0, "header", _magic, 8, file.tellp());

    // Resources
    util::variable_message(0, "resource_types", _resource_types.size());

    for(auto &res_type : _resource_types)
    {
        res_type.to_file_mtscn(file);
    }

    // Dynamic Data Block
    util::variable_message(0, "dynamic_data_block", _dynamic_data_size);

    for(auto dynamic : _dynamic_buffers)
    {
        file.write(dynamic.data, dynamic.size);
        util::parameter_message(2, "data", dynamic.size, dynamic.size, file.tellp());
    }

    util::verification_message("end_file", file_size, (uint64_t)file.tellp());

    return true;
}

ResourcePack::ResourcePack(ResourcePack &&other) noexcept
{
    _resource_types = std::move(other._resource_types);
    _dynamic_buffers = std::move(other._dynamic_buffers);
}

ResourcePack &ResourcePack::operator=(ResourcePack &&other) noexcept
{
    if(this != &other)
    {
        _resource_types = std::move(other._resource_types);
        _dynamic_buffers = std::move(other._dynamic_buffers);
    }
    return *this;
}

}