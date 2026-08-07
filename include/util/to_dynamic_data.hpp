#ifndef TO_DYNAMIC_DATA_HPP
#define TO_DYNAMIC_DATA_HPP

#include "util/from_json.hpp"
#include "util/mtrsc_message.hpp"
#include <string>

namespace mtrs::util
{

struct DynamicBuffer
{
    uint32_t* p_offset;
    uint32_t size;
    char *data;
};

#define IS_SET_FIELD(object, var, type_for_json, json_field) \
if(!mtrs::util::set_in_var_json<type_for_json>(var, json_field)) \
    mtrs::util::mtrsc_error(#object, " failed to get required field: ", #json_field);

#define SET_DYNAMIC_STRING(string, ptr, field, dynamic_data) \
dynamic_data.push_back(mtrs::util::string_to_dynamic_data(std::move(string), &ptr->field##_offset, ptr->field##_size))

#define SET_DYNAMIC_ARRAY(array, type_array, type_for_json, ptr, field, dynamic_data) \
dynamic_data.push_back(mtrs::util::array_to_dynamic_data<type_array, type_for_json>(array, &ptr->field##_offset, ptr->field##_size))

#define SET_DYNAMIC_ARRAY_OF_ARRAY(array, width, type_array, type_for_json, ptr, field, dynamic_data) \
dynamic_data.push_back(mtrs::util::array_of_array_to_dynamic_data<type_array, type_for_json>(array, width, &ptr->field##_offset, ptr->field##_size))

DynamicBuffer string_to_dynamic_data(std::string str, uint32_t *offset, uint32_t &size);

template<typename T, typename TypeForJson = T>
DynamicBuffer array_to_dynamic_data(simdjson::ondemand::array &array, uint32_t *offset, uint32_t &size)
{
    size_t count = get_result_json<size_t>(array.count_elements());
    size = static_cast<uint32_t>(sizeof(T) * count);

    T *buffer = new T[count];

    int i = 0;
    for(auto iter : array)
    {
        buffer[i++] = static_cast<T>(get_var_json<TypeForJson>(iter));
    }

    return {offset, size, reinterpret_cast<char*>(buffer)};
}

template<typename T, typename TypeForJson = T>
DynamicBuffer array_of_array_to_dynamic_data(simdjson::ondemand::array &arr_arr, size_t width, uint32_t *offset, uint32_t &size)
{
    size_t count = get_result_json<size_t>(arr_arr.count_elements());
    size = static_cast<uint32_t>(sizeof(T) * width * count);

    T *buffer = new T[count * width];

    simdjson::ondemand::array row;
    size_t x = 0, y = 0;
    for(auto element : arr_arr)
    {
        row = util::get_var_json<simdjson::ondemand::array>(element);

        for(auto cell : row)
        {
            buffer[y * width + x] = static_cast<T>(get_var_json<TypeForJson>(cell));
            x++;
        }
        x=0;
        y++;
    }

    return {offset, size, reinterpret_cast<char*>(buffer)};
}

}

#endif
