#ifndef RESOURCE_H
#define RESOURCE_H

#include "simdjson.h"

#include "util/DynamicDataBuffer.h"

#define RESOURCE_TYPE \
X(shaders)\
X(textures)\
X(atlases)\
X(sounds)\
X(musics)

class Resource
{
    uint32_t size = 0;
    void *date = nullptr;

public:
    Resource() = default;
    Resource(Resource &) = delete;
    Resource &operator=(const Resource &) = delete;
    Resource(Resource &&other) noexcept;
    Resource &operator=(Resource &&other) noexcept;
    ~Resource();
    
    uint32_t get_size() { return size; }

    bool from_json(
        simdjson::ondemand::object &obj,
        std::string &resName,
        std::vector<DynamicDataBuffer> &bufferDynamicDate);
    bool to_file_mtscn(std::ofstream &file);

#define X(res) void to_##res(simdjson::ondemand::object &obj, std::vector<DynamicDataBuffer> &bufferDynamicDate);
    RESOURCE_TYPE
#undef X
};

#endif
