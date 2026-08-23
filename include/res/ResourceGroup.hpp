#ifndef RESOURCE_GROUP_HPP
#define RESOURCE_GROUP_HPP

#include "Resource.hpp"

namespace mtrs::res
{

class ResourceGroup
{
    std::string _type;

    std::vector<Resource> _resources;
    uint64_t _size = sizeof(uint64_t) * 2;

    bool _is_init = true;

public:
    ResourceGroup() = delete;
    ResourceGroup(ResourceGroup &) = delete;
    ResourceGroup &operator=(const ResourceGroup &) = delete;
    ResourceGroup(ResourceGroup &&other) noexcept;
    ResourceGroup &operator=(ResourceGroup &&other) noexcept;
    ~ResourceGroup() = default;

    ResourceGroup(simdjson::ondemand::array &array, std::string type,
        std::vector<prs::DeferredData> &deferred_data);
    
    inline uint32_t get_cout() { return _resources.size(); }
    inline uint32_t size() { return _size; }
    inline bool is_init() { return _is_init; }

    bool to_file_mtscn(std::ofstream &file, size_t msg_offset);
};

}

#endif
