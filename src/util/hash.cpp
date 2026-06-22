#include "util/hash.hpp"

uint64_t hash_string(const std::string& str, uint64_t seed)
{
    return fnv1a_64(str.data(), str.size(), seed);
}