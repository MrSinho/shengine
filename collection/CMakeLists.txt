cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shengine C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC  ON)

#SUBDIRECTORIES
if (NOT TARGET externals)
add_subdirectory(externals)
endif()

#OPTIONS
option(SH_ENGINE_BUILD_EDITOR CACHE OFF)



include(collection.cmake)



function(collection_main)
build_shenvironment()
build_shapplication_host()
build_shengine()

if(SH_ENGINE_BUILD_EDITOR)
build_sheditor()
endif(SH_ENGINE_BUILD_EDITOR)

endfunction()



collection_main()