import sys
import os
import pathlib
import platform

#optional last args: ../simulations Generator-name
def main():#example call: python export_simulation.py simulation-sample SHARED

    print("EXAMPLE CALL: python export_simulation.py noise SHARED")
    print("EXAMPLE CALL: python export_simulation.py noise STATIC ../simulations")
    #CMAKE STANDARD GENERATOR NAMES BUT REPLACE " " with "-"
    print("EXAMPLE CALL: python export_simulation.py noise SHARED ../simulations MinGW-Makefiles")

    python_src_dir:str  = str(pathlib.Path(__file__).parent.resolve().as_posix())
    simulation_name:str = str(sys.argv[1])
    target_type:str     = str(sys.argv[2])
    os_name:str         = platform.system().lower()

    simulation_path:str = f"{python_src_dir}/simulations/{simulation_name}"
    if len(sys.argv) >= 4:
        simulation_path = python_src_dir + "/" + str(sys.argv[3])

    src_files_path:str =  simulation_path
    
    print(f"loading {src_files_path}/source-files.txt")
    src_stream = open(src_files_path + "/source-files.txt", "r")
    src = src_stream.read()
    src_stream.close()
    print(src)
    
    print(f"loading {src_files_path}/libs.txt")
    libs_stream = open(src_files_path + "/libs.txt", "r")
    libs_data = libs_stream.read()
    libs_stream.close()
    print(libs_data)
    
    libs_arr = libs_data.split()
    libs:str = "\t"
    libs_directories = []
    for i in range (0, len(libs_arr), 2):
        libs += f"{libs_arr[i]} "
        libs_directories.append(libs_arr[i+1])

    cmake_subdirectories:str = "\t"
    for i in range (0, len(libs_directories), 1):
        cmake_subdirectories += f"if (NOT TARGET {libs_directories[i]})\nadd_subdirectory({libs_directories[i]} [EXCLUDE_FROM_ALL])\nendif()\n"
    print(f"subdirectories:\n\t{cmake_subdirectories}\n")
    
    src_arr       = src.split()
    src_files:str = "\t"
    for i in range (0, len(src_arr), 1):
        src_files += f"{src_files_path}/{src_arr[i]}\n"

    cmake_file:str = f"""cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

{cmake_subdirectories}

project({simulation_name} C)

option(SH_SIMULATION_BINARY_TYPE CACHE "EXECUTABLE")
if("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "STATIC")
    add_library(${{SH_SIMULATION_NAME}} STATIC 
        {src_files}
    )
    
elseif("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "SHARED")
    add_library(${{SH_SIMULATION_NAME}} SHARED 
        {src_files}
    )
    target_compile_definitions(shengine PUBLIC SH_SIMULATION_TARGET_TYPE_SHARED=1)
elseif("${{SH_SIMULATION_BINARY_TYPE}}" STREQUAL "EXECUTABLE")
    add_executable(${{SH_SIMULATION_NAME}}  
        {src_files}
        {python_src_dir}/shengine/sheditor/src/shEditor.c
    )
    target_include_directories(${{SH_SIMULATION_NAME}} PUBLIC {python_src_dir}/shengine/sheditor/include)
    target_compile_definitions(${{SH_SIMULATION_NAME}} PUBLIC SH_SIMULATION_TARGET_TYPE_EXECUTABLE=1)
endif()

target_include_directories(${{SH_SIMULATION_NAME}} PUBLIC 
    ${{CMAKE_CURRENT_SOURCE_DIR}}/include
)
target_link_libraries(
    ${{SH_SIMULATION_NAME}} PUBLIC shengine {libs}
)
set_target_properties(${{SH_SIMULATION_NAME}} PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/{os_name}/bin/ 
    RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/{os_name}/bin/
    VS_DEBUGGER_WORKING_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/{os_name}/bin/
)
"""
    print(cmake_file)


    dst_stream = open(f"{simulation_path}/CMakeLists.txt", "w")
    dst_stream.write(cmake_file)
    dst_stream.close()

    build_path:str = f"{simulation_path}/{os_name}/build"
    print(f"Build path: {build_path}")
    os.system(f"mkdir \"{build_path}\"")
    cmd  = f"cd {build_path} && cmake {python_src_dir} "
    cmd += f"-DSH_ENGINE_BUILD_EDITOR=ON "
    cmd += f"-DSIMULATION_PATH={simulation_path} "#for other CMAKE files
    cmd += f"-DSH_SIMULATION_NAME={simulation_name} "#to generate solution with sim name
    cmd += f"-DSH_SIMULATION_BINARY_TYPE={target_type} " #could be STATIC SHARED EXECUTABLE

    if (len(sys.argv) >= 5):
        cmd += "-G\"" + str(sys.argv[4]).replace("-", " ") + "\""

    print(f"running command {cmd}")
    os.system(cmd)

if __name__ == '__main__':
    main()