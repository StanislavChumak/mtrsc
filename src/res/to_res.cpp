#include "res/Resource.hpp"

#include "util/fun/prs/json.hpp"
#include "util/fun/math/hash.hpp"

#include "util/type/prs/DeferredData.hpp"
#include "util/type/prs/res/ScriptFile.hpp"
#include "util/type/prs/res/Shader.hpp"
#include "util/type/prs/res/Texture.hpp"
#include "util/type/prs/res/TextureAtlas.hpp"
#include "util/type/prs/res/Sound.hpp"
#include "util/type/prs/res/Font.hpp"

namespace mtrs::res
{

std::vector<prs::DeferredData> Resource::to_scripts(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::ScriptFile);
    _data = malloc(_size);
    prs::ScriptFile *res = static_cast<prs::ScriptFile*>(_data);

    std::string path;

    prs::set_json_to_var<std::string_view>(_name, obj, "name");
    prs::set_json_to_var<std::string_view>(path, obj, "path");

    return { prs::DeferredData{std::move(path), DEFERRED_ARGS(*res, path)} };
}

std::vector<prs::DeferredData> Resource::to_shaders(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::Shader);
    _data = malloc(_size);
    prs::Shader *res = static_cast<prs::Shader*>(_data);

    std::string vertex, fragment;

    prs::set_json_to_var<std::string_view>(_name, obj, "name");
    prs::set_json_to_var<std::string_view>(vertex, obj, "vertex");
    prs::set_json_to_var<std::string_view>(fragment, obj, "fragment");

    return {
        prs::DeferredData{std::move(vertex), DEFERRED_ARGS(*res, vertex)},
        prs::DeferredData{std::move(fragment), DEFERRED_ARGS(*res, fragment)}
    };
}

std::vector<prs::DeferredData> Resource::to_textures(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::Texture);
    _data = malloc(_size);
    prs::Texture *res = static_cast<prs::Texture*>(_data);

    std::string path;

    prs::set_json_to_var<std::string_view>(_name, obj, "name");
    prs::set_json_to_var<std::string_view>(path, obj, "path");
    prs::set_json_to_var<uint64_t>(res->max_instances, obj, "max_instances", 1000);

    return { prs::DeferredData{std::move(path), DEFERRED_ARGS(*res, path)} };
}

std::vector<prs::DeferredData> Resource::to_atlases(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::TextureAtlas);
    _data = malloc(_size);
    prs::TextureAtlas *res = static_cast<prs::TextureAtlas*>(_data);

    std::string texture;

    prs::set_json_to_var<std::string_view>(_name, obj, "name");
    prs::set_json_to_var<std::string_view>(texture, obj, "texture");
    prs::set_json_to_var<uint64_t>(res->sub_width, obj, "sub_width");
    prs::set_json_to_var<uint64_t>(res->sub_height, obj, "sub_height");
    prs::set_json_to_var<bool>(res->spirality, obj, "spirality", false);
    
    return { prs::DeferredData{std::move(texture), DEFERRED_ARGS(*res, texture)} };
}

std::vector<prs::DeferredData> Resource::to_sounds(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::Sound);
    _data = malloc(_size);
    prs::Sound *res = static_cast<prs::Sound*>(_data);

    std::string path;
    bool flags[4];

    prs::set_json_to_var<std::string_view>(_name, obj, "name");
    prs::set_json_to_var<std::string_view>(path, obj, "path");
    prs::set_json_to_var<uint64_t>(res->count, obj, "count", 1);

    prs::set_json_to_var<bool>(flags[0], obj, "looping", false);
    prs::set_json_to_var<bool>(flags[1], obj, "stream", false);
    prs::set_json_to_var<bool>(flags[2], obj, "position", false);
    prs::set_json_to_var<bool>(flags[3], obj, "pitch", false);

    for(int i = 0; i < 4; i++)
    {
        res->flag |= (uint32_t)flags[i] << i;
    }

    return { prs::DeferredData{std::move(path), DEFERRED_ARGS(*res, path)} };
}

std::vector<prs::DeferredData> Resource::to_fonts(simdjson::ondemand::object &obj)
{
    _size = sizeof(prs::Font);
    _data = malloc(_size);
    prs::Font *res = static_cast<prs::Font*>(_data);

    std::string texture, symbols;
    std::vector<std::array<uint32_t, 2>> symbol_widths;

    prs::set_json_to_var<std::string_view>(_name, obj, "name");
    prs::set_json_to_var<std::string_view>(texture, obj, "texture");
    prs::set_json_to_var<std::string_view>(symbols, obj, "symbols");
    prs::set_json_to_array_of_array<uint64_t>(symbol_widths, obj, "symbol_widths");
    prs::set_json_to_var<uint64_t>(res->symbol_height, obj, "symbol_height");

    return {
        prs::DeferredData{std::move(texture), DEFERRED_ARGS(*res, texture)},
        prs::DeferredData{std::move(symbols), DEFERRED_ARGS(*res, symbols)},
        prs::DeferredData{std::move(symbol_widths), DEFERRED_ARGS(*res, symbol_widths)}
    };
}

}