#include "res/Resource.hpp"

#include "util/type/prs/DeferredData.hpp"

#include "util/fun/math/hash.hpp"
#include "util/fun/msg/mtrs_file_message.hpp"

#include <fstream>

namespace mtrs::res
{

Resource::Resource(simdjson::ondemand::object &obj, const std::string &type_name,
    uint64_t type_id, std::vector<prs::DeferredData> &deferred_data)
{
    std::vector<prs::DeferredData> ddata;
    switch (type_id)
    {
#define X(res) case math::hash64(#res): ddata = to_##res(obj); break;
    RESOURCE_TYPES
#undef X
    default:
        msg::mtrs_error("There is no such resource as \"", type_name, '\"');
        _is_init = false;
    }
    deferred_data.insert(deferred_data.end(),
        std::make_move_iterator(ddata.begin()), std::make_move_iterator(ddata.end()));

    if(_name == "")
    {
        msg::mtrs_error("Resource of type \"", type_name, "\" has no name");
        _is_init = false;
    }
}

bool Resource::to_file_mtscn(std::ofstream &file, size_t msg_offset)
{
    uint64_t id = math::hash64(_name);
    file.write(reinterpret_cast<char*>(&id), sizeof(id));
    msg::parameter_message(msg_offset, _name, id, sizeof(id), file.tellp());
    msg_offset += 2;

    file.write(reinterpret_cast<char*>(_data), _size);
    msg::parameter_message(msg_offset, "data", _size, _size, file.tellp());

    return true;
}

Resource::Resource(Resource &&other) noexcept
{
    _name = std::move(other._name);
    _is_init = other._is_init;
    other._is_init = false;
    _size = other._size;
    other._size = 0;
    _data = other._data;
    other._data = nullptr;
}

Resource &Resource::operator=(Resource &&other) noexcept
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

Resource::~Resource()
{
    free(_data);
    _data = nullptr;
}

}