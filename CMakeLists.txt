cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(PROJECT_NAME Foggy-Engine)
project(${PROJECT_NAME} VERSION 0.1)

add_subdirectory(Externals/glfw)
find_package(Vulkan REQUIRED FATAL_ERROR)

#ENGINE
file(GLOB_RECURSE ENGINE_SRC Foggy-Engine/src/*.cpp)
add_library(Foggy-Engine STATIC ${ENGINE_SRC})
target_include_directories(Foggy-Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Foggy-Engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Externals/glm
    ${Vulkan_INCLUDE_DIR}
)
target_link_libraries(Foggy-Engine PUBLIC
    ${Vulkan_LIBRARIES}      
    glfw
)

#EDITOR
file(GLOB_RECURSE EDITOR_SRC Editor/src/*.cpp)
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
)