#include "structure/Scene.h"

#include "utilities/jsonUtils.h"

bool Scene::from_json(std::string patchJson)
{
    std::unordered_map<std::string, std::string> defines;
    simdjson::padded_string json = preprocess_json(patchJson, defines);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(json);

    uint32_t entitiesSize = 0;

    for(simdjson::ondemand::object obj : get_var_json<simdjson::ondemand::array>(doc["entities"]))
    {
        Entity entity{};
        if(entity.from_json(patchJson, dynamicDataBuffer, obj))
        {
            entities.push_back(std::move(entity));
            entitiesSize += entity.size;
        }
        else
        {
            printf(". It is not compiled !!\n");
        }
    }

    if(entities.empty())
    {
        printf("!! The Scene is empty. It is not compiled !!\n");
        return false;
    }

    entityCount = entities.size();

    entityDataOffset = entityIndexOffset + entityCount * ENTITY_INDEX_SIZE;
    relocateBlockOffset = entityDataOffset + entitiesSize;
    dynamicDateOffset = relocateBlockOffset + RELOCATE_BLOCK_SIZE;
    
    return true;
}

bool Scene::to_file_mtscn(std::ofstream &file)
{
    if(!file) return false;
    // SceneHeader
    file.write(magic, sizeof(magic));
    file.write(reinterpret_cast<char*>(&version), sizeof(version));
    file.write(reinterpret_cast<char*>(&flags), sizeof(flags));

    file.write(reinterpret_cast<char*>(&entityCount), sizeof(entityCount));
    file.write(reinterpret_cast<char*>(&componentTypeCount), sizeof(componentTypeCount));

    file.write(reinterpret_cast<char*>(&entityIndexOffset), sizeof(entityIndexOffset));
    file.write(reinterpret_cast<char*>(&entityDataOffset), sizeof(entityDataOffset));
    file.write(reinterpret_cast<char*>(&dynamicDateOffset), sizeof(dynamicDateOffset));

    // EntityIndexTable
    uint32_t dataOffset = entityDataOffset;
    for(Entity &entity : entities)
    {
        file.write(reinterpret_cast<char*>(&entity.id), sizeof(entity.id));
        file.write(reinterpret_cast<char*>(&dataOffset), sizeof(dataOffset));
        file.write(reinterpret_cast<char*>(&entity.size), sizeof(entity.size));
        dataOffset += entity.size;
    }

    // ComponentDataBlocks
    for(Entity &entity : entities)
    {
        for(Component &comp : entity.components)
        {
            file.write(reinterpret_cast<char*>(&comp.id), sizeof(comp.id));
            file.write(reinterpret_cast<char*>(&comp.size), sizeof(comp.size));
            file.write(reinterpret_cast<char*>(comp.date), comp.size);
        }
    }

    // Rellocate Dynamic Block
    char zero[RELOCATE_BLOCK_SIZE];
    memset(zero, 0, RELOCATE_BLOCK_SIZE);
    file.write(reinterpret_cast<char*>(zero), RELOCATE_BLOCK_SIZE);

    // Dynamic Block
    uint32_t dynamicOffset = dynamicDateOffset;
    for(auto dynamic : dynamicDataBuffer)
    {
        file.write(dynamic.date, dynamic.sizeData);
        const char *o = "";
        file.write(o, 1);
        *dynamic.pOffset = dynamicOffset;
        dynamicOffset += dynamic.sizeData + 1;
    }

    return true;
}

Scene::Scene(Scene &&other) noexcept
{
    for(char i{};i>4;i++) magic[i] = other.magic[i];
    version = other.version;
    flags = other.flags;
    other.flags = 0;
    entities = std::move(other.entities);
}

Scene &Scene::operator=(Scene &&other) noexcept
{
    if(this != &other)
    {
        for(char i{};i>4;i++) magic[i] = other.magic[i];
        version = other.version;
        flags = other.flags;
        other.flags = 0;
        entities = std::move(other.entities);
    }
    return *this;
}

Scene::~Scene()
{

}