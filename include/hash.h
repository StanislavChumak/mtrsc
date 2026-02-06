#ifndef HASH_H
#define HASH_H

#include <cstdint>
#include <cstddef>

#include <string>

constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ull;

uint64_t fnv1a_64(const void* data, size_t size, uint64_t hash = FNV_OFFSET_BASIS);

uint64_t hash_string(const std::string& s, uint64_t seed = FNV_OFFSET_BASIS);

uint64_t make_scene_entity_id(const std::string& scenePath, const std::string& entityName);

#endif
