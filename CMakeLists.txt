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

    src/util/from_json.cpp
    src/util/string_optimizer.cpp
    src/util/to_dynamic_data.cpp
    src/util/mtrsc_message.cpp
    src/util/sourse_to_desp_files.cpp
    src/util/build_scripts.cpp

    src/scn/Scene.cpp
    src/scn/Entity.cpp
    src/scn/Component.cpp

    src/res/ResourcePack.cpp
    src/res/ResourceType.cpp
    src/res/Resource.cpp

)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/mtrsstruct
)

add_subdirectory(external/simdjson)
target_link_libraries(${PROJECT_NAME} PUBLIC simdjson)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts_build"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts_build"
)
