#ifndef HASH_H
#define HASH_H

#include <cstdint>
#include <cstddef>

#include <string>

constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ull;

constexpr uint64_t FNV_PRIME        = 1099511628211ull;

constexpr uint64_t fnv1a_64(const char* data, size_t size, uint64_t hash = FNV_OFFSET_BASIS)
{
    for (size_t i = 0; i < size; ++i)
    {
        hash ^= data[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

constexpr uint64_t hash_c_string(const char *s, uint64_t seed = FNV_OFFSET_BASIS)
{
    int i = 0;
    while (s[i]) i++;
    return fnv1a_64(s, i, seed);
}

uint64_t hash_string(const std::string& s, uint64_t seed = FNV_OFFSET_BASIS);

uint64_t make_scene_entity_id(const std::string& scenePath, const std::string& entityName);

#endif
