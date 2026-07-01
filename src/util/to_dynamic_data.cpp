#include "util/to_dynamic_data.hpp"

namespace mtrs::util
{

DynamicBuffer string_to_dynamic_data(std::string str, uint32_t *offset, uint32_t &size)
{
    size = static_cast<uint32_t>(str.size());
    char *data = new char[size];
    std::copy(str.begin(), str.end(), data);

    return {offset, size, data};
}

}