#include "struc/scn/Component.h"

#include "util/hash.h"
#include "util/jsonUtils.h"
#include "util/to_dynamic_data.h"

#include <fstream>

void Component::to_Transform(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    struct Trancform
    {
        int64_t posX, posY;
        float scaleSizeX, scaleSizeY;
        float rotation;
    };
    size = sizeof(Trancform);
    date = malloc(size);
    Trancform *comp = static_cast<Trancform*>(date);
    comp->posX = 0;         set_in_var_json<int64_t>(comp->posX, obj["posX"]);
    comp->posY = 0;         set_in_var_json<int64_t>(comp->posX, obj["posY"]);
    comp->scaleSizeX = 1.f; set_in_var_json<double>(comp->posX, obj["scaleSizeX"]);
    comp->scaleSizeY = 1.f; set_in_var_json<double>(comp->posX, obj["scaleSizeY"]);
    comp->rotation = 0.f;   set_in_var_json<double>(comp->posX, obj["rotation"]);
}

void Component::to_Parend(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{

}

void Component::to_Children(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{

}

void Component::to_Sprite(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    struct Sprite
    {
        uint32_t offsetShader;
        uint32_t sizeShader;
        uint32_t offsetTexture;
        uint32_t sizeTexture;
        uint32_t offsetAtlas;
        uint32_t sizeAtlas;
        float layer;
        uint32_t sizeX, sizeY;
        uint32_t color;
    };
    size = sizeof(Sprite);
    date = malloc(size);
    Sprite *comp = static_cast<Sprite*>(date);

    std::string shader;
    std::string texture;
    std::string atlas = "";
    comp->layer = 0.f;
    comp->color = 4294967295u;

    GET_REQUIRED_PARAMETER(Sprite, shader, std::string_view, obj, shader);
    GET_REQUIRED_PARAMETER(Sprite, texture, std::string_view, obj, texture);
    
    set_in_var_json<std::string_view>(atlas, obj["atlas"]);
    set_in_var_json<double>(comp->layer, obj["layer"]);
    
    GET_REQUIRED_PARAMETER(Sprite, comp->sizeX, uint64_t, obj, "sizeX");
    GET_REQUIRED_PARAMETER(Sprite, comp->sizeY, uint64_t, obj, "sizeY");

    simdjson::ondemand::array colorArray;
    if(set_in_var_json<simdjson::ondemand::array>(colorArray, obj["color"]))
    {
        uint8_t rgba[4];
        uint8_t i = 0;
        for(auto iter : colorArray)
            rgba[i++] = static_cast<uint8_t>(get_var_json<uint64_t>(iter));
        
        memcpy(&comp->color, rgba, sizeof(uint32_t));
    }

    SET_DYNAMIC_STRING(shader, comp, offsetShader, sizeShader, bufferDynamicDate);
    SET_DYNAMIC_STRING(texture, comp, offsetTexture, sizeTexture, bufferDynamicDate);
    SET_DYNAMIC_STRING(atlas, comp, offsetAtlas, sizeAtlas, bufferDynamicDate);
}

void Component::to_Animator(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    struct Animator
    {
        uint32_t offsetDuration;
        uint32_t sizeDuration;
    };
    size = sizeof(Animator);
    date = malloc(size);
    Animator *comp = static_cast<Animator*>(date);

    simdjson::ondemand::array durations;
    GET_REQUIRED_PARAMETER(Animator, durations, simdjson::ondemand::array, obj, "durations");

    SET_DYNAMIC_ARRAY(durations, float, double, comp, offsetDuration, sizeDuration, bufferDynamicDate);
}

void Component::to_StateAnimator(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    struct StateAnimator
    {
        uint32_t offsetStates;
        uint32_t sizeStates;
    };
    size = sizeof(StateAnimator);
    date = malloc(size);
    StateAnimator *comp = static_cast<StateAnimator*>(date);

    struct State
    {
        std::string name;
        uint16_t offset;
        uint16_t count;
    };
    size_t countStates = get_result_json<size_t>(obj.count_fields());
    State *states = new State[countStates];
    comp->sizeStates = countStates * sizeof(State);

    int i = 0;
    for(auto field : obj)
    {
        std::string name = std::string(get_result_json<std::string_view>(field.unescaped_key()));
        simdjson::ondemand::object dataState = get_var_json<simdjson::ondemand::object>(field.value());

        uint16_t offset, count;
        GET_REQUIRED_PARAMETER(StateAnimator, offset, uint64_t, dataState, "offset");
        GET_REQUIRED_PARAMETER(StateAnimator, count, uint64_t, dataState, "count");

        states[i++] = {name, offset, count};
    }


    DynamicDataBuffer dynamicDate = {
        &comp->offsetStates,
        comp->sizeStates,
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
    struct CursorFollower
    {
        int64_t offsetPosX, offsetPosY;
    };
    size = sizeof(CursorFollower);
    date = malloc(size);
    CursorFollower *comp = static_cast<CursorFollower*>(date);
    comp->offsetPosX = 0; set_in_var_json<int64_t>(comp->offsetPosX, obj["offsetX"]);
    comp->offsetPosY = 0; set_in_var_json<int64_t>(comp->offsetPosY, obj["offsetY"]);
}