cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shprojects)

option(SIMULATION_PATH OFF)

if (NOT SIMULATION_PATH)
set(SIMULATION_PATH "simulations")
endif()

add_subdirectory(shengine)

add_subdirectory(${SIMULATION_PATH} [EXCLUDE_FROM_ALL])


