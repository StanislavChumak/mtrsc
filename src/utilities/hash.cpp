#include "utilities/hash.h"

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