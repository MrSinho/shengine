cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

if (NOT DEFINED SH_SIMULATION_NAME)
set (SH_SIMULATION_NAME shprojects)
endif()

project(${SH_SIMULATION_NAME} C)

option(SIMULATION_PATH CACHE OFF)

add_subdirectory(shengine)

if (SIMULATION_PATH)
add_subdirectory(${SIMULATION_PATH} [EXCLUDE_FROM_ALL])
endif()


