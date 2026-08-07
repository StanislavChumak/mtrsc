#include "res/Resource.hpp"

#include "util/from_json.hpp"
#include "util/string_optimizer.hpp"
#include "util/to_dynamic_data.hpp"
#include "util/hash.hpp"
#include "util/mtrsc_message.hpp"

#include "dynamic_field.def"

namespace mtrs::res
{

void Resource::to_scripts(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/ScriptFile.struct"
    _size = sizeof(ScriptFile_sc);
    _data = malloc(_size);
    ScriptFile_sc *res = static_cast<ScriptFile_sc*>(_data);

    std::string str;

    IS_SET_FIELD(ScriptFile_sc, str, std::string_view, obj["name"]);
    _id = util::hash_string<uint64_t>(std::move(str));

    IS_SET_FIELD(ScriptFile_sc, str, std::string_view, obj["path"]);
    SET_DYNAMIC_STRING(str, res, path, dynamic_buffers);
}

void Resource::to_shaders(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Shader.struct"
    _size = sizeof(Shader_rs);
    _data = malloc(_size);
    Shader_rs *res = static_cast<Shader_rs*>(_data);

    std::string str;

    IS_SET_FIELD(Shader_rs, str, std::string_view, obj["name"]);
    _id = util::hash_string<uint64_t>(std::move(str));

    IS_SET_FIELD(Shader_rs, str, std::string_view, obj["vertex"]);
    SET_DYNAMIC_STRING(str, res, vertex, dynamic_buffers);

    IS_SET_FIELD(Shader_rs, str, std::string_view, obj["fragment"]);
    SET_DYNAMIC_STRING(str, res, fragment, dynamic_buffers);
}

void Resource::to_textures(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Texture.struct"
    _size = sizeof(Texture_rs);
    _data = malloc(_size);
    Texture_rs *res = static_cast<Texture_rs*>(_data);

    std::string str;

    IS_SET_FIELD(Texture_rs, str, std::string_view, obj["name"]);
    _id = util::hash_string<uint64_t>(std::move(str));

    IS_SET_FIELD(Texture_rs, str, std::string_view, obj["path"]);
    SET_DYNAMIC_STRING(str, res , path, dynamic_buffers);

    res->max_instances = 1000;
    util::set_in_var_json<int64_t>(res->max_instances, obj["max_instances"]);
}

void Resource::to_atlases(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/TextureAtlas.struct"
    _size = sizeof(TextureAtlas_rs);
    _data = malloc(_size);
    TextureAtlas_rs *res = static_cast<TextureAtlas_rs*>(_data);

    std::string str;

    IS_SET_FIELD(TextureAtlas_rs, str, std::string_view, obj["name"]);
    _id = util::hash_string<uint64_t>(std::move(str));
    
    IS_SET_FIELD(TextureAtlas_rs, str, std::string_view, obj["texture"]);
    SET_DYNAMIC_STRING(str, res, texture, dynamic_buffers);

    IS_SET_FIELD(TextureAtlas_rs, res->sub_width, uint64_t, obj["sub_width"]);
    IS_SET_FIELD(TextureAtlas_rs, res->sub_height, uint64_t, obj["sub_height"]);

    res->spirality = false;
    util::set_in_var_json<bool>(res->spirality, obj["spirality"]);
}

void Resource::to_sounds(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Sound.struct"
    _size = sizeof(Sound_sc);
    _data = malloc(_size);
    Sound_sc *res = static_cast<Sound_sc*>(_data);

    std::string str;

    IS_SET_FIELD(Sound_sc, str, std::string_view, obj["name"]);
    _id = util::hash_string<uint64_t>(std::move(str));

    IS_SET_FIELD(Sound_sc, str, std::string_view, obj["path"]);
    SET_DYNAMIC_STRING(str, res, path, dynamic_buffers);

    res->count = 1; util::set_in_var_json<uint64_t>(res->count, obj["count"]);

    bool flags[4];
    std::memset(flags, 0, 4);

    util::set_in_var_json<bool>(flags[0], obj["looping"]);
    util::set_in_var_json<bool>(flags[1], obj["stream"]);
    util::set_in_var_json<bool>(flags[2], obj["position"]);
    util::set_in_var_json<bool>(flags[3], obj["pitch"]);

    for(int i = 0; i < 4; i++)
    {
        res->flag |= (uint32_t)flags[i] << i;
    }
}

void Resource::to_fonts(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Font.struct"
    _size = sizeof(Font_sc);
    _data = malloc(_size);
    Font_sc *res = static_cast<Font_sc*>(_data);

    std::string str;

    IS_SET_FIELD(Font_sc, str, std::string_view, obj["name"]);
    _id = util::hash_string<uint64_t>(std::move(str));
    
    IS_SET_FIELD(Font_sc, str, std::string_view, obj["texture"]);
    SET_DYNAMIC_STRING(str, res, texture, dynamic_buffers);

    IS_SET_FIELD(Font_sc, str, std::string_view, obj["symbols"]);
    SET_DYNAMIC_STRING(str, res, symbols, dynamic_buffers);

    simdjson::ondemand::array symbol_widths;
    IS_SET_FIELD(Font_sc, symbol_widths, simdjson::ondemand::array, obj["symbol_widths"]);
    SET_DYNAMIC_ARRAY_OF_ARRAY(symbol_widths, 2, int32_t, int64_t, res, symbol_widths, dynamic_buffers);

    IS_SET_FIELD(TextureAtlas_rs, res->symbol_height, uint64_t, obj["symbol_height"]);
}

}