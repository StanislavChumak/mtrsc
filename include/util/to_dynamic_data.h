#ifndef TO_DYNAMIC_DATA_H
#define TO_DYNAMIC_DATA_H

#include "util/DynamicDataBuffer.h"
#include "util/jsonUtils.h"
#include <string>

#define GET_REQUIRED_PARAMETER(object, param, jsonType, source, jsonName) \
if(!set_in_var_json<jsonType>(param, source[jsonName])) \
    std::cerr << "!= " << #object << " failed to get required parameter: " << jsonName << std::endl

#define SET_DYNAMIC_STRING(string, ptr, offset, size, vectorDynamicDate) \
auto string##dynamic = string_to_dynamic_date(std::move(string), &ptr->offset, ptr->size); \
vectorDynamicDate.push_back(std::move(string##dynamic))

#define SET_DYNAMIC_ARRAY(array, arrayType, jsonType, ptr, offset, size, vectorDynamicDate) \
auto array##dynamic = array_to_dynamic_date<arrayType, jsonType>(array, &ptr->offset, ptr->size); \
vectorDynamicDate.push_back(std::move(array##dynamic))

DynamicDataBuffer string_to_dynamic_date(std::string str, uint32_t *offset, uint32_t &size);


template<typename T, typename getJsonType = T>
DynamicDataBuffer array_to_dynamic_date(simdjson::ondemand::array array, uint32_t *offset, uint32_t &size)
{
    size_t count = static_cast<uint32_t>(get_result_json<size_t>(array.count_elements()));
    size = static_cast<uint32_t>(sizeof(T) * count);
    T *buffer =  new T[count];
    int i = 0;
    for(auto iter : array)
        buffer[i++] = static_cast<T>(get_var_json<getJsonType>(iter));
    
    DynamicDataBuffer dynamicDate = {
        offset,
        size,
        reinterpret_cast<char*>(buffer)
    };
    return std::move(dynamicDate);
}

#endif
