#ifndef TO_DYNAMIC_DATA_HPP
#define TO_DYNAMIC_DATA_HPP

#include "util/DynamicBuffer.hpp"
#include "util/from_json.hpp"
#include <string>

#include "dynamic_field.def"

#define IS_SET_FIELD(object, var, type_for_json, json_field) \
if(!set_in_var_json<type_for_json>(var, json_field)) \
    std::cerr << "!= " << #object << " failed to get required field: " << #json_field << std::endl

#define SET_DYNAMIC_STRING(string, ptr, field, dynamic_date) \
dynamic_date.push_back(string_to_dynamic_date(std::move(string), &ptr->field##_offset, ptr->field##_size))

#define SET_DYNAMIC_ARRAY(array, type_array, type_for_json, ptr, field, dynamic_date) \
dynamic_date.push_back(array_to_dynamic_date<type_array, type_for_json>(array, &ptr->field##_offset, ptr->field##_size))


DynamicBuffer string_to_dynamic_date(std::string str, uint32_t *offset, uint32_t &size);

template<typename T, typename TypeForJson = T>
DynamicBuffer array_to_dynamic_date(simdjson::ondemand::array array, uint32_t *offset, uint32_t &size)
{
    size_t count = static_cast<uint32_t>(get_result_json<size_t>(array.count_elements()));
    size = static_cast<uint32_t>(sizeof(T) * count);

    T *buffer =  new T[count];

    int i = 0;
    for(auto iter : array)
    {
        buffer[i++] = static_cast<T>(get_var_json<TypeForJson>(iter));
    }

    return {offset, size, reinterpret_cast<char*>(buffer)};
}

#endif
