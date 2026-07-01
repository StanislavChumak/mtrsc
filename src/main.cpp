#include "scn/Scene.hpp"
#include "res/ResourcePack.hpp"

#include "util/from_json.hpp"
#include "util/mtrsc_message.hpp"

#include <filesystem>
#include <fstream>

#define VERSION 1.6f

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        MTRS_ERROR("No arguments");
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
                mtrs::util::flag_message(mtrs::util::FlagMessage::PRINT_LOG);
            }
            else if(arg == "-det-log")
            {
                mtrs::util::flag_message(mtrs::util::FlagMessage::PRINT_LOG);
                mtrs::util::flag_message(mtrs::util::FlagMessage::DETAIL_LOG);
            }
        }
    }

    std::string file_name = argv[argc - 2];
    std::string to_path = argv[argc - 1];
    if(file_name.compare(file_name.length() - 5, 5, ".json") != 0)
    {
        MTRS_ERROR("The input is not in json format");
        return 1;
    }
    
    if (!fs::exists(to_path) && !fs::is_directory(to_path))
    {
        if (!fs::create_directories(to_path))
        {
            MTRS_ERROR("Failed to create folder: ", to_path);
        }
    }

    std::unordered_map<std::string, std::string> defines;
    simdjson::padded_string json = mtrs::util::preprocess_json(file_name, defines);
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
    if(mtrs::util::set_in_var_json<simdjson::ondemand::array>(json_scene, doc["scene"]))
    {
        mtrs::comp::Scene scene{VERSION};
        if(!scene.from_json(json_scene, file_name)) return 1;

        std::ofstream output_file(to_path + file_name + ".mtsc", std::ios::binary);
        output_file.seekp(0, std::ios::beg);
        if(!output_file)
        {
            MTRS_ERROR("Failed to create file: ", to_path, file_name, ".mtsc");
            return 1;
        }
        scene.to_file_mtsc(output_file);

        output_file.close();
    }
    else if(mtrs::util::set_in_var_json<simdjson::ondemand::object>(json_pack, doc["pack"]))
    {
        mtrs::res::ResourcePack pack{VERSION};
        if(!pack.from_json(json_pack, file_name)) return 1;

        std::ofstream output_file(to_path + file_name + ".mtrs", std::ios::binary);
        output_file.seekp(0, std::ios::beg);
        if(!output_file)
        {
            MTRS_ERROR("Failed to create file: ", to_path, file_name, ".mtrs");
            return 1;
        }
        pack.to_file_mtrs(output_file);

        output_file.close();
    }
    else
    {
        MTRS_ERROR("Json file is not in the correct format\n",
            "\"scene\" and \"pack\" were not found in the json");
    }

    return 0;
}