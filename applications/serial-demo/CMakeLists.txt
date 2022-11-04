cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

	

project(serial-demo C)

option(SH_APPLICATION_BINARY_TYPE CACHE "EXECUTABLE")
if("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "STATIC")
    add_library(${SH_APPLICATION_NAME} STATIC 
        	C:/Users/sino3/Desktop/GitHub/shengine/applications/serial-demo/src/serial-demo.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	C:/Users/sino3/Desktop/GitHub/shengine/applications/serial-demo/src/serial-demo.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	C:/Users/sino3/Desktop/GitHub/shengine/applications/serial-demo/src/serial-demo.c

        C:/Users/sino3/Desktop/GitHub/shengine/shengine/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC C:/Users/sino3/Desktop/GitHub/shengine/shengine/sheditor/include)
    target_compile_definitions(${SH_APPLICATION_NAME} PUBLIC SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1)
endif()

target_include_directories(${SH_APPLICATION_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    ${SH_APPLICATION_NAME} PUBLIC shengine 	
)
set_target_properties(${SH_APPLICATION_NAME} PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/ 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
)
