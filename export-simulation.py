import sys
import os
import pathlib

def main():#example call: python export_simulation.py simulation-sample SHARED;;;; optional last arg: ../simulation-sample
    
    python_src_dir = str(pathlib.Path(__file__).parent.resolve().as_posix())

    src_path = "."
    if len(sys.argv) == 4:
        src_path = python_src_dir + "/" + str(sys.argv[3])
    else:
        src_path = python_src_dir + "/simulations/" + str(sys.argv[1])

    print(f"loading {src_path}/source-files.txt")
    src_stream = open(src_path + "/source-files.txt", "r")
    src = src_stream.read()
    src_stream.close()

    print(src)

    cmake_file = f"""
cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

option(SH_SIMULATION_NAME emptytarget)
project(${{SH_SIMULATION_NAME}})

option(SH_SIMULATION_BINARY_TYPE "EXECUTABLE")
if("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "STATIC")
    add_library(${{SH_SIMULATION_NAME}} STATIC 
    {src_path}/{src}
)
elseif("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "SHARED")
    add_library(${{SH_SIMULATION_NAME}} SHARED 
    {src_path}/{src}
)
elseif("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "EXECUTABLE")
    add_executable(${{SH_SIMULATION_NAME}}  
    {src_path}/{src}
)
endif()
target_include_directories(${{SH_SIMULATION_NAME}} PUBLIC 
${{CMAKE_CURRENT_SOURCE_DIR}}/${{SH_SIMULATION_NAME}}/include
)
target_link_libraries(${{SH_SIMULATION_NAME}} PUBLIC shengine)
set_target_properties(${{SH_SIMULATION_NAME}} PROPERTIES 
ARCHIVE_OUTPUT_DIRECTORY ${{CMAKE_SOURCE_DIR}}/bin  
RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_SOURCE_DIR}}/bin
)
"""
    print(cmake_file)

    dst_stream = open(src_path.rsplit("/", 1)[0]+"/CMakeLists.txt", "w")
    dst_stream.write(cmake_file)
    dst_stream.close()

    os.system("mkdir build")
    cmd = "cd build && cmake .. -DBUILD_SH_ENGINE=ON -DSH_ENGINE_BUILD_EDITOR=ON -DBUILD_SIMULATIONS=ON -DSH_SIMULATION_NAME="
    cmd += str(sys.argv[1])
    cmd += " -DSH_SIMULATION_BINARY_TYPE=" #could be STATIC SHARED EXECUTABLE
    cmd += str(sys.argv[2])
    print(f"running command {cmd}")
    os.system(cmd)

if __name__ == '__main__':
    main()