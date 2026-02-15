#include "struc/scn/Scene.h"

#include "util/jsonUtils.h"

#include <fstream>

bool Scene::from_json(simdjson::ondemand::array &jsonScene, std::string name)
{
    uint32_t entitiesSize = 0;

    for(simdjson::ondemand::object obj : jsonScene)
    {
        Entity entity{};
        if(!entity.from_json(obj, name, dynamicDataBuffer))
            continue;
        entitiesSize += entity.get_size();
        entities.push_back(std::move(entity));
    }

    if(entities.empty())
    {
        std::cerr << "!= The Scene is empty. It is not compiled =!" << std::endl;
        return false;
    }

    entityCount = entities.size();
    entityDataOffset = entityIndexOffset + entityCount * ENTITY_INDEX_SIZE;
    relocateBlockOffset = entityDataOffset + entitiesSize;
    dynamicDateOffset = relocateBlockOffset + RELOCATE_BLOCK_SIZE;
    
    return true;
}

bool Scene::to_file_mtsc(std::ofstream &file)
{
    if(!file) return false;
    // Header
    file.write(magic, sizeof(magic));
    file.write(reinterpret_cast<char*>(&version), sizeof(version));
    file.write(reinterpret_cast<char*>(&flags), sizeof(flags));

    file.write(reinterpret_cast<char*>(&entityCount), sizeof(entityCount));
    file.write(reinterpret_cast<char*>(&_void), sizeof(_void));

    file.write(reinterpret_cast<char*>(&entityIndexOffset), sizeof(entityIndexOffset));
    file.write(reinterpret_cast<char*>(&entityDataOffset), sizeof(entityDataOffset));
    file.write(reinterpret_cast<char*>(&relocateBlockOffset), sizeof(relocateBlockOffset));
    file.write(reinterpret_cast<char*>(&dynamicDateOffset), sizeof(dynamicDateOffset));

    // Entity
    uint32_t dataOffset = entityDataOffset;
    for(Entity &entity : entities)
        entity.to_file_mtscn(file, dataOffset);

    //Rellocate Dynamic Block
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

Scene::Scene(uint16_t version)
: version(version)
{}

Scene::Scene(Scene &&other) noexcept
{
    version = other.version;
    flags = other.flags;
    other.flags = 0;
    entities = std::move(other.entities);
}

Scene &Scene::operator=(Scene &&other) noexcept
{
    if(this != &other)
    {
        version = other.version;
        flags = other.flags;
        other.flags = 0;
        entities = std::move(other.entities);
    }
    return *this;
}