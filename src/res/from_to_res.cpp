#include "res/Resource.hpp"

#include "util/from_json.hpp"
#include "util/string_optimizer.hpp"
#include "util/to_dynamic_data.hpp"
#include "util/hash.hpp"
#include "util/LogSystem.hpp"

#include <fstream>

std::string shader_path_to_string(std::string path);

void Resource::to_shaders(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Shader.struct"
    _size = sizeof(Shader_rs);
    _date = malloc(_size);
    Shader_rs *res = static_cast<Shader_rs*>(_date);

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
    _id = hash_string(name);
}

std::string shader_path_to_string(std::string path)
{
    std::ifstream shader;
    std::string buffer;

    shader.open(path, std::ios::ate | std::ios::binary);
    if (!shader.is_open())
    {
        LogSystem::print_err("Failed to open shader: " + path);
        return "";
    }
    buffer.resize(shader.tellg());
    shader.seekg(0);

    shader.read(buffer.data(), buffer.size());

    return stiring_optimizer(std::move(buffer));
}

void Resource::to_textures(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/Texture.struct"
    _size = sizeof(Texture_rs);
    _date = malloc(_size);
    Texture_rs *res = static_cast<Texture_rs*>(_date);

    std::string name;
    std::string path;

    IS_SET_FIELD(Texture_rs, name, std::string_view, obj["name"]);
    IS_SET_FIELD(Texture_rs, path, std::string_view, obj["path"]);

    SET_DYNAMIC_STRING(path, res , path, dynamic_buffers);
    _id = hash_string(name);
}

void Resource::to_atlases(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
#include "res_struct/TextureAtlas.struct"
    _size = sizeof(TextureAtlas_rs);
    _date = malloc(_size);
    TextureAtlas_rs *res = static_cast<TextureAtlas_rs*>(_date);

    std::string name;

    IS_SET_FIELD(TextureAtlas_rs, name, std::string_view, obj["name"]);
    
    IS_SET_FIELD(TextureAtlas_rs, res->width, uint64_t, obj["width"]);
    IS_SET_FIELD(TextureAtlas_rs, res->height, uint64_t, obj["height"]);
    IS_SET_FIELD(TextureAtlas_rs, res->sub_width, uint64_t, obj["sub_width"]);
    IS_SET_FIELD(TextureAtlas_rs, res->sub_height, uint64_t, obj["sub_height"]);
    
    _id = hash_string(name);
}

void Resource::to_sounds(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{

}

void Resource::to_musics(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
    
}

void Resource::to_render_context(simdjson::ondemand::object &obj, std::vector<DynamicBuffer> &dynamic_buffers)
{
    std::string name;
    IS_SET_FIELD(RenderContext, name, std::string_view, obj["name"]);
    _id = hash_string(name);
}