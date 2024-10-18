cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(triangle C)



# YOU CAN MODIFY
set(TRIANGLE_VERSION pre-release)

set(TRIANGLE_ROOT_DIR     ${CMAKE_CURRENT_SOURCE_DIR})
set(TRIANGLE_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)



# YOU CAN MODIFY
option(TRIANGLE_BINARY_TYPE CACHE OFF)
option(SH_ENGINE_ROOT_DIR     CACHE OFF)
option(SH_ENGINE_BINARIES_DIR CACHE OFF)



# YOU CAN MODIFY 
if (NOT TRIANGLE_BINARY_TYPE)
set(TRIANGLE_BINARY_TYPE "EXECUTABLE")
endif()

# DO NOT MODIFY!
if (NOT SH_ENGINE_ROOT_DIR)
message(FATAL_ERROR "triangle: missing SH_ENGINE_ROOT_DIR")
endif()

# DO NOT MODIFY!
if (NOT SH_ENGINE_BINARIES_DIR)
set(SH_ENGINE_BINARIES_DIR ${SH_ENGINE_ROOT_DIR}/bin)
endif()


message(STATUS "TRIANGLE_ROOT_DIR      : ${TRIANGLE_ROOT_DIR}")
message(STATUS "TRIANGLE_BINARIES_DIR  : ${TRIANGLE_BINARIES_DIR}")
message(STATUS "TRIANGLE_BINARY_TYPE   : ${TRIANGLE_BINARY_TYPE}")

message(STATUS "SH_ENGINE_ROOT_DIR     : ${SH_ENGINE_ROOT_DIR}")
message(STATUS "SH_ENGINE_BINARIES_DIR : ${SH_ENGINE_BINARIES_DIR}")



include(${SH_ENGINE_ROOT_DIR}/collection/collection.cmake)
include(${SH_ENGINE_ROOT_DIR}/docs/shengine-docs.cmake)


function(build_triangle)

build_shengine_collection()
build_shengine_docs()


if(${TRIANGLE_BINARY_TYPE} STREQUAL "STATIC")
add_library(triangle STATIC 
    ${TRIANGLE_ROOT_DIR}/src/triangle.c
    #src_files
)


elseif(${TRIANGLE_BINARY_TYPE} STREQUAL "SHARED")
add_library(triangle SHARED 
    ${TRIANGLE_ROOT_DIR}/src/triangle.c
    #src_files
)
target_compile_definitions(shengine PUBLIC 
    SH_APPLICATION_TARGET_TYPE_SHARED=1
)


elseif(${TRIANGLE_BINARY_TYPE} STREQUAL "EXECUTABLE")
add_executable(triangle  
    ${TRIANGLE_ROOT_DIR}/src/triangle.c
    #src_files
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/src/shEditor.c
)
target_include_directories(triangle PUBLIC 
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/include
)
target_compile_definitions(triangle PUBLIC 
    SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1
)
endif()


target_include_directories(triangle PUBLIC 
    ${TRIANGLE_ROOT_DIR}/include
)

target_link_libraries(triangle PUBLIC
    shengine
)

if (WIN32)
set_target_properties(triangle sheditor PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY      ${TRIANGLE_BINARIES_DIR}/windows
    RUNTIME_OUTPUT_DIRECTORY      ${TRIANGLE_BINARIES_DIR}/windows
    VS_DEBUGGER_WORKING_DIRECTORY ${TRIANGLE_BINARIES_DIR}/windows
)
else()
set_target_properties(triangle sheditor PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY      ${TRIANGLE_BINARIES_DIR}/linux 
    RUNTIME_OUTPUT_DIRECTORY      ${TRIANGLE_BINARIES_DIR}/linux
    VS_DEBUGGER_WORKING_DIRECTORY ${TRIANGLE_BINARIES_DIR}/linux
)
endif(WIN32)

set_shengine_collection_binaries()

endfunction()


function(triangle_main)
build_triangle()
endfunction()



triangle_main()
