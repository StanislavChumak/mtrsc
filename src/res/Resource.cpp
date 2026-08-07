#include "from_to_res.cpp"

#include <fstream>

namespace mtrs::res
{

bool Resource::from_json(simdjson::ondemand::object &obj,
    std::string &res_name, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
    uint64_t hash = util::hash_string<uint64_t>(res_name);
    switch (hash)
    {
#define X(res) case util::hash_c_string<u_int64_t>(#res):to_##res(obj, dynamic_buffers); break;
    RESOURCE_TYPE
#undef X
    default:
        util::mtrsc_error("There is no such resource as \"", res_name, '\"');
        return false;
    }

    if(_id == 0)
    {
        util::mtrsc_error("Resource of type \"", res_name, "\" has no name");
        return false;
    }

    return true;
}

bool Resource::to_file_mtscn(std::ofstream &file)
{
    LOG_WRITE(file, 4, _id, "res_id");

    file.write(reinterpret_cast<char*>(_data), _size);
    util::parameter_message(6, "res_data", _size, _size, file.tellp());

    return true;
}

Resource::Resource(Resource &&other) noexcept
{
    _id = other._id;
    other._id = 0;
    _size = other._size;
    other._size = 0;
    _data = other._data;
    other._data = nullptr;
}

Resource &Resource::operator=(Resource &&other) noexcept
{
    if(this != &other)
    {
        _id = other._id;
        other._id = 0;
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