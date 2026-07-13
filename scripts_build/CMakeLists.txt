cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
project(mtrs_scripts)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/engine_dir.cmake")
add_subdirectory(${EXTERNAL_INCLUDE_DIR}/glm glm)

function(add_script_module name source dest)
    add_library(${name} MODULE ${source})
    target_include_directories(${name} PRIVATE ${EXTERNAL_INCLUDE_DIR}/mtrsstruct ${EXTERNAL_INCLUDE_DIR}/single-files ${ENGINE_DIR}/include)
    target_link_libraries(${name} PRIVATE glm)
    set_target_properties(${name} PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${dest}
        RUNTIME_OUTPUT_DIRECTORY ${dest}

        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${dest}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${dest}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${dest}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${dest}
    )
endfunction()

include("${CMAKE_CURRENT_SOURCE_DIR}/generated_scripts.cmake" OPTIONAL)