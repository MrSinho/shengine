cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(ShEngine C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC ON)

#SUBDIRECTORIES
add_subdirectory(externals/ShVulkan)
add_subdirectory(externals/glfw)
add_subdirectory(externals/cglm)

#plyimporter
option(SH_ENGINE_BUILD_PLY_IMPORTER OFF)
if(SH_ENGINE_BUILD_PLY_IMPORTER)
add_subdirectory(externals/PlyImporter)
endif(SH_ENGINE_BUILD_PLY_IMPORTER)

#shengine
set(SH_ENGINE_SRC_DIR ShEngine/shengine/src)
add_library(shengine STATIC 
    ${SH_ENGINE_SRC_DIR}/engine/shInput.c
    ${SH_ENGINE_SRC_DIR}/engine/shTime.c
    ${SH_ENGINE_SRC_DIR}/engine/shWindow.c
    ${SH_ENGINE_SRC_DIR}/linear_algebra/shProjection.c
    ${SH_ENGINE_SRC_DIR}/linear_algebra/shView.c
    ${SH_ENGINE_SRC_DIR}/scene/shScene.c
    ${SH_ENGINE_SRC_DIR}/components/shImage.c
)
target_include_directories(shengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/ShEngine/shengine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ShEngine/ecs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/ShECS/include
)
target_link_libraries(shengine PUBLIC
    glfw
    shvulkan
    cglm
)
set_target_properties(shengine glfw shvulkan cglm PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

#shfd
option(SH_ENGINE_USE_SHFD OFF)

if (SH_ENGINE_BUILD_EDITOR)
set(SH_ENGINE_BUILD_SHFD ON)
endif(SH_ENGINE_BUILD_EDITOR)

if (SH_ENGINE_BUILD_SHFD)

if(NOT SH_ENGINE_BUILD_PLY_IMPORTER)
set(SH_ENGINE_BUILD_PLY_IMPORTER ON)
add_subdirectory(externals/PlyImporter)
endif(SH_ENGINE_BUILD_PLY_IMPORTER)

set(SH_FD_SRC ShEngine/shfd/src)
add_library(shfd STATIC 
    ${SH_FD_SRC}/shFile.c
    ${SH_FD_SRC}/shfd.c
)
target_include_directories(shfd PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/externals/ShECS/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ShEngine/ecs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ShEngine/shfd/include
)
add_subdirectory(externals/json-c)
target_link_libraries(shfd PUBLIC json-c shvulkan PlyImporter)
set_target_properties(shfd PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(SH_ENGINE_BUILD_SHFD)

#editor
option(SH_ENGINE_BUILD_EDITOR OFF)
if(SH_ENGINE_BUILD_EDITOR)
add_executable(sheditor editor/src/editor.c)
target_link_libraries(sheditor PUBLIC 
    shengine
    shfd
)
target_include_directories(sheditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/editor/include
)
set_target_properties(sheditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endif(SH_ENGINE_BUILD_EDITOR)