cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(PROJECT_NAME Foggy-Engine)
project(${PROJECT_NAME} C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC ON)

#SUBDIRECTORIES
add_subdirectory(Externals/glfw)
add_subdirectory(Externals/cglm)
add_subdirectory(Externals/PlyImporter)

#INSTALLED PACKAGES
find_package(Vulkan REQUIRED FATAL_ERROR)

option(FGG_USE_VULKAN_1_0 OFF)
if(FGG_USE_VULKAN_1_0)
    add_definitions(-DFGG_USE_VULKAN_1_0)
endif(FGG_USE_VULKAN_1_0)

option(FGG_USE_VULKAN_1_1 OFF)
if(FGG_USE_VULKAN_1_1)
    add_definitions(-DFGG_USE_VULKAN_1_1)
endif(FGG_USE_VULKAN_1_1)

#UTILITIES
file(GLOB_RECURSE FGG_UTILITIES_SRC FggUtilities/src/*.c)
add_library(FggUtilities STATIC ${FGG_UTILITIES_SRC})
target_include_directories(FggUtilities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/FggUtilities/include
    ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(FggUtilities PUBLIC 
    ${VULKAN_LIBRARIES}
)

#ENGINE
file(GLOB_RECURSE ENGINE_SRC Foggy-Engine/src/*.c)
add_library(Foggy-Engine STATIC ${ENGINE_SRC})
target_include_directories(Foggy-Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Foggy-Engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/FggECS/FggECS/include
    ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(Foggy-Engine PUBLIC
    glfw
    ${Vulkan_LIBRARIES}      
    FggUtilities
    cglm
    PlyImporter
)

#SHADERS
message(STATUS "looking for glslc")
find_program(GLSLC_FOUND glslc)
if (NOT GLSLC_FOUND)
    message(STATUS "glslc not found, required for shader compilation, install before executing the engine")
else()
    message(STATUS "found glslc")
endif()


#EDITOR
file(GLOB_RECURSE EDITOR_SRC Editor/src/*.c)
add_executable(Editor ${EDITOR_SRC})
target_link_libraries(Editor PUBLIC 
    Foggy-Engine
    FggUtilities
)
target_include_directories(Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor/include
)

#OUTPUT
set_target_properties(Editor Foggy-Engine FggUtilities glfw cglm PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)