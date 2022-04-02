cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shprojects)

option(BUILD_SH_ENGINE OFF)
option(SIMULATION_PATH OFF)

if (NOT SIMULATION_PATH)
set(SIMULATION_PATH "simulations")
endif()

if (BUILD_SH_ENGINE)
add_subdirectory(shengine)
endif(BUILD_SH_ENGINE)

if (BUILD_SIMULATIONS)
add_subdirectory(${SIMULATION_PATH} [EXCLUDE_FROM_ALL])
endif(BUILD_SIMULATIONS)


