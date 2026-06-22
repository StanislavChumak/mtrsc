#include "from_to_res.cpp"

bool Resource::from_json(simdjson::ondemand::object &obj,
    std::string &res_name, std::vector<DynamicBuffer> &dynamic_buffers)
{
    uint64_t hash = hash_string(res_name);
    switch (hash)
    {
#define X(res) case hash_c_string(#res):to_##res(obj, dynamic_buffers);break;
    RESOURCE_TYPE
#undef X
    default:
        LogSystem::print_err(LogSystem::args_to_str("There is no such resource as \"", res_name,'\"'));
        return false;
    }

    if(_id == 0)
    {
        std::cerr << "!= Resource of type \"" << res_name << "\" has no name =!" << std::endl;
    }

    // if(_size == sizeof(_size)) 
    // {
    //     std::cerr << "!= Resource of type \"" << res_name << "\" was empty =!" << std::endl;
    //     return false;
    // }

    return true;
}

bool Resource::to_file_mtscn(std::ofstream &file)
{
    LOG_WRITE(file, _id, "\tres_id");

    file.write(reinterpret_cast<char*>(_date), _size);
    LogSystem::print_parameter("\t  res_data", std::to_string(_size), _size, file.tellp());

    return true;
}

Resource::Resource(Resource &&other) noexcept
{
    _id = other._id;
    other._id = 0;
    _size = other._size;
    other._size = 0;
    _date = other._date;
    other._date = nullptr;
}

Resource &Resource::operator=(Resource &&other) noexcept
{
    if(this != &other)
    {
        _id = other._id;
        other._id = 0;
        _size = other._size;
        other._size = 0;
        _date = other._date;
        other._date = nullptr;
    }
    return *this;
}

Resource::~Resource()
{
    free(_date);
    _date = nullptr;
}