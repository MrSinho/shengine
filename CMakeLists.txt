cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(SH-Engine C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC ON)

#SUBDIRECTORIES
add_subdirectory(Externals/ShVulkan)
add_subdirectory(Externals/glfw)
add_subdirectory(Externals/cglm)
add_subdirectory(Externals/json-c)
add_subdirectory(Externals/PlyImporter)

#SH-Engine
set(SH_ENGINE_SRC_DIR SH-Engine/src)
add_library(SH-Engine STATIC 
    ${SH_ENGINE_SRC_DIR}/engine/shInput.c
    ${SH_ENGINE_SRC_DIR}/engine/shTime.c
    ${SH_ENGINE_SRC_DIR}/engine/shWindow.c
    ${SH_ENGINE_SRC_DIR}/linear_algebra/shProjection.c
    ${SH_ENGINE_SRC_DIR}/linear_algebra/shView.c
    ${SH_ENGINE_SRC_DIR}/scene/shScene.c
    ${SH_ENGINE_SRC_DIR}/components/shMaterial.c
    ${SH_ENGINE_SRC_DIR}/components/shImage.c
)
target_include_directories(SH-Engine PUBLIC 
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/ShECS/ShECS/include
    ${CMAKE_CURRENT_SOURCE_DIR}/SH-Engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(SH-Engine PUBLIC
    glfw
    ShVulkan
    cglm
    PlyImporter
    ShFd
)

add_library(ShFd STATIC 
    ShFd/src/shFile.c
    ShFd/src/shFd.c
)
target_include_directories(ShFd PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/ShFd/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ShFd PUBLIC json-c ShVulkan PlyImporter)

#EDITOR
file(GLOB_RECURSE EDITOR_SRC editor/src/*.c)
add_executable(Editor ${EDITOR_SRC})
target_link_libraries(Editor PUBLIC 
    SH-Engine
    ShFd
)
target_include_directories(Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/editor/include
)

#OUTPUT
set_target_properties(SH-Engine glfw cglm PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)