cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(noise C)



# YOU CAN MODIFY
set(NOISE_ROOT_DIR     ${CMAKE_CURRENT_SOURCE_DIR})
set(NOISE_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)



# YOU CAN MODIFY
option(NOISE_BINARY_TYPE CACHE OFF)
option(SH_ENGINE_ROOT_DIR     CACHE OFF)
option(SH_ENGINE_BINARIES_DIR CACHE OFF)



# YOU CAN MODIFY 
if (NOT NOISE_BINARY_TYPE)
set(NOISE_BINARY_TYPE "EXECUTABLE")
endif()

# DO NOT MODIFY!
if (NOT SH_ENGINE_ROOT_DIR)
message(FATAL_ERROR "noise: missing SH_ENGINE_ROOT_DIR")
endif()

# DO NOT MODIFY!
if (NOT SH_ENGINE_BINARIES_DIR)
set(SH_ENGINE_BINARIES_DIR ${SH_ENGINE_ROOT_DIR}/bin)
endif()


message(STATUS "NOISE_ROOT_DIR      : ${NOISE_ROOT_DIR}")
message(STATUS "NOISE_BINARIES_DIR  : ${NOISE_BINARIES_DIR}")
message(STATUS "NOISE_BINARY_TYPE   : ${NOISE_BINARY_TYPE}")

message(STATUS "SH_ENGINE_ROOT_DIR     : ${SH_ENGINE_ROOT_DIR}")
message(STATUS "SH_ENGINE_BINARIES_DIR : ${SH_ENGINE_BINARIES_DIR}")



include(${SH_ENGINE_ROOT_DIR}/collection/collection.cmake)


function(build_noise)

build_shengine_collection()


if(${NOISE_BINARY_TYPE} STREQUAL "STATIC")
add_library(noise STATIC 
    ${NOISE_ROOT_DIR}/src/noise.c
    #src_files
)


elseif(${NOISE_BINARY_TYPE} STREQUAL "SHARED")
add_library(noise SHARED 
    ${NOISE_ROOT_DIR}/src/noise.c
    #src_files
)
target_compile_definitions(shengine PUBLIC 
    SH_APPLICATION_TARGET_TYPE_SHARED=1
)
set_target_properties(noise sheditor PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY      ${NOISE_BINARIES_DIR}/windows 
    RUNTIME_OUTPUT_DIRECTORY      ${NOISE_BINARIES_DIR}/windows
    VS_DEBUGGER_WORKING_DIRECTORY ${NOISE_BINARIES_DIR}/windows
)


elseif(${NOISE_BINARY_TYPE} STREQUAL "EXECUTABLE")
add_executable(noise  
    ${NOISE_ROOT_DIR}/src/noise.c
    #src_files
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/src/shEditor.c
)
target_include_directories(noise PUBLIC 
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/include
)
target_compile_definitions(noise PUBLIC 
    SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1
)
set_target_properties(noise PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY      ${NOISE_BINARIES_DIR}/bin 
    RUNTIME_OUTPUT_DIRECTORY      ${NOISE_BINARIES_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${NOISE_BINARIES_DIR}/bin
)
endif()


target_include_directories(noise PUBLIC 
    ${NOISE_ROOT_DIR}/include
)
target_link_libraries(noise PUBLIC
    shengine
)

endfunction()


function(noise_main)
build_noise()
endfunction()



noise_main()
