cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(PROJECT_NAME Foggy-Engine)
project(${PROJECT_NAME} C)

#SUBDIRECTORIES
add_subdirectory(Externals/glfw)
add_subdirectory(Externals/cglm EXCLUDE_FROM_ALL)
option(CGLM_USE_C99 "" ON)

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

#UFBX
#add_library(ufbx STATIC Externals/ufbx/ufbx.c)
#set(UFBX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Externals/ufbx)
#target_include_directories(ufbx PUBLIC ${UFBX_INCLUDE_DIR})

#ENGINE
file(GLOB_RECURSE ENGINE_SRC Foggy-Engine/src/*.c)
add_library(Foggy-Engine STATIC ${ENGINE_SRC})
target_include_directories(Foggy-Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Foggy-Engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/EZ-ECS/EZ-ECS/include
    ${CGS_INCLUDE_DIR}
    ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(Foggy-Engine PUBLIC
    glfw
    ${Vulkan_LIBRARIES}      
    Utilities
    cglm
)

#UTILITIES
file(GLOB_RECURSE UTILITIES_SRC Utilities/src/*.c)
add_library(Utilities STATIC ${UTILITIES_SRC})
target_include_directories(Utilities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities/include
    ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries(Utilities PUBLIC 
    ${VULKAN_LIBRARIES}
)
#SHADERS
message(STATUS "looking for glslc")
find_program(GLSLC_FOUND glslc)
if (NOT GLSLC_FOUND)
    find_program(STATUS "glslc not found, required for shader compilation, install before executing the engine")
else()
    message(STATUS "found glslc")
endif()


#EDITOR
file(GLOB_RECURSE EDITOR_SRC Editor/src/*.c)
add_executable(Editor ${EDITOR_SRC})
target_link_libraries(Editor PUBLIC 
    Foggy-Engine
)
target_include_directories(Editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor/include
)

#OUTPUT
set_target_properties(Editor Foggy-Engine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
