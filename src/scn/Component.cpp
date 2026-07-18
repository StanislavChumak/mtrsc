#include "from_to_comp.cpp"

#include "util/mtrsc_message.hpp"

namespace mtrs::comp
{

bool Component::from_json(
    simdjson::ondemand::object &obj,
    const std::string &name,
    std::vector<util::DynamicBuffer> &dynamic_buffer)
{
    _name = name;
    _id = util::hash_string<uint64_t>(_name);
    switch (_id)
    {
#define X(comp) case util::hash_c_string<uint64_t>(#comp):to_##comp(obj, dynamic_buffer);break;
    COMPONENT_TYPE
#undef X
    default:
        util::mtrsc_error("There is no such component as \"", name, "\"");
        return false;
        break;
    }

    if(_size == 0)
    {
        util::mtrsc_error("Component of type \"", name, "\" was empty");
        return false;
    }

    return true;
}

bool Component::to_file_mtscn(std::ofstream &file)
{
    file.write(reinterpret_cast<char*>(&_id), sizeof(_id));
    util::parameter_message(6, _name, _id, sizeof(_id), file.tellp());

    file.write(reinterpret_cast<char*>(_data), _size);
    util::parameter_message(8, "comp_data", _size, _size, file.tellp());
    
    return true;
}

Component::Component(Component &&other) noexcept
{
    _id = other._id;
    other._id = 0;
    _size = other._size;
    other._size = 0;
    _data = other._data;
    other._data = nullptr;
    _name = std::move(other._name);
}

Component &Component::operator=(Component &&other) noexcept
{
    if(this != &other)
    {
        _id = other._id;
        other._id = 0;
        _size = other._size;
        other._size = 0;
        _data = other._data;
        other._data = nullptr;
        _name = std::move(other._name);
    }
    return *this;
}

Component::~Component()
{
    free(_data);
    _data = nullptr;
}

}