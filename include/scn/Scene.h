#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

#define HEADER_SCENE_SIZE 32
#define ENTITY_INDEX_SIZE 16
#define RELOCATE_BLOCK_SIZE sizeof(uint16_t) * 2 * 32

class Scene
{
    const char magic[4] = {'m','t','s','c'};
    uint16_t version;
    uint16_t flags = 0;

    uint32_t entityCount = 0;
    uint32_t _void = 0;

    uint32_t entityIndexOffset = HEADER_SCENE_SIZE;
    uint32_t entityDataOffset;
    uint32_t relocateBlockOffset;
    uint32_t dynamicDateOffset;

    std::vector<Entity> entities;
    std::vector<DynamicDataBuffer> dynamicDataBuffer;

public:
    Scene(uint16_t version);
    Scene(Scene &) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene(Scene &&other) noexcept;
    Scene &operator=(Scene &&other) noexcept;
    ~Scene() = default;

    bool from_json(simdjson::ondemand::array &jsonScen, std::string name);
    bool to_file_mtsc(std::ofstream &file);
};


#endif
