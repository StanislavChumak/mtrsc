#include "scn/Scene.hpp"
#include "res/ResourcePack.hpp"

#include "util/from_json.hpp"
#include "util/LogSystem.hpp"

#include <filesystem>
#include <fstream>

#define VERSION 1.5f

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        LogSystem::print_err("No arguments");
        return 0;
    }

    if(argc > 3)
    {
        std::string arg;
        LogSystemConfig log_config;

        for(int i = 1; i < (argc - 1); i++)
        {
            arg = argv[i];
            
            if(arg == "-log")
            {
                log_config.print_log = true;
            }
            else if(arg == "-long-log")
            {
                log_config.print_long_log = true;
            }
        }

        LogSystem::set_config(std::move(log_config));
    }

    std::string file_name = argv[argc - 2];
    std::string to_path = argv[argc - 1];
    if(file_name.compare(file_name.length() - 5, 5, ".json") != 0)
    {
        LogSystem::print_err("The input is not in json format");
        return 1;
    }
    
    if (!fs::exists(to_path) && !fs::is_directory(to_path))
    {
        if (!fs::create_directories(to_path))
        {
            LogSystem::print_err("Failed to create folder: " + to_path);
        }
    }

    std::unordered_map<std::string, std::string> defines;
    simdjson::padded_string json = preprocess_json(file_name, defines);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(json);
    
    std::size_t pos = file_name.find_last_of("\\/");
    if(pos == std::string::npos)
    {
        file_name = '/' + file_name;
        pos = 0;
    }
    file_name = file_name.substr(pos, file_name.length() - 5 - pos);
    simdjson::ondemand::array json_scene;
    simdjson::ondemand::object json_pack;
    if(set_in_var_json<simdjson::ondemand::array>(json_scene, doc["scene"]))
    {
        Scene scene{VERSION};
        if(!scene.from_json(json_scene, file_name)) return 1;

        std::ofstream output_file(to_path + file_name + ".mtsc", std::ios::binary);
        output_file.seekp(0, std::ios::beg);
        if(!output_file)
        {
            LogSystem::print_err("Failed to create file: " + to_path + file_name + ".mtsc");
            return 1;
        }
        scene.to_file_mtsc(output_file);

        output_file.close();
    }
    else if(set_in_var_json<simdjson::ondemand::object>(json_pack, doc["pack"]))
    {
        ResourcePack pack{VERSION};
        if(!pack.from_json(json_pack, file_name)) return 1;

        std::ofstream output_file(to_path + file_name + ".mtrs", std::ios::binary);
        output_file.seekp(0, std::ios::beg);
        if(!output_file)
        {
            LogSystem::print_err("Failed to create file: " + to_path + file_name + ".mtrs");
            return 1;
        }
        pack.to_file_mtrs(output_file);

        output_file.close();
    }
    else
    {
        LogSystem::print_err("Json file is not in the correct format\n"
            "\"scene\" and \"pack\" were not found in the json ");
    }

    return 0;
}