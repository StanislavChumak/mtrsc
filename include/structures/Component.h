#ifndef COMPONENT_H
#define COMPONENT_H

#include "simdjson.h"
#include <cstdint>

struct Component
{
    uint64_t id = 0;
    uint32_t size = 0;
    void *date = nullptr;

    bool from_json(std::string name, simdjson::ondemand::object obj);
    Component() = default;
    Component(Component &) = delete;
    Component &operator=(const Component &) = delete;
    Component(Component &&other) noexcept;
    Component &operator=(Component &&other) noexcept;
    ~Component();
};


#endif
