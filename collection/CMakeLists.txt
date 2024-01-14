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



collection_main()