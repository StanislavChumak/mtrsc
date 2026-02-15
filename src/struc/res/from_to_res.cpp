#include "struc/res/Resource.h"

#include "util/jsonUtils.h"
#include "util/hash.h"
#include "util/string_optimizer.h"
#include "util/to_dynamic_data.h"

#include <fstream>

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

void Resource::to_shaders(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    struct Shader
    {
        uint32_t offsetName;
        uint32_t sizeName;
        uint32_t offsetVertex;
        uint32_t sizeVertex;
        uint32_t offsetFragment;
        uint32_t sizeFragment;
    };
    size = sizeof(Shader);
    date = malloc(size);
    Shader *res = static_cast<Shader*>(date);

    std::string name;
    std::string vertex;
    std::string fragment;

    GET_REQUIRED_PARAMETER(Shader, name, std::string_view, obj, "name");
    GET_REQUIRED_PARAMETER(Shader, vertex, std::string_view, obj, "pathVertex");
    GET_REQUIRED_PARAMETER(Shader, fragment, std::string_view, obj, "pathFragment");

    vertex = shader_path_to_string(std::move(vertex));
    fragment = shader_path_to_string(std::move(fragment));

    SET_DYNAMIC_STRING(name, res, offsetName, sizeName, bufferDynamicDate);
    SET_DYNAMIC_STRING(vertex, res, offsetVertex, sizeVertex, bufferDynamicDate);
    SET_DYNAMIC_STRING(fragment, res, offsetFragment, sizeFragment, bufferDynamicDate);
}

void Resource::to_textures(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    struct Texture
    {
        uint32_t offsetName;
        uint32_t sizeName;
        uint32_t offsetPath;
        uint32_t sizePath;
    };
    size = sizeof(Texture);
    date = malloc(size);
    Texture *res = static_cast<Texture*>(date);

    std::string name;
    std::string path;

    GET_REQUIRED_PARAMETER(Texture, name, std::string_view, obj, "name");
    GET_REQUIRED_PARAMETER(Texture, path, std::string_view, obj, "path");
    
    SET_DYNAMIC_STRING(name, res, offsetName, sizeName, bufferDynamicDate);
    SET_DYNAMIC_STRING(path, res , offsetPath, sizePath, bufferDynamicDate);
}

void Resource::to_atlases(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    struct Shader
    {
        uint32_t offsetName;
        uint32_t sizeName;
        uint64_t width;
        uint64_t height;
        uint64_t subWidth;
        uint64_t subHeight;
    };
    size = sizeof(Shader);
    date = malloc(size);
    Shader *res = static_cast<Shader*>(date);

    std::string name;

    GET_REQUIRED_PARAMETER(Shader, name, std::string_view, obj, "name");
    
    GET_REQUIRED_PARAMETER(Shader, res->width, uint64_t, obj, "width");
    GET_REQUIRED_PARAMETER(Shader, res->height, uint64_t, obj, "height");
    GET_REQUIRED_PARAMETER(Shader, res->subWidth, uint64_t, obj, "subWidth");
    GET_REQUIRED_PARAMETER(Shader, res->subHeight, uint64_t, obj, "subHeight");
    
    SET_DYNAMIC_STRING(name, res, offsetName, sizeName, bufferDynamicDate);
}

void Resource::to_sounds(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{

}

void Resource::to_musics(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate)
{
    
}