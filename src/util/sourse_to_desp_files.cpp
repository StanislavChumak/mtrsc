#include "util/sourse_to_desp_files.hpp"

#include "scn/Scene.hpp"
#include "res/ResourcePack.hpp"

#include "util/from_json.hpp"
#include "util/build_scripts.hpp"
#include "util/mtrsc_message.hpp"

#include <fstream>

namespace fs = std::filesystem;

namespace mtrs::util
{

bool json_to_scene(simdjson::ondemand::array &json_scene, const std::string &desp_file_name)
{
    mtrs::comp::Scene scene;
    if(!scene.from_json(json_scene, desp_file_name)) return false;

    std::ofstream output_file(desp_file_name, std::ios::binary);
    output_file.seekp(0, std::ios::beg);
    if(!output_file)
    {
        util::mtrsc_error("Failed to create file: ", desp_file_name);
        return false;
    }
    scene.to_file_mtsc(output_file);

    output_file.close();
    return true;
}

bool json_to_pack(simdjson::ondemand::object &json_pack, const std::string &desp_file_name)
{
    mtrs::res::ResourcePack pack;
    if(!pack.from_json(json_pack, desp_file_name)) return false;

    std::ofstream output_file(desp_file_name, std::ios::binary);
    output_file.seekp(0, std::ios::beg);
    if(!output_file)
    {
        util::mtrsc_error("Failed to create file: ", desp_file_name);
        return false;
    }
    pack.to_file_mtrs(output_file);

    output_file.close();
    return true;
}

void json_to_mtrsfile(std::string json_path, const std::string &desp_file_name)
{
    std::unordered_map<std::string, std::string> defines;
    simdjson::padded_string json = mtrs::util::preprocess_json(json_path, defines);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(json);

    simdjson::ondemand::array json_scene;
    simdjson::ondemand::object json_pack;
    if ((!mtrs::util::set_in_var_json<simdjson::ondemand::object>(json_pack, doc["pack"]) ||
        !json_to_pack(json_pack, desp_file_name.substr(0, desp_file_name.length() - 5) + ".mtpck")) &
        (!mtrs::util::set_in_var_json<simdjson::ondemand::array>(json_scene, doc["scene"]) ||
        !json_to_scene(json_scene, desp_file_name.substr(0, desp_file_name.length() - 5) + ".mtscn")))
    {
        util::mtrsc_error("Json file[", json_pack,"] is not in the correct format\n"
            "for a \"scene\" or \"pack\"");
    }
}

void copy_and_transform_directory(const std::filesystem::path &source_dir, const std::filesystem::path &desp_dir)
{
    if (!fs::exists(desp_dir))
    {
        if (!fs::create_directories(desp_dir))
        {
            util::mtrsc_error("Failed to create folder: ", desp_dir);
        }
    }

    std::vector<util::ScriptEntry> scripts;

    for (const auto& entry : fs::recursive_directory_iterator(source_dir))
    {
        const fs::path& src_path = entry.path();
        if(!entry.is_regular_file()) continue;;

        fs::path relative_path = fs::relative(src_path, source_dir);
        fs::path dest_path = desp_dir / relative_path;

        std::string ext = entry.path().extension().string();

        fs::create_directories(dest_path.parent_path());
        if(ext == ".json")
        {
            json_to_mtrsfile(src_path, dest_path);
        }
        else if(ext == ".cpp")
        {
            scripts.push_back({
                src_path.stem().string(),
                src_path,
                dest_path.parent_path()
            });
        }
        else
        {
            fs::copy_file(src_path, dest_path, fs::copy_options::overwrite_existing);
        }
    }

    util::write_generated_cmake(scripts);
    util::build_all_scripts();
}

}