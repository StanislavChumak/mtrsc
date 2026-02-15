#include "util/to_dynamic_data.h"

DynamicDataBuffer string_to_dynamic_date(std::string str, uint32_t *offset, uint32_t &size)
{
    size = static_cast<uint32_t>(str.size());
    char *data = new char[size];
    std::copy(str.begin(), str.end(), data);

    DynamicDataBuffer dynamicDate = {
        offset,
        size,
        data
    };
    return std::move(dynamicDate);
}