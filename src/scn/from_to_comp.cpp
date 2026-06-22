#include "scn/Component.hpp"

#include "util/from_json.hpp"
#include "util/to_dynamic_data.hpp"

#include <fstream>

void Component::to_Transform(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/Transform.struct"
    _size = sizeof(Trancform_sc);
    _date = malloc(_size);
    Trancform_sc *comp = static_cast<Trancform_sc*>(_date);
    comp->pos_x = 0;         set_in_var_json<int64_t>(comp->pos_x, obj["pos_x"]);
    comp->pos_y = 0;         set_in_var_json<int64_t>(comp->pos_y, obj["pos_y"]);
    comp->scale_size_x = 1.f;set_in_var_json<double>(comp->scale_size_x, obj["scale_size_x"]);
    comp->scale_size_y = 1.f;set_in_var_json<double>(comp->scale_size_y, obj["scale_size_y"]);
    comp->rotation = 0.f;    set_in_var_json<double>(comp->rotation, obj["rotation"]);
}

void Component::to_Parend(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{

}

void Component::to_Children(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{

}

void Component::to_Sprite(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/Sprite.struct"
    _size = sizeof(Sprite_sc);
    _date = malloc(_size);
    Sprite_sc *comp = static_cast<Sprite_sc*>(_date);

    std::string shader;
    std::string texture;
    std::string atlas = "";
    comp->layer = 0.f;
    comp->color = 4294967295u;

    IS_SET_FIELD(Sprite_sc, shader, std::string_view, obj["shader"]);
    IS_SET_FIELD(Sprite_sc, texture, std::string_view, obj["texture"]);
    
    set_in_var_json<std::string_view>(atlas, obj["atlas"]);
    set_in_var_json<double>(comp->layer, obj["layer"]);
    
    IS_SET_FIELD(Sprite_sc, comp->size_x, uint64_t, obj["size_x"]);
    IS_SET_FIELD(Sprite_sc, comp->size_y, uint64_t, obj["size_y"]);

    simdjson::ondemand::array colorArray;
    if(set_in_var_json<simdjson::ondemand::array>(colorArray, obj["color"]))
    {
        uint8_t rgba[4];
        uint8_t i = 0;
        for(auto iter : colorArray)
            rgba[i++] = static_cast<uint8_t>(get_var_json<uint64_t>(iter));
        
        memcpy(&comp->color, rgba, sizeof(uint32_t));
    }

    SET_DYNAMIC_STRING(shader, comp, shader, dynamic_buffers);
    SET_DYNAMIC_STRING(texture, comp, texture, dynamic_buffers);
    SET_DYNAMIC_STRING(atlas, comp, atlas, dynamic_buffers);
}

void Component::to_Animator(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/Animator.struct"
    _size = sizeof(Animator_sc);
    _date = malloc(_size);
    Animator_sc *comp = static_cast<Animator_sc*>(_date);

    simdjson::ondemand::array durations;
    IS_SET_FIELD(Animator_sc, durations, simdjson::ondemand::array, obj["durations"]);

    SET_DYNAMIC_ARRAY(durations, float, double, comp, durations, dynamic_buffers);
}

void Component::to_StateAnimator(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/StateAnimator.struct"
    _size = sizeof(StateAnimator_sc);
    _date = malloc(_size);
    StateAnimator_sc *comp = static_cast<StateAnimator_sc*>(_date);
    
    size_t count = get_result_json<size_t>(obj.count_fields());
    StateAnimator_sc::State *states = new StateAnimator_sc::State[count];
    comp->states_size = count * sizeof(StateAnimator_sc::State);

    StateAnimator_sc::State *iter = states;
    for(auto field : obj)
    {
        std::string name = std::string(get_result_json<std::string_view>(field.unescaped_key()));
        simdjson::ondemand::object state_json = get_var_json<simdjson::ondemand::object>(field.value());

        IS_SET_FIELD(StateAnimator_sc, iter->offset, uint64_t, state_json["offset"]);
        IS_SET_FIELD(StateAnimator_sc, iter->count, uint64_t, state_json["count"]);

        SET_DYNAMIC_STRING(name, iter, name, dynamic_buffers);

        iter++;
    }


    DynamicBuffer dynamicDate = {
        &comp->states_offset,
        comp->states_size,
        reinterpret_cast<char*>(states)
    };
    dynamic_buffers.push_back(std::move(dynamicDate));
}

void Component::to_Sound(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{

}

void Component::to_Music(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{

}

void Component::to_CursorFollower(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/CursorFollower.struct"
    _size = sizeof(CursorFollower_sc);
    _date = malloc(_size);
    CursorFollower_sc *comp = static_cast<CursorFollower_sc*>(_date);
    comp->offset_pos_x = 0; set_in_var_json<int64_t>(comp->offset_pos_x, obj["offset_x"]);
    comp->offset_pos_y = 0; set_in_var_json<int64_t>(comp->offset_pos_y, obj["offset_y"]);
}