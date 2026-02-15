#ifndef RESOURCES_PACK_H
#define RESOURCES_PACK_H

#include "ResourceType.h"

#define HEADER_PACK_SIZE 16

class ResourcePack
{
    char magic[4] = {'m','t','r','s'};
    uint16_t version;
    uint16_t flags = 0;

    uint32_t resourceDataOffset = HEADER_PACK_SIZE;
    uint32_t dynamicDateOffset;

    std::vector<ResourceType> resourceTypes;
    std::vector<DynamicDataBuffer> dynamicDataBuffer;

public:
    ResourcePack(uint16_t version);
    ResourcePack(ResourcePack &) = delete;
    ResourcePack &operator=(const ResourcePack &) = delete;
    ResourcePack(ResourcePack &&other) noexcept;
    ResourcePack &operator=(ResourcePack &&other) noexcept;
    ~ResourcePack() = default;

    bool from_json(simdjson::ondemand::object &jsonRes, std::string name);
    bool to_file_mtrs(std::ofstream &file);
};


#endif
