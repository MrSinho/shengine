import sys
import os

def main():#example call: python export_simulation.py example SHARED example/source_files.txt

    src_stream = open(str(sys.argv[3]), "r")
    src = src_stream.read()
    src_stream.close()

    print(src)

    cmake_file = f"""
cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

option(SH_SIMULATION_NAME EmptyProject)
project(${{SH_SIMULATION_NAME}})

option(SH_SIMULATION_BINARY_TYPE "EXECUTABLE")
if("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "STATIC")
    add_library(${{SH_SIMULATION_NAME}} STATIC 
    {src}
)
elseif("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "SHARED")
    add_library(${{SH_SIMULATION_NAME}} SHARED 
    {src}
)
elseif("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "EXECUTABLE")
    add_executable(${{SH_SIMULATION_NAME}}  
    {src}
)
endif()

target_link_libraries(${{SH_SIMULATION_NAME}} PUBLIC shengine)
set_target_properties(${{SH_SIMULATION_NAME}} PROPERTIES 
ARCHIVE_OUTPUT_DIRECTORY ${{CMAKE_SOURCE_DIR}}/bin  
RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_SOURCE_DIR}}/bin
)
"""
    print(cmake_file)

    dst_stream = open("simulations/CMakeLists.txt", "w")
    dst_stream.write(cmake_file)
    dst_stream.close()

    
    cmd = "mkdir build && cd build && cmake .. -DBUILD_SH_ENGINE=ON -DBUILD_SIMULATIONS=ON -DSH_ENGINE_BUILD_EDITOR=ON -DSH_SIMULATION_NAME="
    cmd += str(sys.argv[1])
    cmd += " -DSH_SIMULATION_BINARY_TYPE=" #could be STATIC SHARED EXECUTABLE
    cmd += str(sys.argv[2])
    print(f"running command {cmd}")
    os.system(cmd)

if __name__ == '__main__':
    main()