
cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

option(SH_SIMULATION_NAME EmptyProject)
project(${SH_SIMULATION_NAME})

option(SH_SIMULATION_BINARY_TYPE "EXECUTABLE")
if("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "STATIC")
    add_library(${SH_SIMULATION_NAME} STATIC 
    example/src/rotator.c
)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_SIMULATION_NAME} SHARED 
    example/src/rotator.c
)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_SIMULATION_NAME}  
    example/src/rotator.c
)
endif()

target_link_libraries(${SH_SIMULATION_NAME} PUBLIC shengine)
set_target_properties(${SH_SIMULATION_NAME} PROPERTIES 
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin  
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
