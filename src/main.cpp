#include "jsonUtils.h"

#include "hash.h"
#include <memory>

int main(int argc, char **argv)
{
    if(!argc) return 1;
    std::string path = argv[1];
    std::unordered_map<std::string, std::string> defines;
    std::shared_ptr<simdjson::padded_string> json = std::make_shared<simdjson::padded_string>(preprocessJSON(path, defines));
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(*json);
    
    std::string sceneName = std::string(getVarJSON<std::string_view>(doc["scene"]));
    printf("|== Scene:      %-18s ==|\n", sceneName.c_str());
    printf("|== ScenePatch: %-18s ==|\n", path.c_str());
    printf("|== SceneID:    0x%016llX ==|\n", make_scene_id(path, sceneName));
    
    int entityId = 0;
    for(simdjson::ondemand::object entity : getVarJSON<simdjson::ondemand::array>(doc["entities"]))
    {
        printf("\n  Entity[%-3d]: ", entityId++);
        for(auto field : entity)
        {
            std::string component = std::string(getResultJSON<std::string_view>(field.unescaped_key()));
            if(component == "Name")
            {
                printf("%s", std::string(getResultJSON<std::string_view>(field.value())).c_str());
                continue;
            }
            printf("\n\tComponent: %-16s", component.c_str());
            simdjson::ondemand::object obj = getVarJSON<simdjson::ondemand::object>(field.value());
            // typeRegistry[std::string(name)].addComponentFromJson(id, obj, world, resources);
        }
    }
    printf("\n");
    return 0;
}