#include "from_to_comp.cpp"

#include "util/hash.hpp"
#include "util/LogSystem.hpp"

bool Component::from_json(
    simdjson::ondemand::object &obj,
    const std::string &name,
    std::vector<DynamicBuffer> &dynamic_buffer)
{
    _name = name;
    _id = hash_string(_name);
    switch (_id)
    {
#define X(comp) case hash_c_string(#comp):to_##comp(obj, dynamic_buffer);break;
    COMPONENT_TYPE
#undef X
    default:
        LogSystem::print_err(LogSystem::args_to_str("There is no such component as \"", name, "\""));
        return false;
        break;
    }

    if(_size == 0)
    {
        LogSystem::print_err(LogSystem::args_to_str("Component of type \"", name, "\" was empty"));
        return false;
    }

    return true;
}

bool Component::to_file_mtscn(std::ofstream &file)
{
    FILE_WRITE(file, _id);
    LogSystem::print_parameter("\t  "+_name, std::to_string(_id), sizeof(_id), file.tellp());

    file.write(reinterpret_cast<char*>(_date), _size);
    LogSystem::print_parameter("\t    comp_data", std::to_string(_size), _size, file.tellp());
    return true;
}

Component::Component(Component &&other) noexcept
{
    _id = other._id;
    other._id = 0;
    _size = other._size;
    other._size = 0;
    _date = other._date;
    other._date = nullptr;
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
        _date = other._date;
        other._date = nullptr;
        _name = std::move(other._name);
    }
    return *this;
}

Component::~Component()
{
    free(_date);
    _date = nullptr;
}