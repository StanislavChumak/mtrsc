#ifndef COMPONENT_H
#define COMPONENT_H

#include "simdjson.h"

#include "comp_struct/comp_type.def"

namespace mtrs::util
{
    struct DynamicBuffer;
}

namespace mtrs::comp
{

class Component
{
    uint64_t _id = 0;
    uint32_t _size = 0;
    void *_data = nullptr;

    std::string _name;
    
public:
    Component() = default;
    Component(Component &) = delete;
    Component &operator=(const Component &) = delete;
    Component(Component &&other) noexcept;
    Component &operator=(Component &&other) noexcept;
    ~Component();
    
    uint32_t size() { return _size + sizeof(_id); }

    bool from_json(
        simdjson::ondemand::object &obj,
        const std::string &name,
        std::vector<util::DynamicBuffer> &dynamic_buffers);
    bool to_file_mtscn(std::ofstream &file);

#define X(Comp) void to_##Comp(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers);
    COMPONENT_TYPE
#undef X
};

}

#endif
