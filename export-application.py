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



def readArg(arg:str, application:Application):
    if (arg.startswith("name=")):
        application.name = arg.removeprefix("name=")
    elif (arg.startswith("target=")):
        application.target_type = arg.removeprefix("target=")
    elif (arg.startswith("path=")):
        application.path = arg.removeprefix("path=")
    elif (arg.startswith("generator=")):
        application.generator = arg.removeprefix("generator=").replace("-", " ")
    elif (arg.startswith("CLEAN-CACHE")):
        application.clean_cache = arg


def main():

    print("--------------------------------------------------------------------------------")
    print("EXAMPLE CALL: python export-application.py name=noise target=SHARED")
    print("EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise")
    print("EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles")#CMAKE STANDARD GENERATOR NAMES BUT REPLACE " " with "-"
    print("EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE")
    print("--------------------------------------------------------------------------------\n")

    

    python_src_dir:str = str(pathlib.Path(__file__).parent.resolve().as_posix())
    os_name:str        = platform.system().lower()


    application:Application = Application()
    for i in range (0, len(sys.argv), 1):
        readArg(str(sys.argv[i]), application)
    if (application.path == ""):
        application.path = f"{python_src_dir}/applications/{application.name}"
    else:
        application.path = f"{python_src_dir}/{application.path}"


    print("--------------------------------------------------------------------------------")
    print(f"platform:       {os_name}")
    print(f"python_src_dir: {python_src_dir}")
    print(f"name:           {application.name}")
    print(f"path:           {application.path}")
    print(f"target_type:    {application.target_type}")
    print(f"generator:      {application.generator}")
    print(f"clean-cache:    {application.clean_cache}");
    print("--------------------------------------------------------------------------------\n")


    src_files_path = application.path
    
    print("--------------------------------------------------------------------------------")
    print(f"loading {src_files_path}/source-files.txt")
    src_stream = open(src_files_path + "/source-files.txt", "r")
    src = src_stream.read()
    src_stream.close()
    print(f"src files:\n\t{src}\n")
    print("--------------------------------------------------------------------------------\n")


    print("--------------------------------------------------------------------------------")
    print(f"loading {src_files_path}/libs.txt")
    libs_stream = open(src_files_path + "/libs.txt", "r")
    libs_data = libs_stream.read()
    libs_stream.close()
    print(f"additional libraries:\n\t{libs_data}")
    print("--------------------------------------------------------------------------------\n")
    

    libs_arr = libs_data.split()
    libs:str = "\t"
    libs_directories = []
    for i in range (0, len(libs_arr), 2):
        libs += f"{libs_arr[i]} "
        libs_directories.append(libs_arr[i+1])

    cmake_subdirectories:str = "\t"
    for i in range (0, len(libs_directories), 1):
        cmake_subdirectories += f"if (NOT TARGET {libs_directories[i]})\nadd_subdirectory({libs_directories[i]} [EXCLUDE_FROM_ALL])\nendif()\n"
    print("--------------------------------------------------------------------------------")
    print(f"CMake subdirectories:\n\t{cmake_subdirectories}\n")
    print("--------------------------------------------------------------------------------\n")
    

    src_arr       = src.split()
    src_files:str = "\t"
    for i in range (0, len(src_arr), 1):
        src_files += f"{src_files_path}/{src_arr[i]}\n"

    cmake_file:str = f"""cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

{cmake_subdirectories}

project({application.name} C)

option(SH_APPLICATION_BINARY_TYPE CACHE "EXECUTABLE")
if("${{SH_APPLICATION_BINARY_TYPE}}" STREQUAL "STATIC")
    add_library(${{SH_APPLICATION_NAME}} STATIC 
        {src_files}
    )
    
elseif("${{SH_APPLICATION_BINARY_TYPE}}" STREQUAL "SHARED")
    add_library(${{SH_APPLICATION_NAME}} SHARED 
        {src_files}
    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${{SH_APPLICATION_NAME}} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/bin/{os_name}/ 
        RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/bin/{os_name}/
        VS_DEBUGGER_WORKING_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/bin/{os_name}/
    )
elseif("${{SH_APPLICATION_BINARY_TYPE}}" STREQUAL "EXECUTABLE")
    add_executable(${{SH_APPLICATION_NAME}}  
        {src_files}
        {python_src_dir}/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${{SH_APPLICATION_NAME}} PUBLIC {python_src_dir}/collection/sheditor/include)
    target_compile_definitions(${{SH_APPLICATION_NAME}} PUBLIC SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1)
    set_target_properties(${{SH_APPLICATION_NAME}} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/bin/{os_name} 
        RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/bin/{os_name}
        VS_DEBUGGER_WORKING_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}}/bin/{os_name}/
    )
endif()

target_include_directories(${{SH_APPLICATION_NAME}} PUBLIC 
    ${{CMAKE_CURRENT_SOURCE_DIR}}/include
)
target_link_libraries(
    ${{SH_APPLICATION_NAME}} PUBLIC shengine {libs}
)
"""
    print("--------------------------------------------------------------------------------")
    print(f"CMake file:\n{cmake_file}")
    print("--------------------------------------------------------------------------------\n")

    cmake_file_path:str = f"{application.path}/CMakeLists.txt"
    print("--------------------------------------------------------------------------------")
    print(f"writing to CMake file: {cmake_file_path}")
    dst_stream = open(cmake_file_path, "w")
    dst_stream.write(cmake_file)
    dst_stream.close()
    print("--------------------------------------------------------------------------------\n")

    build_path:str = f"{application.path}/build/{os_name}"
    print("--------------------------------------------------------------------------------")
    print(f"Build path: {build_path}\n")
    print("--------------------------------------------------------------------------------\n")
    
    print("--------------------------------------------------------------------------------")
    print(f"creating build directory: {build_path}")
    os.system(f"mkdir \"{application.path}/build\"")
    os.system(f"mkdir \"{build_path}\"")
    cmd  = f"cd {build_path} "
    print("--------------------------------------------------------------------------------\n")


    if (application.clean_cache == "CLEAN-CACHE"):
        if (os_name == "windows"):
            cmd += "&& del CMakeCache.txt "
        else:
            cmd += "&& rm CMakeCache.txt "


    cmd += f"&& cmake {python_src_dir} "
    cmd += f"-DSH_ENGINE_BUILD_EDITOR=ON "
    cmd += f"-DSH_APPLICATION_PATH={application.path} "#for other CMAKE files
    cmd += f"-DSH_APPLICATION_NAME={application.name} "#to generate solution with application name
    cmd += f"-DSH_APPLICATION_BINARY_TYPE={application.target_type} "#could be STATIC SHARED EXECUTABLE

    if (application.generator != ""):
            cmd += f"-G\"{application.generator}\""

    print("--------------------------------------------------------------------------------")
    print(f"running command:\n\t{cmd}")
    print("--------------------------------------------------------------------------------\n")

    os.system(cmd)

if __name__ == '__main__':
    main()
