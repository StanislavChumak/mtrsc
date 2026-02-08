cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(PROJECT_NAME mtrsc)
project(${PROJECT_NAME} VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_executable(${PROJECT_NAME}
src/main.cpp

src/utilities/jsonUtils.cpp
src/utilities/hash.cpp

src/structure/Entity.cpp
src/structure/Component.cpp
src/structure/Scene.cpp
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
