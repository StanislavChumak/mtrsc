#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

#include <fstream>

#define HEADER_SIZE 32
#define ENTITY_INDEX_SIZE 16
#define RELOCATE_BLOCK_SIZE sizeof(uint16_t) * 2 * 32

struct Scene
{
    char magic[4] = {'m','t','s','c'};
    uint16_t version = 101;
    uint16_t flags = 0;

    uint32_t entityCount = 0;
    uint32_t componentTypeCount = 0;

    uint32_t entityIndexOffset = HEADER_SIZE; // size = 32 byte
    uint32_t entityDataOffset;
    uint32_t relocateBlockOffset;
    uint32_t dynamicDateOffset;

    std::vector<Entity> entities;
    std::vector<DynamicDataBuffer> dynamicDataBuffer;

    Scene() = default;
    Scene(Scene &) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene(Scene &&other) noexcept;
    Scene &operator=(Scene &&other) noexcept;
    ~Scene();

    bool from_json(std::string patchJson);
    bool to_file_mtscn(std::ofstream &file);
};


#endif
