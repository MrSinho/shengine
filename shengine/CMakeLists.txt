cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(ShEngine C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC ON)

#SUBDIRECTORIES
add_subdirectory(../externals [EXCLUDE_FROM_ALL])

#shengine
set(SH_ENGINE_SRC_DIR shengine/src)
add_library(shengine STATIC 
    ${SH_ENGINE_SRC_DIR}/shengine/shInput.c
    ${SH_ENGINE_SRC_DIR}/shengine/shTime.c
    ${SH_ENGINE_SRC_DIR}/shengine/shWindow.c
    ${SH_ENGINE_SRC_DIR}/shlinear-algebra/shProjection.c
    ${SH_ENGINE_SRC_DIR}/shlinear-algebra/shView.c
    ${SH_ENGINE_SRC_DIR}/shscene/shScene.c
)
target_include_directories(shengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/shengine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shecs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../externals/ShECS/include
)
target_link_libraries(shengine PUBLIC
    glfw
    shvulkan
    cglm
)
set_target_properties(shengine glfw shvulkan cglm PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

#shfd
option(SH_ENGINE_USE_SHFD OFF)

if (SH_ENGINE_BUILD_EDITOR)
set(SH_ENGINE_BUILD_SHFD ON)
endif(SH_ENGINE_BUILD_EDITOR)

if (SH_ENGINE_BUILD_SHFD)

add_library(shfd STATIC 
    shfd/src/shFile.c
    shfd/src/shfd.c
)
target_include_directories(shfd PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../externals/ShECS/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shecs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shfd/include
)
###add_subdirectory(externals/json-c)
target_link_libraries(shfd PUBLIC json-c shvulkan plyimporter)
set_target_properties(shfd PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(SH_ENGINE_BUILD_SHFD)

#editor
option(SH_ENGINE_BUILD_EDITOR OFF)
if(SH_ENGINE_BUILD_EDITOR)
add_executable(sheditor sheditor/src/shEditor.c)
target_link_libraries(sheditor PUBLIC 
    shengine
    shfd
)
target_include_directories(sheditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/sheditor/include
)
set_target_properties(sheditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endif(SH_ENGINE_BUILD_EDITOR)