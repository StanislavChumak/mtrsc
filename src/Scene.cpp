#include "Scene.h"

#include "jsonUtils.h"

#include <memory>

bool Scene::from_json(std::string patchJson)
{
    std::unordered_map<std::string, std::string> defines;
    simdjson::padded_string json = preprocessJSON(patchJson, defines);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(json);

    for(simdjson::ondemand::object obj : getVarJSON<simdjson::ondemand::array>(doc["entities"]))
    {
        Entity entity{};
        if(entity.from_json(patchJson, obj))
        {
            entityCount++;
            entities.push_back(std::move(entity));
        }
        else
        {
            printf(". It is not compiled !!\n");
        }
    }
    if(entityCount == 0)
    {
        printf("!! The Scene is empty. It is not compiled !!\n");
        return false;
    }
    
    return true;
}

Scene::Scene(Scene &&other) noexcept
{
    magic = other.magic;
    version = other.version;
    flags = other.flags;
    other.flags = 0;
    entityCount = other.entityCount;
    other.entityCount = 0;
    entities = std::move(other.entities);
}

Scene &Scene::operator=(Scene &&other) noexcept
{
    if(this != &other)
    {
        magic = other.magic;
        version = other.version;
        flags = other.flags;
        other.flags = 0;
        entityCount = other.entityCount;
        other.entityCount = 0;
        entities = std::move(other.entities);
    }
    return *this;
}

Scene::~Scene()
{

}