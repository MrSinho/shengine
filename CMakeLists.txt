cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shprojects C)

option(SIMULATION_PATH OFF)

add_subdirectory(shengine)

if (SIMULATION_PATH)
add_subdirectory(${SIMULATION_PATH} [EXCLUDE_FROM_ALL])
endif()


