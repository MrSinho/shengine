cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

if (NOT DEFINED SH_APPLICATION_NAME)
set (SH_APPLICATION_NAME shprojects)
endif()

project(${SH_APPLICATION_NAME} C)

option(SH_APPLICATION_PATH CACHE OFF)

add_subdirectory(shengine)

if (SH_APPLICATION_PATH)
add_subdirectory(${SH_APPLICATION_PATH} [EXCLUDE_FROM_ALL])
endif()#SH_APPLICATION_PATH


