#ifndef SCENE_HPP
#define SCENE_HPP

#include "Entity.hpp"

#define HEADER_SCENE_SIZE 24
#define FREE_FOR_DYNAMIC_DATE_SIZE sizeof(uint16_t) * 2 * 32

class Scene
{
    const char _magic[4] = {'m','t','s','c'};
    float _version;

    uint32_t _entity_count = 0;

    uint32_t _entity_offset = HEADER_SCENE_SIZE;
    uint32_t _free_for_dynamic_date_offset = 0;
    uint32_t _dynamic_date_offset = 0;

    std::vector<Entity> _entities;
    std::vector<DynamicBuffer> _dynamic_buffers;

    uint64_t _dynamic_date_size;
    uint64_t _entities_size = 0;

public:
    Scene(float version);
    Scene(Scene &) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene(Scene &&other) noexcept;
    Scene &operator=(Scene &&other) noexcept;
    ~Scene() = default;

    bool from_json(simdjson::ondemand::array &json_scen, std::string name);
    bool to_file_mtsc(std::ofstream &file);
};


#endif
