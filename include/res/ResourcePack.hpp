#ifndef RESOURCE_PACK_HPP
#define RESOURCE_PACK_HPP

#include "ResourceGroup.hpp"
#include "util/type/prs/DeferredData.hpp"

#define HEADER_PACK_SIZE 16

namespace mtrs::res
{

class ResourcePack
{
    const char _magic[8] = "mtrspck";
    uint64_t _cache_lifetime = 0;

    std::vector<ResourceGroup> _groups;
    std::vector<prs::DeferredData> _deferred_data;
    
    uint _size = 8;
    uint64_t _deferred_data_size = 0;

    bool _is_init = true;

public:
    ResourcePack() = default;
    ResourcePack(ResourcePack &) = delete;
    ResourcePack &operator=(const ResourcePack &) = delete;
    ResourcePack(ResourcePack &&other) noexcept;
    ResourcePack &operator=(ResourcePack &&other) noexcept;
    ~ResourcePack() = default;

    ResourcePack(simdjson::ondemand::object &pack_json, uint64_t cache_lifetime, std::string name);

    inline bool is_init() { return _is_init; }

    bool to_file_mtrs(std::ofstream &file);
};

}

#endif
