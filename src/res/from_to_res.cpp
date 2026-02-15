#include "res/Resource.h"

#include "util/jsonUtils.h"
#include "util/string_optimizer.h"
#include "util/to_dynamic_data.h"

#include <fstream>

std::string shader_path_to_string(std::string path);

void Resource::to_shaders(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
#include "res_struct/Shader.struct"
    size = sizeof(Shader_rs);
    date = malloc(size);
    Shader_rs *res = static_cast<Shader_rs*>(date);

    std::string name;
    std::string vertex;
    std::string fragment;

    GET_REQUIRED_FIELD(Shader_rs, name, std::string_view, obj, "name");
    GET_REQUIRED_FIELD(Shader_rs, vertex, std::string_view, obj, "pathVertex");
    GET_REQUIRED_FIELD(Shader_rs, fragment, std::string_view, obj, "pathFragment");

    vertex = shader_path_to_string(std::move(vertex));
    fragment = shader_path_to_string(std::move(fragment));

    SET_DYNAMIC_STRING(name, res, name, bufferDynamicDate);
    SET_DYNAMIC_STRING(vertex, res, vertex, bufferDynamicDate);
    SET_DYNAMIC_STRING(fragment, res, fragment, bufferDynamicDate);
}

std::string shader_path_to_string(std::string path)
{
    std::ifstream shader;
    std::string buffer;

    shader.open(path, std::ios::ate | std::ios::binary);
    if (!shader.is_open())
    {
        std::cerr << "!= Failed to open shader: " << path << " =!" << std::endl;
        return "";
    }
    buffer.resize(shader.tellg());
    shader.seekg(0);

    shader.read(buffer.data(), buffer.size());

    return stiring_optimizer(std::move(buffer));
}

void Resource::to_textures(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
#include "res_struct/Texture.struct"
    size = sizeof(Texture_rs);
    date = malloc(size);
    Texture_rs *res = static_cast<Texture_rs*>(date);

    std::string name;
    std::string path;

    GET_REQUIRED_FIELD(Texture_rs, name, std::string_view, obj, "name");
    GET_REQUIRED_FIELD(Texture_rs, path, std::string_view, obj, "path");
    
    SET_DYNAMIC_STRING(name, res, name, bufferDynamicDate);
    SET_DYNAMIC_STRING(path, res , path, bufferDynamicDate);
}

void Resource::to_atlases(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
#include "res_struct/TextureAtlas.struct"
    size = sizeof(TextureAtlas_rs);
    date = malloc(size);
    TextureAtlas_rs *res = static_cast<TextureAtlas_rs*>(date);

    std::string name;

    GET_REQUIRED_FIELD(TextureAtlas_rs, name, std::string_view, obj, "name");
    
    GET_REQUIRED_FIELD(TextureAtlas_rs, res->width, uint64_t, obj, "width");
    GET_REQUIRED_FIELD(TextureAtlas_rs, res->height, uint64_t, obj, "height");
    GET_REQUIRED_FIELD(TextureAtlas_rs, res->subWidth, uint64_t, obj, "subWidth");
    GET_REQUIRED_FIELD(TextureAtlas_rs, res->subHeight, uint64_t, obj, "subHeight");
    
    SET_DYNAMIC_STRING(name, res, name, bufferDynamicDate);
}

void Resource::to_sounds(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{

}

void Resource::to_musics(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    
}