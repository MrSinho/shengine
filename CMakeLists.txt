cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shprojects)

option(BUILD_SH_ENGINE OFF)
option(BUILD_SIMULATIONS OFF)


if (BUILD_SH_ENGINE)
add_subdirectory(shengine)
endif(BUILD_SH_ENGINE)

if (BUILD_SIMULATIONS)
add_subdirectory(simulations)
endif(BUILD_SIMULATIONS)


