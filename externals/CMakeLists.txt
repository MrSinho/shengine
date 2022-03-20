cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

add_subdirectory(ShVulkan)
add_subdirectory(ShECS)
add_subdirectory(PlyImporter)

add_subdirectory(glfw)
add_subdirectory(cglm)
add_subdirectory(json-c)