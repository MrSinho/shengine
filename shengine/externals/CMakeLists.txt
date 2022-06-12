cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

add_subdirectory(shvulkan)
add_subdirectory(shecs)
add_subdirectory(plyimporter)
add_subdirectory(shserial)

add_subdirectory(glfw)
add_subdirectory(cglm)
add_subdirectory(json-c)