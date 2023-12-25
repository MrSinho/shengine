cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(${SH_APPLICATION_NAME} C)



set(SH_ENGINE_VERSION  "v1.0.0")
set(SH_ENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})



set(SH_ENGINE_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE STRING "Default SH_ENGINE_BINARIES_DIR")
option(SH_APPLICATION_PATH CACHE OFF)



if (NOT DEFINED SH_APPLICATION_NAME)
set (SH_APPLICATION_NAME shprojects)
endif()



add_subdirectory(collection)

if (SH_APPLICATION_PATH)
add_subdirectory(${SH_APPLICATION_PATH} [EXCLUDE_FROM_ALL])
endif()



include(docs/shengine-docs.cmake)



function(main)
build_shengine_docs()
endfunction()



main()