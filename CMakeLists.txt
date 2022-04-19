cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shprojects C)

option(SIMULATION_PATH OFF)

if (NOT SIMULATION_PATH)
set(SIMULATION_PATH "simulations/flappy-circle/")
endif()

add_subdirectory(shengine)

add_subdirectory(${SIMULATION_PATH} [EXCLUDE_FROM_ALL])


