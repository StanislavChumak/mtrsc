#include "res/ResourceType.hpp"

#include "util/hash.hpp"
#include "util/mtrsc_message.hpp"

#include <fstream>

namespace mtrs::res
{

bool ResourceType::from_json(simdjson::ondemand::array &array, 
    std::string &name, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
    _name = name;

    for(simdjson::ondemand::object obj : array)
    {
        Resource res{};
        if(!res.from_json(obj, name, dynamic_buffers))
        {
            continue;
        }
        _size += res.size();
        _resources.push_back(std::move(res));
    }

    if(_resources.empty())
    {
        MTRS_ERROR("The ResourceType \"", name, "\" is empty");
        return false;
    }
    
    return true;
}

bool ResourceType::to_file_mtscn(std::ofstream &file)
{
    uint64_t id = mtrs::util::hash_string<uint64_t>(_name);
    uint64_t offset_to_next_type = static_cast<uint64_t>(file.tellp()) + _size;

    file.write(reinterpret_cast<char*>(&id), sizeof(id));
    util::parameter_message(2, _name, id, sizeof(id), file.tellp());

    util::variable_message(2, "size", _size);

    LOG_WRITE(file, 2, offset_to_next_type, "offset_to_next_type");

    util::variable_message(2, "resources", _resources.size());

    for(auto &res : _resources)
    {
        res.to_file_mtscn(file);
    }

    return true;
}

ResourceType::ResourceType(ResourceType &&other) noexcept
{
    _name = std::move(other._name);
    _resources = std::move(other._resources);
    _size = other._size;
}

ResourceType &ResourceType::operator=(ResourceType &&other) noexcept
{
    if(this != &other)
    {
        _name = std::move(other._name);
        _resources = std::move(other._resources);
        _size = other._size;
    }
    return *this;
}

}