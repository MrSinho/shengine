cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(${SH_APPLICATION_NAME} C)



set(SH_ENGINE_VERSION  "pre-release")
set(SH_ENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})



option(SH_ENGINE_BINARIES_DIR CACHE OFF)



if (NOT SH_ENGINE_BINARIES_DIR)
set(SH_ENGINE_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()



message(STATUS "SH_ENGINE_VERSION       : ${SH_ENGINE_VERSION}")
message(STATUS "SH_ENGINE_ROOT_DIR      : ${SH_ENGINE_ROOT_DIR}")
message(STATUS "SH_ENGINE_BINARIES_DIR  : ${SH_ENGINE_BINARIES_DIR}")



if (NOT TARGET externals)
set(CGLM_USE_C99 ON)
set(CGLM_STATIC  ON)
add_subdirectory(${SH_ENGINE_ROOT_DIR}/collection/externals)
endif()



include(${SH_ENGINE_ROOT_DIR}/collection/collection.cmake)
include(${SH_ENGINE_ROOT_DIR}/docs/shengine-docs.cmake)



function(main)
build_shengine_docs()
build_shengine_collection()
endfunction()



main()