cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(SH-Engine C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC ON)

#SUBDIRECTORIES
add_subdirectory(externals/ShVulkan)
add_subdirectory(externals/glfw)
add_subdirectory(externals/cglm)
add_subdirectory(externals/PlyImporter)

#ShEngine
set(SH_ENGINE_SRC_DIR SH-Engine/ShEngine/src)
add_library(ShEngine STATIC 
    ${SH_ENGINE_SRC_DIR}/engine/shInput.c
    ${SH_ENGINE_SRC_DIR}/engine/shTime.c
    ${SH_ENGINE_SRC_DIR}/engine/shWindow.c
    ${SH_ENGINE_SRC_DIR}/linear_algebra/shProjection.c
    ${SH_ENGINE_SRC_DIR}/linear_algebra/shView.c
    ${SH_ENGINE_SRC_DIR}/scene/shScene.c
    ${SH_ENGINE_SRC_DIR}/components/shImage.c
)
target_include_directories(ShEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/SH-Engine/ShEngine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/SH-Engine/ecs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/SH-Engine/externals/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/SH-Engine/externals/ShECS/ShECS/include
)
target_link_libraries(ShEngine PUBLIC
    glfw
    ShVulkan
    cglm
    shfd
)

#shfd
option(SH_ENGINE_USE_SHFD OFF)
if (SH_ENGINE_USE_SHFD)
set(SH_FD_SRC SH-Engine/shfd/src)
add_library(shfd STATIC 
    ${SH_FD_SRC}/shFile.c
    ${SH_FD_SRC}/shfd.c
)
target_include_directories(shfd PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/SH-Engine/shfd/include
    ${CMAKE_CURRENT_SOURCE_DIR}/SH-Engine/ecs/include
)
add_subdirectory(externals/json-c)
target_link_libraries(shfd PUBLIC json-c ShVulkan PlyImporter)
set_target_properties(shfd PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

#EDITOR
add_executable(Editor editor/src/editor.c)
target_link_libraries(Editor PUBLIC 
    ShEngine
    shfd
)
target_include_directories(Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/editor/include
)

#OUTPUT
set_target_properties(Editor ShEngine glfw cglm PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)