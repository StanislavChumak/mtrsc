#ifndef COMPONENT_H
#define COMPONENT_H

#include "simdjson.h"
#include <cstdint>

struct Component
{
    uint64_t id = 0;
    uint32_t size = 0;
    u_int8_t *date = nullptr;

    bool from_json(u_int64_t id, simdjson::ondemand::object obj);
    Component() = default;
    Component(Component &) = delete;
    Component &operator=(const Component &) = delete;
    Component(Component &&other) noexcept;
    Component &operator=(Component &&other) noexcept;
    ~Component();
};


#endif
