cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(PROJECT_NAME SH-Engine)
project(${PROJECT_NAME} C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC ON)

#SUBDIRECTORIES
add_subdirectory(Externals/glfw)
add_subdirectory(Externals/cglm)
add_subdirectory(Externals/PlyImporter)
add_subdirectory(Externals/json-c)
add_subdirectory(Externals/ShPhysics)
add_subdirectory(Externals/ShSerial)

#INSTALLED PACKAGES
find_package(Vulkan REQUIRED FATAL_ERROR)

option(SH_USE_VULKAN_1_0 OFF)
if(SH_USE_VULKAN_1_0)
    add_definitions(-DSH_USE_VULKAN_1_0)
endif(SH_USE_VULKAN_1_0)

option(SH_USE_VULKAN_1_1 OFF)
if(SH_USE_VULKAN_1_1)
    add_definitions(-DSH_USE_VULKAN_1_1)
endif(SH_USE_VULKAN_1_1)

#SHUTILITIES
file(GLOB_RECURSE SH_UTILITIES_SRC ShUtilities/src/*.c)
add_library(ShUtilities STATIC ${SH_UTILITIES_SRC})
target_include_directories(ShUtilities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ShUtilities/include
    ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(ShUtilities PUBLIC 
    ${VULKAN_LIBRARIES}
)

#SHDESCRIPTORHANDLE
file(GLOB_RECURSE SH_DESCRIPTOR_HANDLE_SRC ShDescriptorHandle/src/*.c)
add_library(ShDescriptorHandle ${SH_DESCRIPTOR_HANDLE_SRC})
target_include_directories(ShDescriptorHandle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ShEcsImplementation/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ShDescriptorHandle/include
)
target_link_libraries(ShDescriptorHandle PUBLIC 
    ShUtilities
    json-c
    PlyImporter
)

#FOGGY-ENGINE
file(GLOB_RECURSE FOGGY_ENGINE_SRC SH-Engine/src/*.c)
add_library(SH-Engine STATIC ${FOGGY_ENGINE_SRC})
target_include_directories(SH-Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/ShEcsImplementation/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/ShECS/ShECS/include
    ${CMAKE_CURRENT_SOURCE_DIR}/SH-Engine/include
    ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(SH-Engine PUBLIC
    glfw
    ${Vulkan_LIBRARIES}      
    ShUtilities
    cglm
    PlyImporter
    ShDescriptorHandle
)

#SHADERS
message(STATUS "looking for glslc")
find_program(GLSLC_FOUND glslc)
if (NOT GLSLC_FOUND)
    message(STATUS "glslc not found, required for shader compilation, install before executing the engine in debug mode")
else()
    message(STATUS "found glslc")
endif()


#EDITOR
file(GLOB_RECURSE EDITOR_SRC Editor/src/*.c)
add_executable(Editor ${EDITOR_SRC})
target_link_libraries(Editor PUBLIC 
    SH-Engine
    ShUtilities
    ShPhysics
    ShSerial
)
target_include_directories(Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor/include
)

#OUTPUT
set_target_properties(Editor SH-Engine ShUtilities glfw cglm PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)