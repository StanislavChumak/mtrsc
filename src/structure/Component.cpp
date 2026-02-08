#include "structure/Component.h"

#include "utilities/hash.h"
#include "utilities/jsonUtils.h"

#include <cstdlib>
#include <utility>

bool Component::from_json(std::string name, std::vector<DynamicDataBuffer> &bufferDynamicDate, simdjson::ondemand::object obj)
{
    switch (hash_c_string(name.c_str()))
    {
    case hash_c_string("Transform"):
    {
        struct Trancform
        {
            float posX, posY;
            float scaleSizeX, scaleSizeY;
            float rotation;
        };
        size = sizeof(Trancform);
        date = malloc(size);
        Trancform *comp = static_cast<Trancform*>(date);
        comp->posX = 0.f; set_var_json<double>(comp->posX, obj["posX"]);
        comp->posY = 0.f; set_var_json<double>(comp->posX, obj["posY"]);
        comp->scaleSizeX = 1.f; set_var_json<double>(comp->posX, obj["scaleSizeX"]);
        comp->scaleSizeY = 1.f; set_var_json<double>(comp->posX, obj["scaleSizeY"]);
        comp->rotation = 0.f; set_var_json<double>(comp->posX, obj["rotation"]);
        break;
    }
    case hash_c_string("Parend"):
    {
        break;
    }
    case hash_c_string("Children"):
    {
        break;
    }
    case hash_c_string("Sprite"):
    {
        break;
    }
    case hash_c_string("Animator"):
    {
        struct Animator
        {
            uint32_t offsetFrameDuration;
        };
        size = sizeof(Animator);
        date = malloc(size);
        Animator *comp = static_cast<Animator*>(date);
        std::vector<float> buffer;
        for(auto frameDuration : get_var_json<simdjson::ondemand::array>(obj["durations"]))
            buffer.push_back(get_var_json<double>(frameDuration.value()));
        DynamicDataBuffer dynamicDate = {
            &comp->offsetFrameDuration,
            sizeof(float) * buffer.size(),
            reinterpret_cast<char*>(buffer.data())};
        bufferDynamicDate.push_back(std::move(dynamicDate));
        break;
    }
    case hash_c_string("StateAnimator"):
    {
        break;
    }
    case hash_c_string("Sound"):
    {
        break;
    }
    case hash_c_string("Music"):
    {
        break;
    }
    case hash_c_string("CursorFollower"):
    {
        break;
    }
    default:
        return false;
        break;
    }
    this->id = hash_string(name);
    // size = 8;
    // date = new u_int8_t(0);

    return true;
}

Component::Component(Component &&other) noexcept
{
    id = other.id;
    size = other.size;
    other.size = 0;
    date = other.date;
    other.date = nullptr;
}

Component &Component::operator=(Component &&other) noexcept
{
    if(this != &other)
    {
        id = other.id;
        size = other.size;
        other.size = 0;
        date = other.date;
        other.date = nullptr;
    }
    return *this;
}

Component::~Component()
{
    free(date);
    date = nullptr;
}