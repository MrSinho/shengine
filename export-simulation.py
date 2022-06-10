import sys
import os
import pathlib

def main():#example call: python export_simulation.py simulation-sample SHARED;;;; optional last arg: ../simulations
    
    python_src_dir = str(pathlib.Path(__file__).parent.resolve().as_posix())

    simulation_path = python_src_dir + "/simulations"
    if len(sys.argv) == 4:
        simulation_path = python_src_dir + "/" + str(sys.argv[3])

    src_path =  simulation_path + "/" + str(sys.argv[1])
    
    print(f"loading {src_path}/source-files.txt")
    src_stream = open(src_path + "/source-files.txt", "r")
    src = src_stream.read()
    src_stream.close()
    print(src)
    
    print(f"loading {src_path}/libs.txt")
    libs_stream = open(src_path + "/libs.txt", "r")
    libs_data = libs_stream.read()
    libs_stream.close()
    print(libs_data)
    
    libs_arr = libs_data.split()
    libs = "\t"
    libs_dir = []
    for i in range (0, len(libs_arr), 2):
        libs += f"{libs_arr[i]} "
        libs_dir.append(libs_arr[i+1])

    cmake_subdirectories = "\t"
    for i in range (0, len(libs_dir), 1):
        cmake_subdirectories += f"add_subdirectory({libs_dir[i]} [EXCLUDE_FROM_ALL])\n"
    print(f"subdirectories:\n\t{cmake_subdirectories}\n")
    
    cmake_file = f"""cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

{cmake_subdirectories}

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
target_link_libraries(${{SH_SIMULATION_NAME}} PUBLIC shengine {libs})
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
    cmd = f"cd {python_src_dir} && mkdir build"
    os.system(cmd)
    cmd = f"cd {python_src_dir}/build && cmake .. -DSH_ENGINE_BUILD_EDITOR=ON -DSIMULATION_PATH={simulation_path} -DSH_EDITOR_ASSETS_PATH={src_path}/assets/ -DSH_SIMULATION_NAME="
    cmd += str(sys.argv[1])
    cmd += " -DSH_SIMULATION_BINARY_TYPE=" #could be STATIC SHARED EXECUTABLE
    cmd += str(sys.argv[2])
    print(f"running command {cmd}")
    os.system(cmd)

if __name__ == '__main__':
    main()