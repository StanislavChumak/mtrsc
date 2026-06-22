#ifndef COMPONENT_H
#define COMPONENT_H

#include "simdjson.h"

#include "util/DynamicBuffer.hpp"

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
    uint64_t _id = 0;
    uint32_t _size = 0;
    void *_date = nullptr;

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
        std::vector<DynamicBuffer> &dynamic_buffers);
    bool to_file_mtscn(std::ofstream &file);

#define X(Comp) void to_##Comp(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers);
    COMPONENT_TYPE
#undef X
};


#endif
