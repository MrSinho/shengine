
# shengine

![linux-badge](exit_code.svg)

## [linux build logs:](https://github.com/mrsinho/shci)



Build ran for `383.12s`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y libvulkan-dev libvulkan1
    sudo apt install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev
    sudo apt install -y libwayland-dev
    sudo apt install -y python3.9 python3-pip
    sudo apt install -y gcc cmake make libc6-dev
    cd requirements
    sudo bash debian-requirements.sh
}

PREREQUISITES > .shci/linux/prerequisites-output.txt

```

```bash
Hit:1 http://deb.debian.org/debian bullseye InRelease
Hit:2 http://security.debian.org/debian-security bullseye-security InRelease
Hit:3 http://deb.debian.org/debian bullseye-updates InRelease
Hit:4 http://ftp.debian.org/debian bullseye-backports InRelease
Reading package lists...
Building dependency tree...
Reading state information...
All packages are up to date.
Reading package lists...
Building dependency tree...
Reading state information...
libvulkan-dev is already the newest version (1.2.162.0-1).
libvulkan1 is already the newest version (1.2.162.0-1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxcursor-dev is already the newest version (1:1.2.0-2).
libxinerama-dev is already the newest version (2:1.1.4-2).
libxrandr-dev is already the newest version (2:1.5.1-1).
libx11-dev is already the newest version (2:1.7.2-1+deb11u1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libwayland-dev is already the newest version (1.18.0-2~exp1.1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
python3-pip is already the newest version (20.3.4-4+deb11u1).
python3.9 is already the newest version (3.9.2-1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.18.4-2+deb11u1).
gcc is already the newest version (4:10.2.1-1).
libc6-dev is already the newest version (2.31-13+deb11u6).
make is already the newest version (4.3-4.1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
Reading package lists...
Building dependency tree...
Reading state information...
Reading package lists...
Building dependency tree...
Reading state information...

```

---

    
```bash
function BUILD {
	python3.9 export-application.py name=triangle target=EXECUTABLE path=applications/triangle
	python3.9 export-application.py name=noise target=EXECUTABLE path=applications/noise
	cd applications/triangle/linux/build
	cmake --build .
	cd ../../../noise/linux/build
	cmake --build .
}

BUILD > .shci/linux/build-output.txt

```

```bash
-- The C compiler identification is GNU 10.2.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Found Vulkan: /usr/lib/x86_64-linux-gnu/libvulkan.so  
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: /usr/include
-- shvulkan message: Vulkan_LIBRARY:     /usr/lib/x86_64-linux-gnu/libvulkan.so
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE  
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including X11 support
-- Found X11: /usr/include   
-- Looking for XOpenDisplay in /usr/lib/x86_64-linux-gnu/libX11.so;/usr/lib/x86_64-linux-gnu/libXext.so
-- Looking for XOpenDisplay in /usr/lib/x86_64-linux-gnu/libX11.so;/usr/lib/x86_64-linux-gnu/libXext.so - found
-- Looking for gethostbyname
-- Looking for gethostbyname - found
-- Looking for connect
-- Looking for connect - found
-- Looking for remove
-- Looking for remove - found
-- Looking for shmat
-- Looking for shmat - found
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/GitHub/shengine/applications/triangle/linux/build
--------------------------------------------------------------------------------
EXAMPLE CALL: python export-application.py name=noise target=SHARED
EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise
EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles
EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
platform:       linux
python_src_dir: /mnt/d/home/desktop/GitHub/shengine
name:           triangle
path:           /mnt/d/home/desktop/GitHub/shengine/applications/triangle
target_type:    EXECUTABLE
generator:      
clean-cache:    
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading /mnt/d/home/desktop/GitHub/shengine/applications/triangle/source-files.txt
src files:
	src/triangle.c

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading /mnt/d/home/desktop/GitHub/shengine/applications/triangle/libs.txt
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
        	/mnt/d/home/desktop/GitHub/shengine/applications/triangle/src/triangle.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	/mnt/d/home/desktop/GitHub/shengine/applications/triangle/src/triangle.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${SH_APPLICATION_NAME} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/
    )
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	/mnt/d/home/desktop/GitHub/shengine/applications/triangle/src/triangle.c

        /mnt/d/home/desktop/GitHub/shengine/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC /mnt/d/home/desktop/GitHub/shengine/collection/sheditor/include)
    target_compile_definitions(${SH_APPLICATION_NAME} PUBLIC SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1)
    set_target_properties(${SH_APPLICATION_NAME} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/
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
writing to CMake file: /mnt/d/home/desktop/GitHub/shengine/applications/triangle/CMakeLists.txt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Os path: /mnt/d/home/desktop/GitHub/shengine/applications/triangle/linux
Build path: /mnt/d/home/desktop/GitHub/shengine/applications/triangle/linux/build

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
creating build directory: /mnt/d/home/desktop/GitHub/shengine/applications/triangle/linux/build
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
running command:
	cd /mnt/d/home/desktop/GitHub/shengine/applications/triangle/linux/build && cmake /mnt/d/home/desktop/GitHub/shengine -DSH_ENGINE_BUILD_EDITOR=ON -DSH_APPLICATION_PATH=/mnt/d/home/desktop/GitHub/shengine/applications/triangle -DSH_APPLICATION_NAME=triangle -DSH_APPLICATION_BINARY_TYPE=EXECUTABLE 
--------------------------------------------------------------------------------

-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: /usr/include
-- shvulkan message: Vulkan_LIBRARY:     /usr/lib/x86_64-linux-gnu/libvulkan.so
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including X11 support
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/GitHub/shengine/applications/noise/linux/build
--------------------------------------------------------------------------------
EXAMPLE CALL: python export-application.py name=noise target=SHARED
EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise
EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles
EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
platform:       linux
python_src_dir: /mnt/d/home/desktop/GitHub/shengine
name:           noise
path:           /mnt/d/home/desktop/GitHub/shengine/applications/noise
target_type:    EXECUTABLE
generator:      
clean-cache:    
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading /mnt/d/home/desktop/GitHub/shengine/applications/noise/source-files.txt
src files:
	src/noise.c

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading /mnt/d/home/desktop/GitHub/shengine/applications/noise/libs.txt
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
        	/mnt/d/home/desktop/GitHub/shengine/applications/noise/src/noise.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	/mnt/d/home/desktop/GitHub/shengine/applications/noise/src/noise.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${SH_APPLICATION_NAME} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/
    )
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	/mnt/d/home/desktop/GitHub/shengine/applications/noise/src/noise.c

        /mnt/d/home/desktop/GitHub/shengine/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC /mnt/d/home/desktop/GitHub/shengine/collection/sheditor/include)
    target_compile_definitions(${SH_APPLICATION_NAME} PUBLIC SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1)
    set_target_properties(${SH_APPLICATION_NAME} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/linux/bin/
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
writing to CMake file: /mnt/d/home/desktop/GitHub/shengine/applications/noise/CMakeLists.txt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Os path: /mnt/d/home/desktop/GitHub/shengine/applications/noise/linux
Build path: /mnt/d/home/desktop/GitHub/shengine/applications/noise/linux/build

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
creating build directory: /mnt/d/home/desktop/GitHub/shengine/applications/noise/linux/build
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
running command:
	cd /mnt/d/home/desktop/GitHub/shengine/applications/noise/linux/build && cmake /mnt/d/home/desktop/GitHub/shengine -DSH_ENGINE_BUILD_EDITOR=ON -DSH_APPLICATION_PATH=/mnt/d/home/desktop/GitHub/shengine/applications/noise -DSH_APPLICATION_NAME=noise -DSH_APPLICATION_BINARY_TYPE=EXECUTABLE 
--------------------------------------------------------------------------------

[  2%] Built target smd
[  4%] Built target shvulkan
[  7%] Built target shenvironment
[ 10%] Built target shsharedhost
[ 12%] Built target shserial
[ 14%] Built target shthreads
[ 41%] Built target glfw
[ 84%] Built target cglm
[ 92%] Built target shengine
[ 94%] Built target sheditor
[ 96%] Built target plyimporter
[100%] Built target triangle
Scanning dependencies of target smd
[  1%] Building C object collection/externals/smd/CMakeFiles/smd.dir/smd/src/smd.c.o
[  2%] Linking C static library ../../../../../../../bin/libsmd.a
[  2%] Built target smd
Scanning dependencies of target shvulkan
[  3%] Building C object collection/externals/shvulkan/CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVulkan.c.o
[  4%] Linking C static library ../../../../../../../bin/libshvulkan.a
[  4%] Built target shvulkan
Scanning dependencies of target shenvironment
[  5%] Building C object collection/CMakeFiles/shenvironment.dir/shenvironment/src/shEnvironment.c.o
[  6%] Building C object collection/CMakeFiles/shenvironment.dir/shenvironment/src/shFileUtilities.c.o
[  7%] Linking C static library ../../../../../bin/libshenvironment.a
[  7%] Built target shenvironment
Scanning dependencies of target shsharedhost
[  8%] Building C object collection/CMakeFiles/shsharedhost.dir/shsharedhost/src/shSharedHost.c.o
[ 10%] Linking C static library libshsharedhost.a
[ 10%] Built target shsharedhost
Scanning dependencies of target shserial
[ 11%] Building C object collection/externals/shserial/CMakeFiles/shserial.dir/ShSerial/src/shserial/shSerial.c.o
[ 12%] Linking C static library ../../../../../../../collection/externals/shserial/bin/libshserial.a
[ 12%] Built target shserial
Scanning dependencies of target shthreads
[ 13%] Building C object collection/externals/shthreads/CMakeFiles/shthreads.dir/shthreads/src/shthreads.c.o
[ 14%] Linking C static library ../../../../../../../bin/libshthreads.a
[ 14%] Built target shthreads
Scanning dependencies of target glfw
[ 15%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/context.c.o
[ 16%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/init.c.o
[ 17%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/input.c.o
[ 18%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/monitor.c.o
[ 20%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/platform.c.o
[ 21%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/vulkan.c.o
[ 22%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/window.c.o
[ 23%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/egl_context.c.o
[ 24%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/osmesa_context.c.o
[ 25%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/null_init.c.o
[ 26%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/null_monitor.c.o
[ 27%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/null_window.c.o
[ 28%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/null_joystick.c.o
[ 30%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/posix_module.c.o
[ 31%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/posix_time.c.o
[ 32%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/posix_thread.c.o
[ 33%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/x11_init.c.o
[ 34%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/x11_monitor.c.o
[ 35%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/x11_window.c.o
[ 36%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/xkb_unicode.c.o
[ 37%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/glx_context.c.o
[ 38%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/linux_joystick.c.o
[ 40%] Building C object collection/externals/glfw/src/CMakeFiles/glfw.dir/posix_poll.c.o
[ 41%] Linking C static library libglfw3.a
[ 41%] Built target glfw
Scanning dependencies of target cglm
[ 42%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/euler.c.o
[ 43%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/affine.c.o
[ 44%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/io.c.o
[ 45%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/quat.c.o
[ 46%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/cam.c.o
[ 47%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/vec2.c.o
[ 48%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/ivec2.c.o
[ 50%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/vec3.c.o
[ 51%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/ivec3.c.o
[ 52%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/vec4.c.o
[ 53%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/ivec4.c.o
[ 54%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/mat2.c.o
[ 55%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/mat3.c.o
[ 56%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/mat4.c.o
[ 57%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/plane.c.o
[ 58%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/frustum.c.o
[ 60%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/box.c.o
[ 61%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/project.c.o
[ 62%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/sphere.c.o
[ 63%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/ease.c.o
[ 64%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/curve.c.o
[ 65%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/bezier.c.o
[ 66%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/ray.c.o
[ 67%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/affine2d.c.o
[ 68%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_lh_no.c.o
[ 70%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_lh_zo.c.o
[ 71%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_rh_no.c.o
[ 72%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_rh_zo.c.o
[ 73%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_lh_no.c.o
[ 74%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_lh_zo.c.o
[ 75%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_rh_no.c.o
[ 76%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_rh_zo.c.o
[ 77%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_lh_no.c.o
[ 78%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_lh_zo.c.o
[ 80%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_rh_no.c.o
[ 81%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_rh_zo.c.o
[ 82%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/project_no.c.o
[ 83%] Building C object collection/externals/cglm/CMakeFiles/cglm.dir/src/clipspace/project_zo.c.o
[ 84%] Linking C shared library libcglm.so
[ 84%] Built target cglm
Scanning dependencies of target shengine
[ 85%] Building C object collection/CMakeFiles/shengine.dir/shengine/src/shEngine.c.o
[ 86%] Building C object collection/CMakeFiles/shengine.dir/shengine/src/shInput.c.o
[ 87%] Building C object collection/CMakeFiles/shengine.dir/shengine/src/shTime.c.o
[ 88%] Building C object collection/CMakeFiles/shengine.dir/shengine/src/shWindow.c.o
[ 90%] Building C object collection/CMakeFiles/shengine.dir/shengine/src/shLinearAlgebra.c.o
[ 91%] Building C object collection/CMakeFiles/shengine.dir/shengine/src/shScene.c.o
[ 92%] Linking C static library ../../../../../bin/libshengine.a
[ 92%] Built target shengine
Scanning dependencies of target sheditor
[ 93%] Building C object collection/CMakeFiles/sheditor.dir/sheditor/src/shEditor.c.o
[ 94%] Linking C executable ../../../../../bin/sheditor
[ 94%] Built target sheditor
Scanning dependencies of target plyimporter
[ 95%] Building C object collection/externals/plyimporter/CMakeFiles/plyimporter.dir/PlyImporter/src/plyimporter/plyImporter.c.o
[ 96%] Linking C static library ../../../../../../../bin/libplyimporter.a
[ 96%] Built target plyimporter
Scanning dependencies of target noise
[ 97%] Building C object [EXCLUDE_FROM_ALL]/CMakeFiles/noise.dir/src/noise.c.o
[ 98%] Building C object [EXCLUDE_FROM_ALL]/CMakeFiles/noise.dir/__/__/collection/sheditor/src/shEditor.c.o
[100%] Linking C executable ../../bin/noise
[100%] Built target noise

```

---

    

Build terminated with exit code 0

---

