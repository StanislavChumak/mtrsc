cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(mtrsc VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp

    src/util/jsonUtils.cpp
    src/util/hash.cpp
    src/util/string_optimizer.cpp
    src/util/to_dynamic_data.cpp

    src/scn/Scene.cpp
    src/scn/Entity.cpp
    src/scn/Component.cpp

    src/res/ResourcePack.cpp
    src/res/ResourceType.cpp
    src/res/Resource.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/external/struct
)

add_subdirectory(external/simdjson)
target_link_libraries(${PROJECT_NAME} PUBLIC simdjson)
