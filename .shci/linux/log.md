
# Linux build logs

![](linux-exit-code.svg)

```
system        : Linux
version       : #1 SMP Thu Oct 5 21:02:42 UTC 2023
platform      : Linux-5.15.133.1-microsoft-standard-WSL2-x86_64-with-glibc2.31
processor     : 
min frequency : 0.0 MHz
max frequency : 0.0 MHz
cores         : 4
```

build ran for `56.06 s` and terminated with exit code `0`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y libvulkan-dev libvulkan1
    sudo apt install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev
    sudo apt install -y libwayland-dev
    sudo apt install -y python3.9 python3-pip
    sudo apt install -y gcc cmake make libc6-dev
}

PREREQUISITES > .shci/linux/prerequisites-output.txt

```

```bash
Hit:1 http://security.debian.org/debian-security bullseye-security InRelease
Hit:2 http://deb.debian.org/debian bullseye InRelease
Hit:3 http://deb.debian.org/debian bullseye-updates InRelease
Hit:4 http://ftp.debian.org/debian bullseye-backports InRelease
Reading package lists...
Building dependency tree...
Reading state information...
39 packages can be upgraded. Run 'apt list --upgradable' to see them.
Reading package lists...
Building dependency tree...
Reading state information...
libvulkan-dev is already the newest version (1.2.162.0-1).
libvulkan1 is already the newest version (1.2.162.0-1).
0 upgraded, 0 newly installed, 0 to remove and 39 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxcursor-dev is already the newest version (1:1.2.0-2).
libxinerama-dev is already the newest version (2:1.1.4-2).
libxrandr-dev is already the newest version (2:1.5.1-1).
libx11-dev is already the newest version (2:1.7.2-1+deb11u2).
0 upgraded, 0 newly installed, 0 to remove and 39 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libwayland-dev is already the newest version (1.18.0-2~exp1.1).
0 upgraded, 0 newly installed, 0 to remove and 39 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
python3-pip is already the newest version (20.3.4-4+deb11u1).
python3.9 is already the newest version (3.9.2-1).
0 upgraded, 0 newly installed, 0 to remove and 39 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.18.4-2+deb11u1).
gcc is already the newest version (4:10.2.1-1).
make is already the newest version (4.3-4.1).
libc6-dev is already the newest version (2.31-13+deb11u7).
0 upgraded, 0 newly installed, 0 to remove and 39 not upgraded.

```

---
    
```bash
function BUILD {
	python3.9 export-application.py name=triangle target=EXECUTABLE path=applications/triangle
	python3.9 export-application.py name=noise target=EXECUTABLE path=applications/noise
	cd applications/triangle/build/linux
	cmake --build .
	cd ../../../noise/build/linux
	cmake --build .
}

BUILD > .shci/linux/build-output.txt

```

```bash
-- SH_ENGINE_VERSION       : pre-release
-- SH_ENGINE_ROOT_DIR      : /mnt/d/home/desktop/github/shengine
-- SH_ENGINE_BINARIES_DIR  : /mnt/d/home/desktop/github/shengine/bin
-- SH_APPLICATION_PATH     : /mnt/d/home/desktop/github/shengine/applications/triangle
-- SH_APPLICATION_NAME     : triangle
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: /usr/include
-- shvulkan message: Vulkan_LIBRARY:     /usr/lib/x86_64-linux-gnu/libvulkan.so
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including X11 support
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- SMD_VERSION       : pre-release
-- SMD_ROOT_DIR      : /mnt/d/home/desktop/github/shengine/collection/externals/smd
-- SMD_BINARIES_DIR  : /mnt/d/home/desktop/github/shengine/collection/externals/smd/bin
-- SMD_BUILD_EXAMPLE : OFF
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/github/shengine/applications/triangle/build/linux
--------------------------------------------------------------------------------
EXAMPLE CALL: python export-application.py name=noise target=SHARED
EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise
EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles
EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
platform:       linux
python_src_dir: /mnt/d/home/desktop/github/shengine
name:           triangle
path:           /mnt/d/home/desktop/github/shengine/applications/triangle
target_type:    EXECUTABLE
generator:      
clean-cache:    
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading /mnt/d/home/desktop/github/shengine/applications/triangle/source-files.txt
src files:
	src/triangle.c

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading /mnt/d/home/desktop/github/shengine/applications/triangle/libs.txt
additional libraries:
	
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
CMake subdirectories:
		

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
CMake file:
cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

	

project(triangle C)

option(SH_APPLICATION_BINARY_TYPE CACHE "EXECUTABLE")
if("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "STATIC")
    add_library(${SH_APPLICATION_NAME} STATIC 
        	/mnt/d/home/desktop/github/shengine/applications/triangle/src/triangle.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	/mnt/d/home/desktop/github/shengine/applications/triangle/src/triangle.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${SH_APPLICATION_NAME} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/
    )
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	/mnt/d/home/desktop/github/shengine/applications/triangle/src/triangle.c

        /mnt/d/home/desktop/github/shengine/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC /mnt/d/home/desktop/github/shengine/collection/sheditor/include)
    target_compile_definitions(${SH_APPLICATION_NAME} PUBLIC SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1)
    set_target_properties(${SH_APPLICATION_NAME} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/
    )
endif()

target_include_directories(${SH_APPLICATION_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    ${SH_APPLICATION_NAME} PUBLIC shengine 	
)

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
writing to CMake file: /mnt/d/home/desktop/github/shengine/applications/triangle/CMakeLists.txt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Build path: /mnt/d/home/desktop/github/shengine/applications/triangle/build/linux

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
creating build directory: /mnt/d/home/desktop/github/shengine/applications/triangle/build/linux
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
running command:
	cd /mnt/d/home/desktop/github/shengine/applications/triangle/build/linux && cmake /mnt/d/home/desktop/github/shengine -DSH_ENGINE_BUILD_EDITOR=ON -DSH_APPLICATION_PATH=/mnt/d/home/desktop/github/shengine/applications/triangle -DSH_APPLICATION_NAME=triangle -DSH_APPLICATION_BINARY_TYPE=EXECUTABLE 
--------------------------------------------------------------------------------

-- SH_ENGINE_VERSION       : pre-release
-- SH_ENGINE_ROOT_DIR      : /mnt/d/home/desktop/github/shengine
-- SH_ENGINE_BINARIES_DIR  : /mnt/d/home/desktop/github/shengine/bin
-- SH_APPLICATION_PATH     : /mnt/d/home/desktop/github/shengine/applications/noise
-- SH_APPLICATION_NAME     : noise
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: /usr/include
-- shvulkan message: Vulkan_LIBRARY:     /usr/lib/x86_64-linux-gnu/libvulkan.so
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including X11 support
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- SMD_VERSION       : pre-release
-- SMD_ROOT_DIR      : /mnt/d/home/desktop/github/shengine/collection/externals/smd
-- SMD_BINARIES_DIR  : /mnt/d/home/desktop/github/shengine/collection/externals/smd/bin
-- SMD_BUILD_EXAMPLE : OFF
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/github/shengine/applications/noise/build/linux
--------------------------------------------------------------------------------
EXAMPLE CALL: python export-application.py name=noise target=SHARED
EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise
EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles
EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
platform:       linux
python_src_dir: /mnt/d/home/desktop/github/shengine
name:           noise
path:           /mnt/d/home/desktop/github/shengine/applications/noise
target_type:    EXECUTABLE
generator:      
clean-cache:    
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading /mnt/d/home/desktop/github/shengine/applications/noise/source-files.txt
src files:
	src/noise.c

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading /mnt/d/home/desktop/github/shengine/applications/noise/libs.txt
additional libraries:
	
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
CMake subdirectories:
		

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
CMake file:
cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

	

project(noise C)

option(SH_APPLICATION_BINARY_TYPE CACHE "EXECUTABLE")
if("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "STATIC")
    add_library(${SH_APPLICATION_NAME} STATIC 
        	/mnt/d/home/desktop/github/shengine/applications/noise/src/noise.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	/mnt/d/home/desktop/github/shengine/applications/noise/src/noise.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${SH_APPLICATION_NAME} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/
    )
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	/mnt/d/home/desktop/github/shengine/applications/noise/src/noise.c

        /mnt/d/home/desktop/github/shengine/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC /mnt/d/home/desktop/github/shengine/collection/sheditor/include)
    target_compile_definitions(${SH_APPLICATION_NAME} PUBLIC SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1)
    set_target_properties(${SH_APPLICATION_NAME} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux/
    )
endif()

target_include_directories(${SH_APPLICATION_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    ${SH_APPLICATION_NAME} PUBLIC shengine 	
)

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
writing to CMake file: /mnt/d/home/desktop/github/shengine/applications/noise/CMakeLists.txt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Build path: /mnt/d/home/desktop/github/shengine/applications/noise/build/linux

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
creating build directory: /mnt/d/home/desktop/github/shengine/applications/noise/build/linux
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
running command:
	cd /mnt/d/home/desktop/github/shengine/applications/noise/build/linux && cmake /mnt/d/home/desktop/github/shengine -DSH_ENGINE_BUILD_EDITOR=ON -DSH_APPLICATION_PATH=/mnt/d/home/desktop/github/shengine/applications/noise -DSH_APPLICATION_NAME=noise -DSH_APPLICATION_BINARY_TYPE=EXECUTABLE 
--------------------------------------------------------------------------------

[  2%] Built target smd
[  4%] Built target shvulkan
[  7%] Built target shenvironment
[  9%] Built target shthreads
[ 11%] Built target shapplication-host
[ 12%] Built target plyimporter
[ 14%] Built target shserial
[ 17%] Built target shgui
[ 39%] Built target glfw
[ 80%] Built target cglm
[ 88%] Built target shengine
[ 90%] Built target sheditor
[ 92%] Built target shgui-app
[ 94%] Built target native-export
[ 96%] Built target shgui-export-shaders
[ 97%] Linking C executable ../../../bin/linux/triangle
[100%] Built target triangle
[  2%] Built target smd
[  4%] Built target shvulkan
[  7%] Built target shenvironment
[  9%] Built target shthreads
[ 11%] Built target shapplication-host
[ 13%] Built target plyimporter
[ 15%] Built target shserial
[ 18%] Built target shgui
[ 40%] Built target glfw
[ 82%] Built target cglm
[ 90%] Built target shengine
[ 92%] Built target sheditor
[ 93%] Built target shgui-app
[ 95%] Built target native-export
[ 97%] Built target shgui-export-shaders
[100%] Built target noise

```

---
    
build ran for `56.06 s` and terminated with exit code `0`

---

