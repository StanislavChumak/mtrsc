#include "utilities/jsonUtils.h"

#include "structures/Scene.h"

#include <fstream>

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

    std::ofstream file("scene.mtscn", std::ios::binary);

    return 0;
}