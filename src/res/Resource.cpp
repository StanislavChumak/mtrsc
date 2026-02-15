#include "from_to_res.cpp"

#include "util/hash.h"

bool Resource::from_json(
    simdjson::ondemand::object &obj,
    std::string &resName,
    std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    auto hash = hash_string(resName);
    switch (hash)
    {
#define X(res) case hash_c_string(#res):to_##res(obj, bufferDynamicDate);break;
    RESOURCE_TYPE
#undef X
    default:
        std::cerr << "!= There is no such resource as \"" << resName << "\" =!" << std::endl;
        return false;
    }

    if(!size)
    {
        std::cerr << "!= Resource of type \"" << resName << "\" was empty =!" << std::endl;
        return false;
    }
    return true;
}

bool Resource::to_file_mtscn(std::ofstream &file)
{
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(date), size);
    return true;
}

Resource::Resource(Resource &&other) noexcept
{
    size = other.size;
    other.size = 0;
    date = other.date;
    other.date = nullptr;
}

Resource &Resource::operator=(Resource &&other) noexcept
{
    if(this != &other)
    {
        size = other.size;
        other.size = 0;
        date = other.date;
        other.date = nullptr;
    }
    return *this;
}

Resource::~Resource()
{
    free(date);
    date = nullptr;
}