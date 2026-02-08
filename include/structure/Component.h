#ifndef COMPONENT_H
#define COMPONENT_H

#include "simdjson.h"
#include <cstdint>

struct DynamicDataBuffer
{
    uint32_t* pOffset;
    size_t sizeData;
    char *date;
};

struct Component
{
    uint64_t id = 0;
    uint32_t size = 0;
    void *date = nullptr;

    Component() = default;
    Component(Component &) = delete;
    Component &operator=(const Component &) = delete;
    Component(Component &&other) noexcept;
    Component &operator=(Component &&other) noexcept;
    ~Component();

    bool from_json(std::string name, std::vector<DynamicDataBuffer> &bufferDynamicDate, simdjson::ondemand::object obj);
};


#endif
