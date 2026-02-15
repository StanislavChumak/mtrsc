#include "from_to_comp.cpp"


bool Component::from_json(
    simdjson::ondemand::object &obj,
    const std::string &compName,
    std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    id = hash_string(compName);
    switch (id)
    {
#define X(comp) case hash_c_string(#comp):to_##comp(obj, bufferDynamicDate);break;
    COMPONENT_TYPE
#undef X
    default:
        std::cerr << "!= There is no such component as \"" << compName << "\" =!" << std::endl;
        return false;
        break;
    }

    if(!size)
    {
        std::cerr << "!= Component of type \"" << compName << "\" was empty =!" << std::endl;
        return false;
    }
    size += 12;

    return true;
}

bool Component::to_file_mtscn(std::ofstream &file)
{
    file.write(reinterpret_cast<char*>(&id), sizeof(id));
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(reinterpret_cast<char*>(date), size);
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