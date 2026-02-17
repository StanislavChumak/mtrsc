#ifndef RESOURCE_TYPE_H
#define RESOURCE_TYPE_H

#include "Resource.h"

class ResourceType
{
    uint32_t sizeBlockResources = 0;
    uint32_t sizeName;
    std::string name;
    std::vector<Resource> resources;

public:
    ResourceType() = default;
    ResourceType(ResourceType &) = delete;
    ResourceType &operator=(const ResourceType &) = delete;
    ResourceType(ResourceType &&other) noexcept;
    ResourceType &operator=(ResourceType &&other) noexcept;
    ~ResourceType() = default;

    uint32_t get_cout() { return resources.size(); }

    bool from_json(
        simdjson::ondemand::array &array,
        std::string &resName,
        std::vector<DynamicDataBuffer> &dynamicDataBuffer);
    bool to_file_mtscn(std::ofstream &file);
};

#endif
