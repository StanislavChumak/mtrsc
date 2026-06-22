#ifndef DYNAMIC_BUFFER_HPP
#define DYNAMIC_BUFFER_HPP

#include <cstdint>

struct DynamicBuffer
{
    uint32_t* p_offset;
    uint32_t size;
    char *date;
};

#endif
