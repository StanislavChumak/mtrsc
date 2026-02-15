#ifndef DYNAMIC_DATA_BUFFER_H
#define DYNAMIC_DATA_BUFFER_H

#include <cstdint>

struct DynamicDataBuffer
{
    uint32_t* pOffset;
    uint32_t sizeData;
    char *date;
};

#endif
