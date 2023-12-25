cmake_minimum_required(VERSION 3.13)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



if (NOT DEFINED SH_ENGINE_BINARIES_DIR)
message(FATAL_ERROR "shengine cmake error: missing SH_ENGINE_BINARIES_DIR")
endif()

if (NOT DEFINED SH_ENGINE_ROOT_DIR)
message(FATAL_ERROR "shengine cmake error: missing SH_ENGINE_ROOT_DIR")
endif()



if (NOT TARGET shvulkan)
add_subdirectory(../externals/shvulkan EXCLUDE_FROM_ALL)
endif()



function(build_shengine)
add_library(shengine 
    shengine/src/shEngine.c
    shengine/src/shInput.c
    shengine/src/shTime.c
    shengine/src/shProfiling.c
    shengine/src/shWindow.c
    shengine/src/shLinearAlgebra.c
    shengine/src/shScene.c
    shengine/src/shEngineUI.c
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
    shthreads
    shgui
)
if (WIN32)
set_target_properties(shengine glfw cglm shvulkan plyimporter shgui PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/windows
)
else()
set_target_properties(shengine glfw cglm shvulkan plyimporter shgui PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/linux
)
endif(WIN32)
endfunction()



function(build_shenvironment)
add_library(shenvironment STATIC 
    shenvironment/src/shEnvironment.c
    shenvironment/src/shFileUtilities.c
)
target_include_directories(shenvironment PUBLIC 
    ${SH_ENGINE_ROOT_DIR}/collection/shengine/include
    ${SH_ENGINE_ROOT_DIR}/collection/shenvironment/include
    ${SH_ENGINE_ROOT_DIR}/collection/shcomponents/include
    ${SH_ENGINE_ROOT_DIR}/collection/shcardinputs/include
)
target_link_libraries(shenvironment PUBLIC smd shvulkan)
if (WIN32)
set_target_properties(shenvironment smd shvulkan PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/windows
)
else()
set_target_properties(shenvironment PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/linux
)
endif(WIN32)
endfunction()




function(build_sheditor)
add_executable(sheditor sheditor/src/shEditor.c)
target_link_libraries(sheditor PUBLIC 
    shengine
)
target_include_directories(sheditor PUBLIC
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/include
)
if (WIN32)
set_target_properties(sheditor PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY      ${SH_ENGINE_BINARIES_DIR}/windows
    VS_DEBUGGER_WORKING_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/windows
)
else()
set_target_properties(sheditor PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY      ${SH_ENGINE_BINARIES_DIR}/linux
    VS_DEBUGGER_WORKING_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/linux
)
endif(WIN32)
endfunction()



function(build_shapplication_host)
add_library(shapplication-host 
shapplication-host/src/shApplicationHost.c
)
target_include_directories(shapplication-host PUBLIC
${SH_ENGINE_ROOT_DIR}/collection/shapplication-host/include
${SH_ENGINE_ROOT_DIR}/collection/externals
)
target_link_libraries(shapplication-host PUBLIC
    shenvironment
    shthreads
)
if (WIN32)
set_target_properties(shapplication-host shthreads PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/windows
)
else()
set_target_properties(shapplication-host shthreads PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${SH_ENGINE_BINARIES_DIR}/linux
)
endif(WIN32)
endfunction()
