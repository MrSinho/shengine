import sys
import os
import pathlib
import platform
import curses


class Application:
    name:str            = ""
    path:str            = ""
    target_type:str     = ""
    generator:str       = ""
    clean_cache:str     = ""



def read_arg(arg:str, application:Application):
    if (arg.startswith("name=")):
        application.name = arg.removeprefix("name=")
    elif (arg.startswith("path=")):
        application.path = arg.removeprefix("path=")


def print_separator():
    print("--------------------------------------------------------------------------------\n")
    return

def main():

    print_separator()
    print("EXAMPLE CALL: python export-application.py name=noise target=SHARED")
    print("EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise")
    print("EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles")#CMAKE STANDARD GENERATOR NAMES BUT REPLACE " " with "-"
    print("EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE")
    print_separator()
    print("\n")

    

    python_src_dir:str = str(pathlib.Path(__file__).parent.resolve().as_posix())
    os_name:str        = platform.system().lower()


    application:Application = Application()
    for i in range (0, len(sys.argv), 1):
        read_arg(str(sys.argv[i]), application)


    print_separator()
    print(f"platform:       {os_name}")
    print(f"python_src_dir: {python_src_dir}")
    print(f"name:           {application.name}")
    print(f"path:           {application.path}")
    print_separator()
    print("\n")

    

    application_caps_cmake_name:str = application.name.upper().replace("-", "_")
    application_underscore_name:str = application.name.replace("-", "_")


    cmake_file:str = f"""cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project({application.name} C)



# YOU CAN MODIFY
set({application_caps_cmake_name}_VERSION pre-release)

set({application_caps_cmake_name}_ROOT_DIR     ${{CMAKE_CURRENT_SOURCE_DIR}})
set({application_caps_cmake_name}_BINARIES_DIR ${{CMAKE_CURRENT_SOURCE_DIR}}/bin)



# YOU CAN MODIFY
option({application_caps_cmake_name}_BINARY_TYPE CACHE OFF)
option(SH_ENGINE_ROOT_DIR     CACHE OFF)
option(SH_ENGINE_BINARIES_DIR CACHE OFF)



# YOU CAN MODIFY 
if (NOT {application_caps_cmake_name}_BINARY_TYPE)
set({application_caps_cmake_name}_BINARY_TYPE "EXECUTABLE")
endif()

# DO NOT MODIFY!
if (NOT SH_ENGINE_ROOT_DIR)
message(FATAL_ERROR "{application.name}: missing SH_ENGINE_ROOT_DIR")
endif()

# DO NOT MODIFY!
if (NOT SH_ENGINE_BINARIES_DIR)
set(SH_ENGINE_BINARIES_DIR ${{SH_ENGINE_ROOT_DIR}}/bin)
endif()


message(STATUS "{application_caps_cmake_name}_ROOT_DIR      : ${{{application_caps_cmake_name}_ROOT_DIR}}")
message(STATUS "{application_caps_cmake_name}_BINARIES_DIR  : ${{{application_caps_cmake_name}_BINARIES_DIR}}")
message(STATUS "{application_caps_cmake_name}_BINARY_TYPE   : ${{{application_caps_cmake_name}_BINARY_TYPE}}")

message(STATUS "SH_ENGINE_ROOT_DIR     : ${{SH_ENGINE_ROOT_DIR}}")
message(STATUS "SH_ENGINE_BINARIES_DIR : ${{SH_ENGINE_BINARIES_DIR}}")



include(${{SH_ENGINE_ROOT_DIR}}/collection/collection.cmake)
include(${{SH_ENGINE_ROOT_DIR}}/docs/shengine-docs.cmake)


function(build_{application.name})

build_shengine_collection()
build_shengine_docs()


if(${{{application_caps_cmake_name}_BINARY_TYPE}} STREQUAL "STATIC")
add_library({application.name} STATIC 
    ${{{application_caps_cmake_name}_ROOT_DIR}}/src/{application.name}.c
    #src_files
)


elseif(${{{application_caps_cmake_name}_BINARY_TYPE}} STREQUAL "SHARED")
add_library({application.name} SHARED 
    ${{{application_caps_cmake_name}_ROOT_DIR}}/src/{application.name}.c
    #src_files
)
target_compile_definitions(shengine PUBLIC 
    SH_APPLICATION_TARGET_TYPE_SHARED=1
)


elseif(${{{application_caps_cmake_name}_BINARY_TYPE}} STREQUAL "EXECUTABLE")
add_executable({application.name}  
    ${{{application_caps_cmake_name}_ROOT_DIR}}/src/{application.name}.c
    #src_files
    ${{SH_ENGINE_ROOT_DIR}}/collection/sheditor/src/shEditor.c
)
target_include_directories({application.name} PUBLIC 
    ${{SH_ENGINE_ROOT_DIR}}/collection/sheditor/include
)
target_compile_definitions({application.name} PUBLIC 
    SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1
)
endif()


target_include_directories({application.name} PUBLIC 
    ${{{application_caps_cmake_name}_ROOT_DIR}}/include
)

target_link_libraries({application.name} PUBLIC
    shengine
)

if (WIN32)
set_target_properties({application.name} sheditor PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY      ${{{application_caps_cmake_name}_BINARIES_DIR}}/windows
    RUNTIME_OUTPUT_DIRECTORY      ${{{application_caps_cmake_name}_BINARIES_DIR}}/windows
    VS_DEBUGGER_WORKING_DIRECTORY ${{{application_caps_cmake_name}_BINARIES_DIR}}/windows
)
else()
set_target_properties({application.name} sheditor PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY      ${{{application_caps_cmake_name}_BINARIES_DIR}}/linux 
    RUNTIME_OUTPUT_DIRECTORY      ${{{application_caps_cmake_name}_BINARIES_DIR}}/linux
    VS_DEBUGGER_WORKING_DIRECTORY ${{{application_caps_cmake_name}_BINARIES_DIR}}/linux
)
endif(WIN32)

set_shengine_collection_binaries()

endfunction()


function({application_underscore_name}_main)
build_{application.name}()
endfunction()



{application_underscore_name}_main()
"""



    cmake_file_path:str = f"{application.path}/CMakeLists.txt"


    print_separator()
    print(f"CMake file:\n{cmake_file}")
    print_separator()



    print_separator()
    print(f"Writing CMake file: {cmake_file_path}")
    dst_stream = open(cmake_file_path, "w")
    dst_stream.write(cmake_file)
    dst_stream.close()
    print_separator()



if __name__ == '__main__':
    main()
