#ifndef RESOURCE_PACK_HPP
#define RESOURCE_PACK_HPP

#include "ResourceType.hpp"

#define HEADER_PACK_SIZE 8

class ResourcePack
{
    const char _magic[4] = {'m','t','r','s'};
    float _version = 0.f;

    std::vector<ResourceType> _resource_types;
    std::vector<DynamicBuffer> _dynamic_buffers;
    
    uint _size = 8;
    uint64_t _dynamic_date_size = 0;

public:
    ResourcePack(float version);
    ResourcePack(ResourcePack &) = delete;
    ResourcePack &operator=(const ResourcePack &) = delete;
    ResourcePack(ResourcePack &&other) noexcept;
    ResourcePack &operator=(ResourcePack &&other) noexcept;
    ~ResourcePack() = default;

    bool from_json(simdjson::ondemand::object &jsonRes, std::string name);
    bool to_file_mtrs(std::ofstream &file);
};


#endif
