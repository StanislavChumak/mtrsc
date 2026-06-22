#ifndef RESOURCE_TYPE_HPP
#define RESOURCE_TYPE_HPP

#include "Resource.hpp"

class ResourceType
{
    std::string _name;

    std::vector<Resource> _resources;
    uint64_t _size = sizeof(uint64_t) * 2;

public:
    ResourceType() = default;
    ResourceType(ResourceType &) = delete;
    ResourceType &operator=(const ResourceType &) = delete;
    ResourceType(ResourceType &&other) noexcept;
    ResourceType &operator=(ResourceType &&other) noexcept;
    ~ResourceType() = default;

    uint32_t get_cout() { return _resources.size(); }
    uint32_t size() { return _size; }

    bool from_json(
        simdjson::ondemand::array &array,
        std::string &name,
        std::vector<DynamicBuffer> &dynamic_buffer);
    bool to_file_mtscn(std::ofstream &file);
};

#endif
