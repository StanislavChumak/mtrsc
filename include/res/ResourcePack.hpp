#ifndef RESOURCE_PACK_HPP
#define RESOURCE_PACK_HPP

#include "ResourceType.hpp"
#include "util/to_dynamic_data.hpp"

#define HEADER_PACK_SIZE 8

namespace mtrs::res
{

class ResourcePack
{
    const char _magic[8] = "mtrspck";

    std::vector<ResourceType> _resource_types;
    std::vector<util::DynamicBuffer> _dynamic_buffers;
    
    uint _size = 8;
    uint64_t _dynamic_data_size = 0;

public:
    ResourcePack() = default;
    ResourcePack(ResourcePack &) = delete;
    ResourcePack &operator=(const ResourcePack &) = delete;
    ResourcePack(ResourcePack &&other) noexcept;
    ResourcePack &operator=(ResourcePack &&other) noexcept;
    ~ResourcePack() = default;

    bool from_json(simdjson::ondemand::object &jsonRes, std::string name);
    bool to_file_mtrs(std::ofstream &file);
};

}

#endif
