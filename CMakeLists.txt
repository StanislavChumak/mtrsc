cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(MtrsCompiler)
set(EXE_NAME "mtrsc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set_target_properties(${EXE_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()
    
add_subdirectory(external/simdjson)
add_subdirectory(external/utils)

add_executable(${EXE_NAME}
    src/main.cpp

    src/scn/Scene.cpp
    src/scn/Entity.cpp
    src/scn/Component.cpp
    src/scn/to_comp.cpp

    src/res/ResourcePack.cpp
    src/res/ResourceGroup.cpp
    src/res/Resource.cpp
    src/res/to_res.cpp
)

target_include_directories(${EXE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${EXE_NAME} PUBLIC simdjson)
target_link_libraries(${EXE_NAME} PUBLIC mtrsutils)

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts_build"
        "$<TARGET_FILE_DIR:${EXE_NAME}>/scripts_build"
)
