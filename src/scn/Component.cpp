#include "scn/Component.hpp"

#include "util/type/prs/DeferredData.hpp"

#include "util/fun/msg/mtrs_file_message.hpp"
#include "util/fun/math/hash.hpp"

#include <fstream>

namespace mtrs::comp
{

Component::Component(simdjson::ondemand::object &obj, const std::string &name,
    std::vector<prs::DeferredData> &deferred_data)
: _name(std::move(name))
{
    std::vector<prs::DeferredData> ddata;
    switch (math::hash64(_name))
    {
#define X(comp) case math::hash64(#comp): ddata = to_##comp(obj);break;
        COMPONENT_TYPES
#undef X
        default:
        msg::mtrs_error("There is no such component as \"", _name, "\"");
        _is_init = false;
        break;
    }
    deferred_data.insert(deferred_data.end(), ddata.begin(), ddata.end());

    if(_size == 0)
    {
        msg::mtrs_error("Component of type \"", _name, "\" was empty");
        _is_init = false;
    }
}

bool Component::to_file_mtscn(std::ofstream &file, size_t msg_offset)
{
    uint64_t id = math::hash64(_name);
    file.write(reinterpret_cast<char*>(&id), sizeof(id));
    msg::parameter_message(msg_offset, _name, id, sizeof(id), file.tellp());
    msg_offset += 2;

    file.write(reinterpret_cast<char*>(_data), _size);
    msg::parameter_message(msg_offset, "comp_data", _size, _size, file.tellp());
    
    return true;
}

Component::Component(Component &&other) noexcept
{
    _name = std::move(other._name);
    _is_init = other._is_init;
    other._is_init = false;
    _size = other._size;
    other._size = 0;
    _data = other._data;
    other._data = nullptr;
}

Component &Component::operator=(Component &&other) noexcept
{
    if(this != &other)
    {
        _name = std::move(other._name);
        _is_init = other._is_init;
        other._is_init = false;
        _size = other._size;
        other._size = 0;
        _data = other._data;
        other._data = nullptr;
    }
    return *this;
}

Component::~Component()
{
    free(_data);
    _data = nullptr;
}

}