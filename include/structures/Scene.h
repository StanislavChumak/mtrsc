#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

struct Scene
{
    uint64_t magic = 0x4D5453434E000000;
    uint16_t version = 1;
    uint16_t flags = 0;
    uint32_t entityCount = 0;
    std::vector<Entity> entities;

    bool from_json(std::string patchJson);
    Scene() = default;
    Scene(Scene &) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene(Scene &&other) noexcept;
    Scene &operator=(Scene &&other) noexcept;
    ~Scene();
};


#endif
