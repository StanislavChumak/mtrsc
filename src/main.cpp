#include "scn/Scene.hpp"
#include "res/ResourcePack.hpp"
#include "util/fun/prs/json.hpp"
#include "util/fun/str/preprocess_json.hpp"
#include "util/fun/msg/mtrs_message.hpp"

#include <filesystem>
#include <fstream>

bool json_to_scene(simdjson::ondemand::array &json_scene, const std::string &desp_file_name)
{
    mtrs::comp::Scene scene{json_scene, desp_file_name};
    if(!scene.is_init()) return false;

    std::ofstream output_file(desp_file_name, std::ios::binary);
    output_file.seekp(0, std::ios::beg);
    if(!output_file)
    {
        mtrs::msg::mtrs_error("Failed to create file: ", desp_file_name);
        return false;
    }
    scene.to_file_mtsc(output_file);

    output_file.close();
    return true;
}

bool json_to_pack(simdjson::ondemand::object &json_pack, const std::string &desp_file_name)
{
    mtrs::res::ResourcePack pack{json_pack, desp_file_name};
    if(!pack.is_init()) return false;

    std::ofstream output_file(desp_file_name, std::ios::binary);
    output_file.seekp(0, std::ios::beg);
    if(!output_file)
    {
        mtrs::msg::mtrs_error("Failed to create file: ", desp_file_name);
        return false;
    }
    pack.to_file_mtrs(output_file);

    output_file.close();
    return true;
}

void json_to_mtrsfile(std::string json_path, const std::string &desp_file_name)
{
    std::unordered_map<std::string, std::string> defines;
    std::string str = mtrs::str::preprocess_json(json_path, defines);
    simdjson::padded_string json{str.data(), str.length()};
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(json);
    
    auto json_pack = doc["pack"].get_object();
    if(!json_pack.error())
    {
        json_to_pack(json_pack.value(), desp_file_name.substr(0, desp_file_name.size() - 5) + ".mtpck");
    }
    auto json_scene = doc["scene"].get_array();
    if(!json_scene.error())
    {
        json_to_scene(json_scene.value(), desp_file_name.substr(0, desp_file_name.size() - 5) + ".mtscn");
    }
}

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        mtrs::msg::mtrs_error("No arguments");
        return 0;
    }

    if(argc > 3)
    {
        std::string arg;

        for(int i = 1; i < (argc - 1); i++)
        {
            arg = argv[i];
            
            if(arg == "-log")
            {
                mtrs::msg::flag_message(mtrs::msg::FlagMessage::PRINT_INFO);
            }
            else if(arg == "-det-log")
            {
                mtrs::msg::flag_message(mtrs::msg::FlagMessage::PRINT_INFO);
                mtrs::msg::flag_message(mtrs::msg::FlagMessage::DETAIL_INFO);
            }
        }
    }

    std::string source_dir = argv[argc - 2];
    std::string desp_dir = argv[argc - 1];

    if (!fs::exists(desp_dir))
    {
        if (!fs::create_directories(desp_dir))
        {
            mtrs::msg::mtrs_error("Failed to create folder: ", desp_dir);
        }
    }

    struct ScriptEntry
    {
        std::string target_name;
        fs::path source_path;
        fs::path dest_dir;
    };
    std::vector<ScriptEntry> scripts;

    for(const auto& entry : fs::recursive_directory_iterator(source_dir))
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

    std::ofstream out("scripts_build/generated_scripts.cmake");

    out << "# AUTO-GENERATED - no manual editing required\n";
    for (const auto& s : scripts)
    {
        out << "add_script_module(" << s.target_name
            << " \"" << s.source_path.generic_string() << "\""
            << " \"" << s.dest_dir.generic_string() << "\")\n";
    }
    out.close();

    fs::path build_output = "scripts_build/_cmake_build";

    std::ostringstream configure_cmd;
    configure_cmd << "cmake -S \"" << fs::current_path().string() << "/scripts_build\""
                  << " -B \"" << build_output.string() << "\""
#if !defined(_WIN32)
                  << " -DCMAKE_BUILD_TYPE=Release"
#endif
                  ;

    if (std::system(configure_cmd.str().c_str()) != 0)
    {
        mtrs::msg::mtrs_error("CMake configure failed for scripts");
        return 1;
    }

    std::ostringstream build_cmd;
    build_cmd << "cmake --build \"" << build_output.string() << "\""
              << " --config Release"
              << " --parallel";

    if (std::system(build_cmd.str().c_str()) != 0)
    {
        mtrs::msg::mtrs_error("CMake build failed for scripts");
        return 1;
    }

    return 0;
}

