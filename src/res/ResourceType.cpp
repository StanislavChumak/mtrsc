#include "res/ResourceType.hpp"

#include "util/hash.hpp"
#include "util/LogSystem.hpp"

#include <fstream>

bool ResourceType::from_json(simdjson::ondemand::array &array, 
    std::string &name, std::vector<DynamicBuffer> &dynamic_buffers)
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
        LogSystem::print_err(LogSystem::args_to_str("The ResourceType \"", name, "\" is empty"));
        return false;
    }
    
    return true;
}

bool ResourceType::to_file_mtscn(std::ofstream &file)
{
    uint64_t id = hash_string(_name);
    uint64_t offset_to_next_type = static_cast<uint64_t>(file.tellp()) + _size;

    FILE_WRITE(file, id);
    LogSystem::print_parameter("  "+_name, std::to_string(id), sizeof(id), file.tellp());

    LogSystem::print_variable("  size", std::to_string(_size));

    LOG_WRITE(file, offset_to_next_type, "  offset_to_next_type");

    LogSystem::print_variable("  resources", std::to_string(_resources.size()));

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