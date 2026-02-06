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

uint64_t make_scene_entity_id(const std::string& scenePath, const std::string& entityName)
{
    uint64_t hash = FNV_OFFSET_BASIS;

    hash = hash_string(scenePath, hash);
    hash = hash_string("::", hash);
    hash = hash_string(entityName, hash);

    return hash;
}