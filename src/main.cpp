#include "utilities/jsonUtils.h"

#include "structure/Scene.h"

int main(int argc, char **argv)
{   
    std::string path = argv[1];
    if(path.compare(path.length() - 5, 5, ".json") != 0)
    {
        printf("The input is not in json format\n");
        return 1;
    }
    Scene scene{};
    if(argc < 2 || !scene.from_json(path)) return 1;

    std::ofstream file(path.substr(0, path.length() - 4) + "mtsc", std::ios::binary);
    file.seekp(0, std::ios::beg);
    if(!file) return 1;
    scene.to_file_mtscn(file);

    return 0;
}