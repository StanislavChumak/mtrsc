#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "simdjson.h"

#include "res_struct/res_type.def"

namespace mtrs::util
{
    struct DynamicBuffer;
}

namespace mtrs::res
{

class Resource
{
    uint64_t _id = 0;
    void *_data = nullptr;
    
    uint32_t _size = 0;

public:
    Resource() = default;
    Resource(Resource &) = delete;
    Resource &operator=(const Resource &) = delete;
    Resource(Resource &&other) noexcept;
    Resource &operator=(Resource &&other) noexcept;
    ~Resource();
    
    uint32_t size() { return _size + sizeof(_id); }

    bool from_json(
        simdjson::ondemand::object &obj,
        std::string &resource, 
        std::vector<util::DynamicBuffer> &dynamic_buffer);
    bool to_file_mtscn(std::ofstream &file);

#define X(res) void to_##res(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffer);
    RESOURCE_TYPE
#undef X
};

}

#endif
