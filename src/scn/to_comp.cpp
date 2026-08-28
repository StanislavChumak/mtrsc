#include "scn/Component.hpp"

#include "util/fun/prs/json.hpp"
#include "util/fun/math/hash.hpp"

#include "util/type/prs/DeferredData.hpp"
#include "util/type/prs/comp/ScriptUpdate.hpp"
#include "util/type/prs/comp/ScriptCallback.hpp"
#include "util/type/prs/comp/Transform.hpp"
#include "util/type/prs/comp/Sprite.hpp"
#include "util/type/prs/comp/Animator.hpp"
#include "util/type/prs/comp/StateAnimator.hpp"
#include "util/type/prs/comp/SpriteMap.hpp"
#include "util/type/prs/comp/MapAnimator.hpp"
#include "util/type/prs/comp/Label.hpp"
#include "util/type/prs/comp/SoundPlayer.hpp"

namespace mtrs::comp
{

std::vector<prs::DeferredData> Component::to_ScriptUpdate(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::ScriptUpdate);
    _data = malloc(_size);
    prs::ScriptUpdate *comp = static_cast<prs::ScriptUpdate*>(_data);

    std::string script_file;
    prs::set_json_to_var<std::string_view>(script_file, obj, "script_file");

    return { prs::DeferredData{std::move(script_file), DEFERRED_ARGS(*comp, script_file)} };
}

std::vector<prs::DeferredData> Component::to_ScriptCallback(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::ScriptCallback);
    _data = malloc(_size);
    prs::ScriptCallback *comp = static_cast<prs::ScriptCallback*>(_data);

    std::string script_file;
    prs::set_json_to_var<std::string_view>(script_file, obj, "script_file");

    return { prs::DeferredData{std::move(script_file), DEFERRED_ARGS(*comp, script_file)} };
}

std::vector<prs::DeferredData> Component::to_Transform(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::Transform);
    _data = malloc(_size);
    prs::Transform *comp = static_cast<prs::Transform*>(_data);

    prs::set_json_to_var<int64_t>(comp->pos_x, obj, "pos_x", 0);
    prs::set_json_to_var<int64_t>(comp->pos_y, obj, "pos_y", 0);
    prs::set_json_to_var<double>(comp->scale_size_x, obj, "scale_size_x", 1.f);
    prs::set_json_to_var<double>(comp->scale_size_y, obj, "scale_size_y", 1.f);
    prs::set_json_to_var<double>(comp->rotation, obj, "rotation", 0.f);

    return {};
}

std::vector<prs::DeferredData> Component::to_Sprite(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::Sprite);
    _data = malloc(_size);
    prs::Sprite *comp = static_cast<prs::Sprite*>(_data);

    std::string shader, texture, atlas;
    std::vector<uint8_t> color;
    
    prs::set_json_to_var<std::string_view>(shader, obj, "shader");
    prs::set_json_to_var<std::string_view>(texture, obj, "texture");
    prs::set_json_to_var<std::string_view>(atlas, obj, "atlas", "");

    prs::set_json_to_var<double>(comp->layer, obj, "layer", 0.f);

    prs::set_json_to_var<uint64_t>(comp->size_x, obj, "size_x");
    prs::set_json_to_var<uint64_t>(comp->size_y, obj, "size_y");

    prs::set_json_to_array<uint64_t>(color, obj, "color", {0xFFu,0xFFu,0xFFu,0xFFu});
    std::memcpy(&comp->color, color.data(), sizeof(uint32_t));

    comp->visibility = true;

    return {
        prs::DeferredData{std::move(shader), DEFERRED_ARGS(*comp, shader)},
        prs::DeferredData{std::move(texture), DEFERRED_ARGS(*comp, texture)},
        prs::DeferredData{std::move(atlas), DEFERRED_ARGS(*comp, atlas)}
    };
}

std::vector<prs::DeferredData> Component::to_Animator(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::Animator);
    _data = malloc(_size);
    prs::Animator *comp = static_cast<prs::Animator*>(_data);

    std::vector<float> durations;

    prs::set_json_to_array<double>(durations, obj, "durations");

    comp->frame_offset = 0;
    comp->count_frame = durations.size();

    return { prs::DeferredData{std::move(durations), DEFERRED_ARGS(*comp, durations)} };
}

std::vector<prs::DeferredData> Component::to_StateAnimator(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::StateAnimator);
    _data = malloc(_size);
    prs::StateAnimator *comp = static_cast<prs::StateAnimator*>(_data);
    
    size_t count = prs::get_result_json<size_t>(obj.count_fields());
    std::vector<prs::StateAnimator::State> states;
    states.reserve(count);

    std::string name;
    prs::StateAnimator::State state;
    simdjson::ondemand::object state_json;
    for(auto field : obj)
    {
        name = (std::string)prs::get_result_json<std::string_view>(field.unescaped_key());
        state_json = prs::get_value_json<simdjson::ondemand::object>(field.value());

        state.id = math::hash32(std::move(name));
        prs::set_json_to_var<uint64_t>(state.offset, state_json, "offset");
        prs::set_json_to_var<uint64_t>(state.count, state_json, "count");
        states.push_back(state);
    }

    return { prs::DeferredData{std::move(states), DEFERRED_ARGS(*comp, states)} };
}

std::vector<prs::DeferredData> Component::to_SpriteMap(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::SpriteMap);
    _data = malloc(_size);
    prs::SpriteMap *comp = static_cast<prs::SpriteMap*>(_data);

    std::string shader, texture, atlas;
    std::vector<uint8_t> color;
    std::vector<uint32_t> types;
    std::vector<std::array<uint32_t, 3>> cells;
    
    prs::set_json_to_var<std::string_view>(shader, obj, "shader");
    prs::set_json_to_var<std::string_view>(texture, obj, "texture");
    prs::set_json_to_var<std::string_view>(atlas, obj, "atlas", "");

    prs::set_json_to_var<double>(comp->layer, obj, "layer", 0.f);

    prs::set_json_to_var<uint64_t>(comp->cell_size_x, obj, "cell_size_x");
    prs::set_json_to_var<uint64_t>(comp->cell_size_y, obj, "cell_size_y");

    prs::set_json_to_array<uint64_t>(color, obj, "color", {0xFFu,0xFFu,0xFFu,0xFFu});
    std::memcpy(&comp->color, color.data(), sizeof(uint32_t));

    prs::set_json_to_array<uint64_t>(types, obj, "cell_types");
    prs::set_json_to_array_of_array<uint64_t>(cells, obj, "cells");

    comp->visibility = true;

    return {
        prs::DeferredData{std::move(shader), DEFERRED_ARGS(*comp, shader)},
        prs::DeferredData{std::move(texture), DEFERRED_ARGS(*comp, texture)},
        prs::DeferredData{std::move(atlas), DEFERRED_ARGS(*comp, atlas)},
        prs::DeferredData{std::move(types), DEFERRED_ARGS(*comp, cell_types)},
        prs::DeferredData{std::move(cells), DEFERRED_ARGS(*comp, cell_map)}
    };
}

std::vector<prs::DeferredData> Component::to_MapAnimator(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::MapAnimator);
    _data = malloc(_size);
    prs::MapAnimator *comp = static_cast<prs::MapAnimator*>(_data);

    std::vector<float> durations;
    std::vector<std::array<uint32_t, 2>> cell_animators;

    prs::set_json_to_array<double>(durations, obj, "durations");
    prs::set_json_to_array_of_array<uint64_t>(cell_animators, obj, "cell_animators");
    
    return {
        prs::DeferredData{std::move(durations), DEFERRED_ARGS(*comp, durations)},
        prs::DeferredData{std::move(cell_animators), DEFERRED_ARGS(*comp, cell_animators)}
    };
}

std::vector<prs::DeferredData> Component::to_Label(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::Label);
    _data = malloc(_size);
    prs::Label *comp = static_cast<prs::Label*>(_data);

    std::string shader, text;
    std::vector<uint8_t> color;
    
    prs::set_json_to_var<std::string_view>(shader, obj, "shader");
    prs::set_json_to_var<std::string_view>(text, obj, "text");

    prs::set_json_to_var<double>(comp->layer, obj, "layer", 0.f);

    prs::set_json_to_var<uint64_t>(comp->size_x, obj, "size_x");
    prs::set_json_to_var<uint64_t>(comp->size_y, obj, "size_y");

    prs::set_json_to_array<uint64_t>(color, obj, "color", {0x0,0x0,0x0,0xFFu});
    std::memcpy(&comp->color, color.data(), sizeof(uint32_t));

    comp->visibility = true;

    return {
        prs::DeferredData{std::move(shader), DEFERRED_ARGS(*comp, shader)},
        prs::DeferredData{std::move(text), DEFERRED_ARGS(*comp, text)}
    };
}

std::vector<prs::DeferredData> Component::to_SoundPlayer(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::SoundPlayer);
    _data = malloc(_size);
    prs::SoundPlayer *comp = static_cast<prs::SoundPlayer*>(_data);

    std::string sound;

    prs::set_json_to_var<std::string_view>(sound, obj, "sound");
    prs::set_json_to_var<double>(comp->volume, obj, "volume", 1.f);

    return { prs::DeferredData{std::move(sound), DEFERRED_ARGS(*comp, sound)} };
}

}