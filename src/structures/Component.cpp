#include "structures/Component.h"

bool Component::from_json(u_int64_t id, simdjson::ondemand::object obj)
{
    switch (id)
    {
    case 0xC1FFF4F356DFB2FB:
        /* code */
        break;
    
    default:
        // return false;
        break;
    }
    this->id = id;
    size = 8;
    date = new u_int8_t(0);

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
    delete date;
    date = nullptr;
}