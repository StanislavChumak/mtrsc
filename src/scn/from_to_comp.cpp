#include "scn/Component.h"

#include "util/jsonUtils.h"
#include "util/to_dynamic_data.h"

#include <fstream>

void Component::to_Transform(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
#include "comp_struct/Transform.struct"
    size = sizeof(Trancform_sc);
    date = malloc(size);
    Trancform_sc *comp = static_cast<Trancform_sc*>(date);
    comp->posX = 0;         set_in_var_json<int64_t>(comp->posX, obj["posX"]);
    comp->posY = 0;         set_in_var_json<int64_t>(comp->posY, obj["posY"]);
    comp->scaleSizeX = 1.f; set_in_var_json<double>(comp->scaleSizeX, obj["scaleSizeX"]);
    comp->scaleSizeY = 1.f; set_in_var_json<double>(comp->scaleSizeY, obj["scaleSizeY"]);
    comp->rotation = 0.f;   set_in_var_json<double>(comp->rotation, obj["rotation"]);
}

void Component::to_Parend(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{

}

void Component::to_Children(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{

}

void Component::to_Sprite(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
#include "comp_struct/Sprite.struct"
    size = sizeof(Sprite_sc);
    date = malloc(size);
    Sprite_sc *comp = static_cast<Sprite_sc*>(date);

    std::string shader;
    std::string texture;
    std::string atlas = "";
    comp->layer = 0.f;
    comp->color = 4294967295u;

    GET_REQUIRED_FIELD(Sprite_sc, shader, std::string_view, obj, shader);
    GET_REQUIRED_FIELD(Sprite_sc, texture, std::string_view, obj, texture);
    
    set_in_var_json<std::string_view>(atlas, obj["atlas"]);
    set_in_var_json<double>(comp->layer, obj["layer"]);
    
    GET_REQUIRED_FIELD(Sprite_sc, comp->sizeX, uint64_t, obj, "sizeX");
    GET_REQUIRED_FIELD(Sprite_sc, comp->sizeY, uint64_t, obj, "sizeY");

    simdjson::ondemand::array colorArray;
    if(set_in_var_json<simdjson::ondemand::array>(colorArray, obj["color"]))
    {
        uint8_t rgba[4];
        uint8_t i = 0;
        for(auto iter : colorArray)
            rgba[i++] = static_cast<uint8_t>(get_var_json<uint64_t>(iter));
        
        memcpy(&comp->color, rgba, sizeof(uint32_t));
    }

    SET_DYNAMIC_STRING(shader, comp, shader, bufferDynamicDate);
    SET_DYNAMIC_STRING(texture, comp, texture, bufferDynamicDate);
    SET_DYNAMIC_STRING(atlas, comp, atlas, bufferDynamicDate);
}

void Component::to_Animator(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
#include "comp_struct/Animator.struct"
    size = sizeof(Animator_sc);
    date = malloc(size);
    Animator_sc *comp = static_cast<Animator_sc*>(date);

    simdjson::ondemand::array durations;
    GET_REQUIRED_FIELD(Animator_sc, durations, simdjson::ondemand::array, obj, "durations");

    SET_DYNAMIC_ARRAY(durations, float, double, comp, durations, bufferDynamicDate);
}

void Component::to_StateAnimator(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
#include "comp_struct/StateAnimator.struct"
    size = sizeof(StateAnimator_sc);
    date = malloc(size);
    StateAnimator_sc *comp = static_cast<StateAnimator_sc*>(date);

    
    size_t countStates = get_result_json<size_t>(obj.count_fields());
    StateAnimator_sc::State *states = new StateAnimator_sc::State[countStates];
    comp->statesSize = countStates * sizeof(StateAnimator_sc::State);

    StateAnimator_sc::State *iter = states;
    for(auto field : obj)
    {
        std::string name = std::string(get_result_json<std::string_view>(field.unescaped_key()));
        simdjson::ondemand::object dataState = get_var_json<simdjson::ondemand::object>(field.value());

        GET_REQUIRED_FIELD(StateAnimator_sc, iter->offset, uint64_t, dataState, "offset");
        GET_REQUIRED_FIELD(StateAnimator_sc, iter->count, uint64_t, dataState, "count");

        SET_DYNAMIC_STRING(name, iter, name, bufferDynamicDate);

        iter++;
    }


    DynamicDataBuffer dynamicDate = {
        &comp->statesOffset,
        comp->statesSize,
        reinterpret_cast<char*>(states)
    };
    bufferDynamicDate.push_back(std::move(dynamicDate));
}

void Component::to_Sound(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{

}

void Component::to_Music(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{

}

void Component::to_CursorFollower(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
#include "comp_struct/CursorFollower.struct"
    size = sizeof(CursorFollower_sc);
    date = malloc(size);
    CursorFollower_sc *comp = static_cast<CursorFollower_sc*>(date);
    comp->offsetPosX = 0; set_in_var_json<int64_t>(comp->offsetPosX, obj["offsetX"]);
    comp->offsetPosY = 0; set_in_var_json<int64_t>(comp->offsetPosY, obj["offsetY"]);
}