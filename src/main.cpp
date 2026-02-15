#include "struc/scn/Scene.h"
#include "struc/res/ResourcePack.h"

#include "util/jsonUtils.h"

#include <fstream>

#define VERSION 103u

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cerr << "!= No arguments =!" << std::endl;
        // printf("Use the \"--help\" argument for help\n");
        return 0;
    }


    std::string fileName = argv[argc - 1];
    if(fileName.compare(fileName.length() - 5, 5, ".json") != 0)
    {
        std::cerr << "!= The input is not in json format =!" << std::endl;
        return 1;
    }
    std::unordered_map<std::string, std::string> defines;
    simdjson::padded_string json = preprocess_json(fileName, defines);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(json);
    
    fileName = fileName.substr(0, fileName.length() - 5);
    simdjson::ondemand::array jsonScene;
    simdjson::ondemand::object jsonRes;
    if(set_in_var_json<simdjson::ondemand::array>(jsonScene, doc["scene"]))
    {
        Scene scene{VERSION};
        if(!scene.from_json(jsonScene, fileName)) return 1;

        std::ofstream outputFile(fileName + ".mtsc", std::ios::binary);
        outputFile.seekp(0, std::ios::beg);
        if(!outputFile) return 1;
        scene.to_file_mtsc(outputFile);

        outputFile.close();
    }
    else if(set_in_var_json<simdjson::ondemand::object>(jsonRes, doc["resourcePack"]))
    {
        ResourcePack pack{VERSION};
        if(!pack.from_json(jsonRes, fileName)) return 1;

        std::ofstream outputFile(fileName + ".mtrs", std::ios::binary);
        outputFile.seekp(0, std::ios::beg);
        if(!outputFile) return 1;
        pack.to_file_mtrs(outputFile);

        outputFile.close();
    }
    else
    {
        std::cerr << "!= Json file is not in the correct format =!" << std::endl <<
            "!= \"scene\" and \"resources\" were not found in the json =!" << std::endl;
    }

    return 0;
}