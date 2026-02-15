#ifndef COMPONENT_H
#define COMPONENT_H

#include "simdjson.h"

#include "util/DynamicDataBuffer.h"

#define COMPONENT_TYPE \
X(Transform)\
X(Parend)\
X(Children)\
X(Sprite)\
X(Animator)\
X(StateAnimator)\
X(Sound)\
X(Music)\
X(CursorFollower)


class Component
{
    uint64_t id = 0;
    uint32_t size = 0;
    void *date = nullptr;
public:
    Component() = default;
    Component(Component &) = delete;
    Component &operator=(const Component &) = delete;
    Component(Component &&other) noexcept;
    Component &operator=(Component &&other) noexcept;
    ~Component();
    
    uint32_t get_size() { return size; }

    bool from_json(
        simdjson::ondemand::object &obj,
        const std::string &compName,
        std::vector<DynamicDataBuffer> &bufferDynamicDate);
    bool to_file_mtscn(std::ofstream &file);

#define X(comp) void to_##comp(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate);
    COMPONENT_TYPE
#undef X
};


#endif
