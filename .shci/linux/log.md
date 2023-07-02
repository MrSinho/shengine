
# shengine

![linux-badge](exit_code.svg)

## [linux build logs:](https://github.com/mrsinho/shci)



Build ran for `71.55s`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y libvulkan-dev libvulkan1
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
Hit:3 http://ftp.debian.org/debian bullseye-backports InRelease
Hit:4 http://deb.debian.org/debian bullseye-updates InRelease
Reading package lists...
Building dependency tree...
Reading state information...
All packages are up to date.
Reading package lists...
Building dependency tree...
Reading state information...
The following additional packages will be installed:
  libdrm-amdgpu1 libdrm-common libdrm2 libllvm11 libwayland-client0
  libx11-xcb1 libxcb-dri3-0 libxcb-present0 libxcb-randr0 libxcb-sync1
  libxshmfence1 libz3-4 mesa-vulkan-drivers
The following NEW packages will be installed:
  libdrm-amdgpu1 libdrm-common libdrm2 libllvm11 libvulkan-dev libvulkan1
  libwayland-client0 libx11-xcb1 libxcb-dri3-0 libxcb-present0 libxcb-randr0
  libxcb-sync1 libxshmfence1 libz3-4 mesa-vulkan-drivers
0 upgraded, 15 newly installed, 0 to remove and 0 not upgraded.
Need to get 30.4 MB of archives.
After this operation, 139 MB of additional disk space will be used.
Get:1 http://deb.debian.org/debian bullseye/main amd64 libdrm-common all 2.4.104-1 [14.9 kB]
Get:2 http://security.debian.org/debian-security bullseye-security/main amd64 libx11-xcb1 amd64 2:1.7.2-1+deb11u1 [203 kB]
Get:3 http://deb.debian.org/debian bullseye/main amd64 libdrm2 amd64 2.4.104-1 [41.5 kB]
Get:4 http://deb.debian.org/debian bullseye/main amd64 libdrm-amdgpu1 amd64 2.4.104-1 [28.5 kB]
Get:5 http://deb.debian.org/debian bullseye/main amd64 libz3-4 amd64 4.8.10-1 [6,949 kB]
Get:6 http://deb.debian.org/debian bullseye/main amd64 libllvm11 amd64 1:11.0.1-2 [17.9 MB]
Get:7 http://deb.debian.org/debian bullseye/main amd64 libvulkan1 amd64 1.2.162.0-1 [103 kB]
Get:8 http://deb.debian.org/debian bullseye/main amd64 libvulkan-dev amd64 1.2.162.0-1 [586 kB]
Get:9 http://deb.debian.org/debian bullseye/main amd64 libwayland-client0 amd64 1.18.0-2~exp1.1 [26.9 kB]
Get:10 http://deb.debian.org/debian bullseye/main amd64 libxcb-dri3-0 amd64 1.14-3 [102 kB]
Get:11 http://deb.debian.org/debian bullseye/main amd64 libxcb-present0 amd64 1.14-3 [101 kB]
Get:12 http://deb.debian.org/debian bullseye/main amd64 libxcb-randr0 amd64 1.14-3 [113 kB]
Get:13 http://deb.debian.org/debian bullseye/main amd64 libxcb-sync1 amd64 1.14-3 [105 kB]
Get:14 http://deb.debian.org/debian bullseye/main amd64 libxshmfence1 amd64 1.3-1 [8,820 B]
Get:15 http://deb.debian.org/debian bullseye/main amd64 mesa-vulkan-drivers amd64 20.3.5-1 [4,086 kB]
Fetched 30.4 MB in 6s (5,466 kB/s)
Selecting previously unselected package libdrm-common.
(Reading database ... 
(Reading database ... 5%
(Reading database ... 10%
(Reading database ... 15%
(Reading database ... 20%
(Reading database ... 25%
(Reading database ... 30%
(Reading database ... 35%
(Reading database ... 40%
(Reading database ... 45%
(Reading database ... 50%
(Reading database ... 55%
(Reading database ... 60%
(Reading database ... 65%
(Reading database ... 70%
(Reading database ... 75%
(Reading database ... 80%
(Reading database ... 85%
(Reading database ... 90%
(Reading database ... 95%
(Reading database ... 100%
(Reading database ... 25256 files and directories currently installed.)
Preparing to unpack .../00-libdrm-common_2.4.104-1_all.deb ...
Unpacking libdrm-common (2.4.104-1) ...
Selecting previously unselected package libdrm2:amd64.
Preparing to unpack .../01-libdrm2_2.4.104-1_amd64.deb ...
Unpacking libdrm2:amd64 (2.4.104-1) ...
Selecting previously unselected package libdrm-amdgpu1:amd64.
Preparing to unpack .../02-libdrm-amdgpu1_2.4.104-1_amd64.deb ...
Unpacking libdrm-amdgpu1:amd64 (2.4.104-1) ...
Selecting previously unselected package libz3-4:amd64.
Preparing to unpack .../03-libz3-4_4.8.10-1_amd64.deb ...
Unpacking libz3-4:amd64 (4.8.10-1) ...
Selecting previously unselected package libllvm11:amd64.
Preparing to unpack .../04-libllvm11_1%3a11.0.1-2_amd64.deb ...
Unpacking libllvm11:amd64 (1:11.0.1-2) ...
Selecting previously unselected package libvulkan1:amd64.
Preparing to unpack .../05-libvulkan1_1.2.162.0-1_amd64.deb ...
Unpacking libvulkan1:amd64 (1.2.162.0-1) ...
Selecting previously unselected package libvulkan-dev:amd64.
Preparing to unpack .../06-libvulkan-dev_1.2.162.0-1_amd64.deb ...
Unpacking libvulkan-dev:amd64 (1.2.162.0-1) ...
Selecting previously unselected package libwayland-client0:amd64.
Preparing to unpack .../07-libwayland-client0_1.18.0-2~exp1.1_amd64.deb ...
Unpacking libwayland-client0:amd64 (1.18.0-2~exp1.1) ...
Selecting previously unselected package libx11-xcb1:amd64.
Preparing to unpack .../08-libx11-xcb1_2%3a1.7.2-1+deb11u1_amd64.deb ...
Unpacking libx11-xcb1:amd64 (2:1.7.2-1+deb11u1) ...
Selecting previously unselected package libxcb-dri3-0:amd64.
Preparing to unpack .../09-libxcb-dri3-0_1.14-3_amd64.deb ...
Unpacking libxcb-dri3-0:amd64 (1.14-3) ...
Selecting previously unselected package libxcb-present0:amd64.
Preparing to unpack .../10-libxcb-present0_1.14-3_amd64.deb ...
Unpacking libxcb-present0:amd64 (1.14-3) ...
Selecting previously unselected package libxcb-randr0:amd64.
Preparing to unpack .../11-libxcb-randr0_1.14-3_amd64.deb ...
Unpacking libxcb-randr0:amd64 (1.14-3) ...
Selecting previously unselected package libxcb-sync1:amd64.
Preparing to unpack .../12-libxcb-sync1_1.14-3_amd64.deb ...
Unpacking libxcb-sync1:amd64 (1.14-3) ...
Selecting previously unselected package libxshmfence1:amd64.
Preparing to unpack .../13-libxshmfence1_1.3-1_amd64.deb ...
Unpacking libxshmfence1:amd64 (1.3-1) ...
Selecting previously unselected package mesa-vulkan-drivers:amd64.
Preparing to unpack .../14-mesa-vulkan-drivers_20.3.5-1_amd64.deb ...
Unpacking mesa-vulkan-drivers:amd64 (20.3.5-1) ...
Setting up libxcb-dri3-0:amd64 (1.14-3) ...
Setting up libx11-xcb1:amd64 (2:1.7.2-1+deb11u1) ...
Setting up libxcb-present0:amd64 (1.14-3) ...
Setting up libz3-4:amd64 (4.8.10-1) ...
Setting up libllvm11:amd64 (1:11.0.1-2) ...
Setting up libxcb-sync1:amd64 (1.14-3) ...
Setting up libvulkan1:amd64 (1.2.162.0-1) ...
Setting up libxshmfence1:amd64 (1.3-1) ...
Setting up libxcb-randr0:amd64 (1.14-3) ...
Setting up libdrm-common (2.4.104-1) ...
Setting up libwayland-client0:amd64 (1.18.0-2~exp1.1) ...
Setting up libvulkan-dev:amd64 (1.2.162.0-1) ...
Setting up libdrm2:amd64 (2.4.104-1) ...
Setting up libdrm-amdgpu1:amd64 (2.4.104-1) ...
Setting up mesa-vulkan-drivers:amd64 (20.3.5-1) ...
Processing triggers for libc-bin (2.31-13+deb11u6) ...
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
-- Configuring incomplete, errors occurred!
See also "/mnt/d/home/desktop/GitHub/shengine/applications/triangle/linux/build/CMakeFiles/CMakeOutput.log".
See also "/mnt/d/home/desktop/GitHub/shengine/applications/triangle/linux/build/CMakeFiles/CMakeError.log".
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
-- Configuring incomplete, errors occurred!
See also "/mnt/d/home/desktop/GitHub/shengine/applications/noise/linux/build/CMakeFiles/CMakeOutput.log".
See also "/mnt/d/home/desktop/GitHub/shengine/applications/noise/linux/build/CMakeFiles/CMakeError.log".
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


```

---

    

Build terminated with exit code 2

---

