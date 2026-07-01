#include "scn/Component.hpp"

#include "util/from_json.hpp"
#include "util/to_dynamic_data.hpp"
#include "util/hash.hpp"

#include <fstream>

#include "dynamic_field.def"

namespace mtrs::comp
{

void Component::to_Transform(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/Transform.struct"
    _size = sizeof(Trancform_sc);
    _data = malloc(_size);
    Trancform_sc *comp = static_cast<Trancform_sc*>(_data);
    comp->pos_x = 0;          util::set_in_var_json<int64_t>(comp->pos_x, obj["pos_x"]);
    comp->pos_y = 0;          util::set_in_var_json<int64_t>(comp->pos_y, obj["pos_y"]);
    comp->scale_size_x = 1.f; util::set_in_var_json<double>(comp->scale_size_x, obj["scale_size_x"]);
    comp->scale_size_y = 1.f; util::set_in_var_json<double>(comp->scale_size_y, obj["scale_size_y"]);
    comp->rotation = 0.f;     util::set_in_var_json<double>(comp->rotation, obj["rotation"]);
}

void Component::to_Sprite(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/Sprite.struct"
    _size = sizeof(Sprite_sc);
    _data = malloc(_size);
    Sprite_sc *comp = static_cast<Sprite_sc*>(_data);

    std::string shader;
    std::string texture;
    std::string atlas = "";
    comp->layer = 0.f;
    comp->color = 4294967295u;

    IS_SET_FIELD(Sprite_sc, shader, std::string_view, obj["shader"]);
    SET_DYNAMIC_STRING(shader, comp, shader, dynamic_buffers);

    IS_SET_FIELD(Sprite_sc, texture, std::string_view, obj["texture"]);
    SET_DYNAMIC_STRING(texture, comp, texture, dynamic_buffers);
    
    util::set_in_var_json<std::string_view>(atlas, obj["atlas"]);
    util::set_in_var_json<double>(comp->layer, obj["layer"]);
    SET_DYNAMIC_STRING(atlas, comp, atlas, dynamic_buffers);
    
    IS_SET_FIELD(Sprite_sc, comp->size_x, uint64_t, obj["size_x"]);
    IS_SET_FIELD(Sprite_sc, comp->size_y, uint64_t, obj["size_y"]);

    simdjson::ondemand::array color_array;
    if(util::set_in_var_json<simdjson::ondemand::array>(color_array, obj["color"]))
    {
        uint8_t rgba[4];
        uint8_t i = 0;
        for(auto iter : color_array)
            rgba[i++] = static_cast<uint8_t>(util::get_var_json<uint64_t>(iter));
        
        memcpy(&comp->color, rgba, sizeof(uint32_t));
    }
}

void Component::to_Animator(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/Animator.struct"
    _size = sizeof(Animator_sc);
    _data = malloc(_size);
    Animator_sc *comp = static_cast<Animator_sc*>(_data);

    simdjson::ondemand::array durations;
    IS_SET_FIELD(Animator_sc, durations, simdjson::ondemand::array, obj["durations"]);

    SET_DYNAMIC_ARRAY(durations, float, double, comp, durations, dynamic_buffers);
}

void Component::to_StateAnimator(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/StateAnimator.struct"
    _size = sizeof(StateAnimator_sc);
    _data = malloc(_size);
    StateAnimator_sc *comp = static_cast<StateAnimator_sc*>(_data);
    
    size_t count = util::get_result_json<size_t>(obj.count_fields());
    StateAnimator_sc::State *data_states = new StateAnimator_sc::State[count];
    comp->states_size = count * sizeof(StateAnimator_sc::State);

    StateAnimator_sc::State *iter = data_states;
    std::string name;
    simdjson::ondemand::object state_json;
    for(auto field : obj)
    {
        name = (std::string)util::get_result_json<std::string_view>(field.unescaped_key());
        state_json = util::get_var_json<simdjson::ondemand::object>(field.value());

        iter->id_state = util::hash_string<uint32_t>(name);
        IS_SET_FIELD(StateAnimator_sc, iter->offset, uint64_t, state_json["offset"]);
        IS_SET_FIELD(StateAnimator_sc, iter->count, uint64_t, state_json["count"]);

        iter++;
    }

    util::DynamicBuffer dynamic_data = {
        &comp->states_offset,
        comp->states_size,
        reinterpret_cast<char*>(data_states)
    };
    dynamic_buffers.push_back(std::move(dynamic_data));
}

void Component::to_SpriteMap(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/SpriteMap.struct"
    _size = sizeof(SpriteMap_sc);
    _data = malloc(_size);
    SpriteMap_sc *comp = static_cast<SpriteMap_sc*>(_data);

    std::string shader;
    std::string texture;
    std::string atlas;
    comp->layer = 0.f;
    comp->color = 4294967295u;

    IS_SET_FIELD(SpriteMap_sc, shader, std::string_view, obj["shader"]);
    SET_DYNAMIC_STRING(shader, comp, shader, dynamic_buffers);

    IS_SET_FIELD(SpriteMap_sc, texture, std::string_view, obj["texture"]);
    SET_DYNAMIC_STRING(texture, comp, texture, dynamic_buffers);

    IS_SET_FIELD(SpriteMap_sc, atlas, std::string_view, obj["atlas"]);
    SET_DYNAMIC_STRING(atlas, comp, atlas, dynamic_buffers);

    util::set_in_var_json<double>(comp->layer, obj["layer"]);

    IS_SET_FIELD(SpriteMap_sc, comp->cell_size_x, uint64_t, obj["size_x"]);
    IS_SET_FIELD(SpriteMap_sc, comp->cell_size_y, uint64_t, obj["size_y"]);

    simdjson::ondemand::array color_array;
    if(util::set_in_var_json<simdjson::ondemand::array>(color_array, obj["color"]))
    {
        uint8_t rgba[4];
        uint8_t i = 0;
        for(auto iter : color_array)
            rgba[i++] = static_cast<uint8_t>(util::get_var_json<uint64_t>(iter));
        
        memcpy(&comp->color, rgba, sizeof(uint32_t));
    }

    simdjson::ondemand::array types ,cells;

    IS_SET_FIELD(SpriteMap_sc, types, simdjson::ondemand::array, obj["cell_types"]);
    SET_DYNAMIC_ARRAY(types, uint32_t, uint64_t, comp, cell_types, dynamic_buffers);

    IS_SET_FIELD(SpriteMap_sc, cells, simdjson::ondemand::array, obj["cells"]);
    SET_DYNAMIC_ARRAY_OF_ARRAY(cells, 3, uint32_t, uint64_t, comp, cell_map, dynamic_buffers);
}

void Component::to_MapAnimator(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/MapAnimator.struct"
    _size = sizeof(MapAnimator_sc);
    _data = malloc(_size);
    MapAnimator_sc *comp = static_cast<MapAnimator_sc*>(_data);

    simdjson::ondemand::array ranges, durations;
    
    IS_SET_FIELD(MapAnimator_sc, ranges, simdjson::ondemand::array, obj["ranges"]);
    SET_DYNAMIC_ARRAY_OF_ARRAY(ranges, 2, uint32_t, uint64_t, comp, ranges, dynamic_buffers);
    
    IS_SET_FIELD(MapAnimator_sc, durations, simdjson::ondemand::array, obj["durations"]);
    SET_DYNAMIC_ARRAY(durations, float, double, comp, durations, dynamic_buffers);
}

void Component::to_SoundPlayer(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/SoundPlayer.struct"
    _size = sizeof(SoundPlayer_sc);
    _data = malloc(_size);
    SoundPlayer_sc *comp = static_cast<SoundPlayer_sc*>(_data);

    std::string sound;
    IS_SET_FIELD(SoundPlayer_sc, sound, std::string_view, obj["sound"]);
    SET_DYNAMIC_STRING(sound, comp, sound, dynamic_buffers);

    util::set_in_var_json<double>(comp->volume, obj["volume"]);
}

void Component::to_CursorFollower(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "comp_struct/CursorFollower.struct"
    _size = sizeof(CursorFollower_sc);
    _data = malloc(_size);
    CursorFollower_sc *comp = static_cast<CursorFollower_sc*>(_data);
    comp->offset_pos_x = 0; util::set_in_var_json<int64_t>(comp->offset_pos_x, obj["offset_x"]);
    comp->offset_pos_y = 0; util::set_in_var_json<int64_t>(comp->offset_pos_y, obj["offset_y"]);
}

}