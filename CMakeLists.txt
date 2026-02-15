cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(PROJECT_NAME mtrsc)
project(${PROJECT_NAME} VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_executable(${PROJECT_NAME}
    src/main.cpp

    src/util/jsonUtils.cpp
    src/util/hash.cpp
    src/util/string_optimizer.cpp
    src/util/to_dynamic_data.cpp

    src/struc/scn/Entity.cpp
    src/struc/scn/Component.cpp
    src/struc/scn/Scene.cpp

    src/struc/res/ResourcePack.cpp
    src/struc/res/ResourceType.cpp
    src/struc/res/Resource.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(external/simdjson)
target_link_libraries(${PROJECT_NAME} PUBLIC simdjson)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()
