#include "res/Resource.hpp"

#include "util/from_json.hpp"
#include "util/string_optimizer.hpp"
#include "util/to_dynamic_data.hpp"
#include "util/hash.hpp"
#include "util/mtrsc_message.hpp"

#include <fstream>

#include "dynamic_field.def"

namespace mtrs::res
{

std::string shader_path_to_string(std::string path);

void Resource::to_shaders(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Shader.struct"
    _size = sizeof(Shader_rs);
    _data = malloc(_size);
    Shader_rs *res = static_cast<Shader_rs*>(_data);

    std::string name;
    std::string vertex;
    std::string fragment;

    IS_SET_FIELD(Shader_rs, name, std::string_view, obj["name"]);
    IS_SET_FIELD(Shader_rs, vertex, std::string_view, obj["vertex"]);
    IS_SET_FIELD(Shader_rs, fragment, std::string_view, obj["fragment"]);

    vertex = shader_path_to_string(std::move(vertex));
    fragment = shader_path_to_string(std::move(fragment));
    
    SET_DYNAMIC_STRING(vertex, res, vertex, dynamic_buffers);
    SET_DYNAMIC_STRING(fragment, res, fragment, dynamic_buffers);

    _id = util::hash_string<uint64_t>(name);
}

std::string shader_path_to_string(std::string path)
{
    std::ifstream shader;
    std::string buffer;

    shader.open(path, std::ios::ate | std::ios::binary);
    if (!shader.is_open())
    {
        util::mtrsc_message(util::TypeMessage::ERROR ,"Failed to open shader: ", path);
        return "";
    }
    buffer.resize(shader.tellg());
    shader.seekg(0);

    shader.read(buffer.data(), buffer.size());

    return util::stiring_optimizer(std::move(buffer));
}

void Resource::to_textures(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Texture.struct"
    _size = sizeof(Texture_rs);
    _data = malloc(_size);
    Texture_rs *res = static_cast<Texture_rs*>(_data);

    std::string name;
    std::string path;

    IS_SET_FIELD(Texture_rs, name, std::string_view, obj["name"]);
    IS_SET_FIELD(Texture_rs, path, std::string_view, obj["path"]);

    SET_DYNAMIC_STRING(path, res , path, dynamic_buffers);

    _id = util::hash_string<uint64_t>(name);
}

void Resource::to_atlases(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/TextureAtlas.struct"
    _size = sizeof(TextureAtlas_rs);
    _data = malloc(_size);
    TextureAtlas_rs *res = static_cast<TextureAtlas_rs*>(_data);

    std::string name;

    IS_SET_FIELD(TextureAtlas_rs, name, std::string_view, obj["name"]);
    
    IS_SET_FIELD(TextureAtlas_rs, res->width, uint64_t, obj["width"]);
    IS_SET_FIELD(TextureAtlas_rs, res->height, uint64_t, obj["height"]);
    IS_SET_FIELD(TextureAtlas_rs, res->sub_width, uint64_t, obj["sub_width"]);
    IS_SET_FIELD(TextureAtlas_rs, res->sub_height, uint64_t, obj["sub_height"]);
    
    _id = util::hash_string<uint64_t>(name);
}

void Resource::to_sounds(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Sound.struct"
    _size = sizeof(Sound_sc);
    _data = malloc(_size);
    Sound_sc *res = static_cast<Sound_sc*>(_data);

    std::string name;
    std::string path;

    IS_SET_FIELD(Sound_sc, name, std::string_view, obj["name"]);

    IS_SET_FIELD(Sound_sc, path, std::string_view, obj["path"]);
    SET_DYNAMIC_STRING(path, res , path, dynamic_buffers);

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

    _id = util::hash_string<uint64_t>(name);
}

void Resource::to_render_context(simdjson::ondemand::object &obj, std::vector<util::DynamicBuffer> &dynamic_buffers)
{
    std::string name;
    IS_SET_FIELD(RenderContext, name, std::string_view, obj["name"]);
    _id = util::hash_string<uint64_t>(name);
}

}