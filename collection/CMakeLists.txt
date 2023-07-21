cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(shengine C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC ON)

#SUBDIRECTORIES
if (NOT TARGET externals)
add_subdirectory(externals)
endif()

#OPTIONS
option(SH_ENGINE_BUILD_EDITOR CACHE OFF)
option(SH_ENGINE_BUILD_SHARED_HOST CACHE OFF)
option(SH_ENGINE_ASSETS_PATH CACHE OFF)



function(build_shengine)
add_library(shengine 
    shengine/src/shEngine.c
    shengine/src/shInput.c
    shengine/src/shTime.c
    shengine/src/shProfiling.c
    shengine/src/shWindow.c
    shengine/src/shLinearAlgebra.c
    shengine/src/shScene.c
)
target_include_directories(shengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/shengine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shcomponents/include
)
target_link_libraries(shengine PUBLIC
    glfw
    shvulkan
    cglm
    shapplicationhost
    shenvironment
    shserial
    shthreads
)
set_target_properties(shengine PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



function(build_shenvironment)
add_library(shenvironment STATIC 
    shenvironment/src/shEnvironment.c
    shenvironment/src/shFileUtilities.c
)
target_include_directories(shenvironment PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/shengine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shenvironment/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shcomponents/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shcardinputs/include
)
target_link_libraries(shenvironment PUBLIC smd shvulkan)
set_target_properties(shenvironment PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endfunction()




function(build_sheditor)
add_executable(sheditor sheditor/src/shEditor.c)
target_link_libraries(sheditor PUBLIC 
    shengine
)
target_include_directories(sheditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/sheditor/include
)
set_target_properties(sheditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



function(build_shapplication_host)
add_library(shapplicationhost 
shapplicationhost/src/shApplicationHost.c
)
target_include_directories(shapplicationhost PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/shapplicationhost/include
${CMAKE_CURRENT_SOURCE_DIR}/externals
)
target_link_libraries(shapplicationhost PUBLIC shenvironment shthreads)
endfunction()


function(main)
build_shenvironment()
build_shapplication_host()
build_shengine()

if(SH_ENGINE_BUILD_EDITOR)
build_sheditor()
endif(SH_ENGINE_BUILD_EDITOR)

endfunction()



main()