#include "res/ResourceGroup.hpp"

#include "util/fun/math/hash.hpp"
#include "util/fun/msg/mtrs_file_message.hpp"

#include <fstream>

namespace mtrs::res
{

ResourceGroup::ResourceGroup(simdjson::ondemand::array &array, 
    std::string type, std::vector<prs::DeferredData> &deferred_data)
: _type(std::move(type))
{
    for(simdjson::ondemand::object obj : array)
    {
        Resource res{obj, _type, math::hash64(_type), deferred_data};
        if(!res.is_init())
        {
            continue;
        }
        _size += res.size();
        _resources.push_back(std::move(res));
    }

    if(_resources.empty())
    {
        msg::mtrs_error("The ResourceGroup \"", type, "\" is empty");
        _is_init = false;
    }
}

bool ResourceGroup::to_file_mtscn(std::ofstream &file, size_t msg_offset)
{
    uint64_t id = math::hash64(_type);
    uint64_t next_group = static_cast<uint64_t>(file.tellp()) + _size;

    file.write(reinterpret_cast<char*>(&id), sizeof(id));
    msg::parameter_message(msg_offset, _type, id, sizeof(id), file.tellp());
    msg_offset += 2;
    
    msg::variable_message(msg_offset, "size", _size);
    
    file.write(reinterpret_cast<char*>(&next_group), sizeof(next_group));
    msg::parameter_message(msg_offset, "next_group", next_group, sizeof(next_group), file.tellp());

    msg::variable_message(msg_offset, "resources", _resources.size());

    for(auto &res : _resources)
    {
        res.to_file_mtscn(file, msg_offset + 2);
    }

    return true;
}

ResourceGroup::ResourceGroup(ResourceGroup &&other) noexcept
{
    _type = std::move(other._type);
    _resources = std::move(other._resources);
    _size = other._size;
}

ResourceGroup &ResourceGroup::operator=(ResourceGroup &&other) noexcept
{
    if(this != &other)
    {
        _type = std::move(other._type);
        _resources = std::move(other._resources);
        _size = other._size;
    }
    return *this;
}

}