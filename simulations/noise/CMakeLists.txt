cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

	

project(noise C)

option(SH_SIMULATION_BINARY_TYPE CACHE "EXECUTABLE")
if("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "STATIC")
    add_library(${SH_SIMULATION_NAME} STATIC 
        	C:/Users/sino3/Desktop/GitHub/shengine/simulations/noise//src/noise.c

    )
    
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_SIMULATION_NAME} SHARED 
        	C:/Users/sino3/Desktop/GitHub/shengine/simulations/noise//src/noise.c

    )
    target_compile_definitions(shengine PUBLIC SH_SIMULATION_TARGET_TYPE_SHARED=1)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_SIMULATION_NAME}  
        	C:/Users/sino3/Desktop/GitHub/shengine/simulations/noise//src/noise.c

        C:/Users/sino3/Desktop/GitHub/shengine/shengine/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_SIMULATION_NAME} PUBLIC C:/Users/sino3/Desktop/GitHub/shengine/shengine/sheditor/include)
    target_compile_definitions(${SH_SIMULATION_NAME} PUBLIC SH_SIMULATION_TARGET_TYPE_EXECUTABLE=1)
endif()

target_include_directories(${SH_SIMULATION_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/${SH_SIMULATION_NAME}/include
)
target_link_libraries(
    ${SH_SIMULATION_NAME} PUBLIC shengine 	
)
set_target_properties(${SH_SIMULATION_NAME} PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/noise  
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/noise
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/noise
)
