#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "simdjson.h"

#include "util/type/prs/comp/comp_types.hpp"

namespace mtrs::prs
{
    struct DeferredData;
}

namespace mtrs::comp
{

class Component
{
    std::string _name;
    bool _is_init = true;

    uint32_t _size = 0;
    void *_data = nullptr;

#define X(Comp) std::vector<prs::DeferredData> to_##Comp(simdjson::ondemand::object &obj);
    COMPONENT_TYPES
#undef X

public:
    Component() = delete;
    Component(Component &) = delete;
    Component &operator=(const Component &) = delete;
    Component(Component &&other) noexcept;
    Component &operator=(Component &&other) noexcept;
    ~Component();

    Component(simdjson::ondemand::object &obj, const std::string &name,
        std::vector<prs::DeferredData> &dederred_data);

    inline bool is_init() noexcept { return _is_init; };
    inline uint32_t size() { return _size + sizeof(uint64_t); }

    bool to_file_mtscn(std::ofstream &file, size_t msg_offset);
};

}

#endif
