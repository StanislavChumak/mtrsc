#ifndef SCENE_HPP
#define SCENE_HPP

#include "Entity.hpp"
#include "util/type/prs/DeferredData.hpp"

#define HEADER_SCENE_SIZE 28

namespace mtrs::comp
{

class Scene
{
    const char _magic[8] = "mtrsscn";
    uint64_t _cache_lifetime = 0;

    uint32_t _entity_count = 0;
    uint32_t _entity_offset = HEADER_SCENE_SIZE;
    uint32_t _ddata_offset = 0;

    std::vector<Entity> _entities;
    std::vector<prs::DeferredData> _deferred_data;

    uint64_t _deferred_data_size;
    uint64_t _entities_size = 0;

    bool _is_init = true;

public:
    Scene() = delete;
    Scene(Scene &) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene(Scene &&other) noexcept;
    Scene &operator=(Scene &&other) noexcept;
    ~Scene() = default;

    Scene(simdjson::ondemand::array &json_scen, uint64_t cache_lifetime, std::string name);

    inline bool is_init() { return _is_init; }
    
    bool to_file_mtsc(std::ofstream &file);
};

}

#endif
