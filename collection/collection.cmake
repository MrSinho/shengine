cmake_minimum_required(VERSION 3.13)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SH_ENGINE_BINARIES_DIR)
message(FATAL_ERROR "shengine cmake error: missing SH_ENGINE_BINARIES_DIR")
endif()

if (NOT DEFINED SH_ENGINE_ROOT_DIR)
message(FATAL_ERROR "shengine cmake error: missing SH_ENGINE_ROOT_DIR")
endif()



function(build_shenvironment)

if (NOT TARGET externals)
add_subdirectory(${SH_ENGINE_ROOT_DIR}/collection/externals [EXCLUDE_FROM_ALL])
endif()

add_library(shenvironment STATIC 
    ${SH_ENGINE_ROOT_DIR}/collection/shenvironment/src/shEnvironment.c
    ${SH_ENGINE_ROOT_DIR}/collection/shenvironment/src/shFileUtilities.c
)
target_include_directories(shenvironment PUBLIC 
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/include
    ${SH_ENGINE_ROOT_DIR}/collection/shenvironment/include
    ${SH_ENGINE_ROOT_DIR}/collection/shcomponents/include
    ${SH_ENGINE_ROOT_DIR}/collection/shcardinputs/include
)
target_link_libraries(shenvironment PUBLIC 
    smd 
    shvulkan
)

endfunction()



function(build_shapplication_host)
add_library(shapplication-host 
    ${SH_ENGINE_ROOT_DIR}/collection/shapplication-host/src/shApplicationHost.c
)
target_include_directories(shapplication-host PUBLIC
    ${SH_ENGINE_ROOT_DIR}/collection/shapplication-host/include
    ${SH_ENGINE_ROOT_DIR}/collection/externals
)
target_link_libraries(shapplication-host PUBLIC
    shenvironment
    #shthreads
)
if (WIN32)
set_target_properties(shapplication-host PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/windows
)
else()
set_target_properties(shapplication-host PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/linux
)
endif(WIN32)
endfunction()



function(build_shengine)
add_library(shengine 
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/src/shEngine.c
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/src/shInput.c
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/src/shTime.c
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/src/shProfiling.c
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/src/shWindow.c
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/src/shLinearAlgebra.c
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/src/shScene.c
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/src/shEngineUI.c
)
target_include_directories(shengine PUBLIC 
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/include
    ${SH_ENGINE_ROOT_DIR}/collection/shcomponents/include
)
target_link_libraries(shengine PUBLIC
    glfw
    shvulkan
    cglm
    plyimporter
    shapplication-host
    shenvironment
    shserial
    #shthreads
    shgui
)
endfunction()



function(build_sheditor)
add_executable(sheditor 
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/src/shEditor.c
)
target_link_libraries(sheditor PUBLIC 
    shengine
)
target_include_directories(sheditor PUBLIC
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/include
)
endfunction()



function(set_shengine_collection_binaries)

if (WIN32)
set_target_properties(
    shengine
    glfw
    shvulkan
    cglm
    plyimporter
    shapplication-host
    shenvironment
    shserial
    #shthreads
    shgui
    native-export
    sheditor
    
    PROPERTIES

    ARCHIVE_OUTPUT_DIRECTORY      ${SH_ENGINE_BINARIES_DIR}/windows
    RUNTIME_OUTPUT_DIRECTORY      ${SH_ENGINE_BINARIES_DIR}/windows
    VS_DEBUGGER_WORKING_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/windows
)
else()
set_target_properties(
    shengine
    glfw
    shvulkan
    cglm
    plyimporter
    shapplication-host
    shenvironment
    shserial
    #shthreads
    shgui
    native-export
    sheditor
    
    PROPERTIES

    ARCHIVE_OUTPUT_DIRECTORY      ${SH_ENGINE_BINARIES_DIR}/linux
    RUNTIME_OUTPUT_DIRECTORY      ${SH_ENGINE_BINARIES_DIR}/linux
    VS_DEBUGGER_WORKING_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/linux
)
endif(WIN32)

endfunction()


function(build_shengine_collection)
build_shapplication_host()
build_shenvironment()
build_shengine()
build_sheditor()
set_shengine_collection_binaries()
endfunction()