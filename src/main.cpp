#include "utilities/jsonUtils.h"

#include "structures/Scene.h"

int main(int argc, char **argv)
{   
    Scene scene{};
    if(argc < 2 || !scene.from_json(std::string(argv[1]))) return 1;

    return 0;
}