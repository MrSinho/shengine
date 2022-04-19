cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

project(ShEngine C)

#SUBDIRECTORIES OPTIONS
set(CGLM_USE_C99 ON)
set(CGLM_STATIC ON)

#SUBDIRECTORIES
add_subdirectory(../externals [EXCLUDE_FROM_ALL])

#OPTIONS
option(SH_ENGINE_BUILD_EDITOR OFF)
option(SH_ENGINE_BUILD_SHARED_HOST OFF)
option(SH_ENGINE_ASSETS_PATH OFF)

if (NOT SH_EDITOR_ASSETS_PATH)
    set(SH_EDITOR_ASSETS_PATH "../assets")
endif()

function(build_shengine)
add_library(shengine 
    shengine/src/shengine/shInput.c
    shengine/src/shengine/shTime.c
    shengine/src/shengine/shWindow.c
    shengine/src/shlinear-algebra/shProjection.c
    shengine/src/shlinear-algebra/shView.c
    shengine/src/shscene/shScene.c
    shengine/src/shmaterialhost/shMaterialHost.c
)
target_include_directories(shengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/shengine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shecs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../externals/shecs/include
)
target_link_libraries(shengine PUBLIC
    glfw
    shvulkan
    cglm
    shsharedhost
    shfd
    shserial
)
set_target_properties(shengine PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
endfunction()



function(build_shfd)
add_library(shfd STATIC 
    shfd/src/shFile.c
    shfd/src/shfd.c
)
target_include_directories(shfd PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../externals/shecs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shecs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shfd/include
)
target_link_libraries(shfd PUBLIC json-c shvulkan plyimporter)
target_compile_definitions(shfd PUBLIC SH_EDITOR_ASSETS_PATH="${SH_EDITOR_ASSETS_PATH}")
set_target_properties(shfd PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
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



function(build_shshared_host)
add_library(shsharedhost 
shsharedhost/src/shSharedHost.c
)
target_include_directories(shsharedhost PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/shsharedhost/include
)
target_link_libraries(shsharedhost PUBLIC shfd)
endfunction()


function(main)
build_shfd()
build_shshared_host()
build_shengine()

if(SH_ENGINE_BUILD_EDITOR)
build_sheditor()
endif(SH_ENGINE_BUILD_EDITOR)

endfunction()



main()
