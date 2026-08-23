#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "simdjson.h"

#include "util/type/prs/res/res_types.hpp"

namespace mtrs::prs
{
    struct DeferredData;
}

namespace mtrs::res
{

class Resource
{
    std::string _name = "";
    bool _is_init = true;

    void *_data = nullptr;
    uint32_t _size = 0;

#define X(res) std::vector<prs::DeferredData> to_##res(simdjson::ondemand::object &obj);
    RESOURCE_TYPES
#undef X

public:
    Resource() = delete;
    Resource(Resource &) = delete;
    Resource &operator=(const Resource &) = delete;
    Resource(Resource &&other) noexcept;
    Resource &operator=(Resource &&other) noexcept;
    ~Resource();

    Resource(simdjson::ondemand::object &obj, const std::string &type_name,
        uint64_t type_id, std::vector<prs::DeferredData> &deferred_data);
    
    inline uint32_t size() { return _size + sizeof(uint64_t); }
    inline bool is_init() { return _is_init; }

    bool to_file_mtscn(std::ofstream &file, size_t msg_offset);
};

}

#endif
