#include "hash.h"

constexpr uint64_t FNV_PRIME        = 1099511628211ull;

uint64_t fnv1a_64(const void* data, size_t size, uint64_t hash)
{
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    for (size_t i = 0; i < size; ++i)
    {
        hash ^= bytes[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

uint64_t hash_string(const std::string& s, uint64_t seed)
{
    return fnv1a_64(s.data(), s.size(), seed);
}

uint64_t make_scene_entity_id(
    const std::string& scenePath,
    const std::string& entityName,
    uint32_t entityIndex
)
{
    uint64_t hash = FNV_OFFSET_BASIS;

    hash = fnv1a_64(scenePath.data(), scenePath.size(), hash);
    hash = fnv1a_64("::", 2, hash);
    hash = fnv1a_64(entityName.data(), entityName.size(), hash);
    hash = fnv1a_64("::", 2, hash);
    hash = fnv1a_64(&entityIndex, sizeof(entityIndex), hash);

    return hash;
}

uint64_t make_scene_id(
    const std::string& scenePath,
    const std::string& sceneName
)
{
    uint64_t hash = FNV_OFFSET_BASIS;

    hash = fnv1a_64(scenePath.data(), scenePath.size(), hash);
    hash = fnv1a_64("::", 2, hash);
    hash = fnv1a_64(sceneName.data(), sceneName.size(), hash);

    return hash;
}