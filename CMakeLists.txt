cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(${SH_APPLICATION_NAME} C)



set(SH_ENGINE_VERSION  "pre-release")
set(SH_ENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})



option(SH_ENGINE_BINARIES_DIR CACHE OFF)
option(SH_APPLICATION_PATH    CACHE OFF)



if (NOT SH_ENGINE_BINARIES_DIR)
set(SH_ENGINE_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()



if (NOT DEFINED SH_APPLICATION_PATH)
set (SH_APPLICATION_PATH undefined)
endif()



if (NOT DEFINED SH_APPLICATION_NAME)
set (SH_APPLICATION_NAME shprojects)
endif()



message(STATUS "SH_ENGINE_VERSION       : ${SH_ENGINE_VERSION}")
message(STATUS "SH_ENGINE_ROOT_DIR      : ${SH_ENGINE_ROOT_DIR}")
message(STATUS "SH_ENGINE_BINARIES_DIR  : ${SH_ENGINE_BINARIES_DIR}")
message(STATUS "SH_APPLICATION_PATH     : ${SH_APPLICATION_PATH}")
message(STATUS "SH_APPLICATION_NAME     : ${SH_APPLICATION_NAME}")



add_subdirectory(collection)

if (SH_APPLICATION_PATH)
add_subdirectory(${SH_APPLICATION_PATH} [EXCLUDE_FROM_ALL])
endif()



include(docs/shengine-docs.cmake)



function(main)
build_shengine_docs()
endfunction()



main()