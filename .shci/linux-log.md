# shengine
![linux_badge](linux-status.svg)
## [Linux/Unix build logs:](https://github.com/MrSinho/ShCI)
  `Build ran for 25.62s`
```bash $shci call$
apt install -y wget gcc cmake make gdb
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.16.3-1ubuntu1).
gcc is already the newest version (4:9.3.0-1ubuntu2).
make is already the newest version (4.2.1-1.2).
gdb is already the newest version (9.2-0ubuntu1~20.04.1).
wget is already the newest version (1.20.3-1ubuntu2).
0 upgraded, 0 newly installed, 0 to remove and 419 not upgraded.

```

```bash $shci call$
sudo apt update -y
sudo apt install -y libx11-dev doxygen libxrandr-dev libusb-dev
sudo apt install -y libxinerama-dev libxcursor-dev libxi-dev

sudo add-apt-repository -y ppa:oibaf/graphics-drivers
sudo apt install -y vulkan libvulkan-dev libvulkan1 mesa-vulkan-drivers vulkan-utils

Hit:1 http://packages.microsoft.com/repos/code stable InRelease
Hit:2 http://archive.canonical.com/ubuntu focal InRelease
Hit:3 http://security.ubuntu.com/ubuntu focal-security InRelease
Hit:4 https://packages.microsoft.com/repos/edge stable InRelease
Get:5 http://ppa.launchpad.net/obsproject/obs-studio/ubuntu focal InRelease [18,1 kB]
Hit:6 http://archive.ubuntu.com/ubuntu focal InRelease
Ign:7 http://packages.linuxmint.com uma InRelease
Get:8 http://ppa.launchpad.net/oibaf/graphics-drivers/ubuntu focal InRelease [23,9 kB]
Hit:9 http://archive.ubuntu.com/ubuntu focal-updates InRelease
Hit:10 http://packages.linuxmint.com uma Release
Hit:11 http://archive.ubuntu.com/ubuntu focal-backports InRelease
Err:5 http://ppa.launchpad.net/obsproject/obs-studio/ubuntu focal InRelease
  The following signatures couldn't be verified because the public key is not available: NO_PUBKEY EFC71127F425E228
Err:8 http://ppa.launchpad.net/oibaf/graphics-drivers/ubuntu focal InRelease
  The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 957D2708A03A4626
Reading package lists...
Reading package lists...
Building dependency tree...
Reading state information...
libusb-dev is already the newest version (2:0.1.12-32).
libxrandr-dev is already the newest version (2:1.5.2-0ubuntu1).
doxygen is already the newest version (1.8.17-0ubuntu2).
libx11-dev is already the newest version (2:1.6.9-2ubuntu1.2).
0 upgraded, 0 newly installed, 0 to remove and 419 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxcursor-dev is already the newest version (1:1.2.0-2).
libxi-dev is already the newest version (2:1.7.10-0ubuntu1).
libxinerama-dev is already the newest version (2:1.1.4-2).
0 upgraded, 0 newly installed, 0 to remove and 419 not upgraded.
Executing: /tmp/apt-key-gpghome.jZD5ICwZk1/gpg.1.sh --keyserver hkps://keyserver.ubuntu.com:443 --recv-keys 5ABCE68FF4633EA42E219156957D2708A03A4626
You are about to add the following PPA:
 PLEASE READ: don't email me to report bugs, unless you are sure it's a packaging bug. Not only is email not a good tool for tracking bugs, it also excludes anybody else from tracking or working on the issue. Please read the section "Debugging and reporting problems" below.

Also, please don't ask me to include non-free drivers, I won't do it.

Patches and suggestions are welcomed.

=============

All Ubuntu architectures are supported.

Supported Ubuntu versions:
- 20.04 (focal) <- obsolete
- 21.10 (impish) <- obsolete
- 22.04 (jammy) <- supported
- 22.10 (kinetic) <- supported

For forum support see: http://goo.gl/qoUpWK

=== Introduction ===
This PPA provides updated free graphics drivers (from mesa). Updates packages provide:
 * all driver packages are automagically built every day, when there are upstream changes
 * Vulkan 1.3+ and OpenGL 4.6+ : http://mesamatrix.net
 * gallium-nine support. Read the specific section below
 * VDPAU and VAAPI Gallium3D accelerated video drivers (see below)
 * packages built against latest stable LLVM
 * mesa packages built with gcc LTO for smaller binary size (~10-15%) and improved performance

As an added bonus I may include some updated open source games (backported from Debian or newer Ubuntu).

=== Basic usage ===
See "Adding this PPA to your system" later on, or just do:
sudo add-apt-repository ppa:oibaf/graphics-drivers
and update your Ubuntu with the package here.

=== Using gallium-nine ===
Gallium-nine lets you run DirectX 9 games in a faster way, avoiding the inefficient translation of D3D9 calls to OpenGL. For more info see https://wiki.ixit.cz/d3d9
You need:
 * enable this PPA (see previous paragraph)
 * then follow the instructions to install wine from this PPA: https://launchpad.net/~commendsarnex/+archive/ubuntu/winedri3

=== Using alternative drivers - Here be dragons ===
* zink - OpenGL emulated on a Vulkan driver:
  Set MESA_LOADER_DRIVER_OVERRIDE=zink
* zink + lavapipe - OpenGL emulated on a software Vulkan driver:
  Set VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/lvp_icd.x86_64.json MESA_LOADER_DRIVER_OVERRIDE=zink
* llvmpipe OpenGL software render:
  Set MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
* Other options here: https://www.mesa3d.org/envvars.html

=== Using accelerated video ===
Supported VDPAU drivers: r300, r600, radeonsi, nouveau
Supported VAAPI drivers: r600, radeonsi

* First you need to install mesa-vdpau-drivers package:
sudo apt-get install mesa-vdpau-drivers

* Then to test the VDPAU driver with mpv use:
$ mpv --hwdec=vdpau yourvideofile

=== Upgrading to a newer Ubuntu ===
It is *strongly* suggested to remove all packages from this PPA before updating to a newer Ubuntu release. See the section "Revert to original drivers" later on.
Then, after the upgrade, you can add again this PPA.

=== Debugging and reporting problems ===
If you have some problem with the drivers try running the application from the command line and see if there are graphics related warnings. Also type dmesg to see if there are other related information here.

If you get crashes install the relevant -dbg pacakges (libgl1-mesa-dri-dbg or libgl1-mesa-dri-experimental-dbg for 3D drivers and xserver-xorg-video-intel-dbg , xserver-xorg-video-nouveau-dbg or xserver-xorg-video-radeon-dbg for X drivers) and reproduce the crash with gdb. Example:
$ gdb glxgears
[...make your application crash and then get a backtrace with:]
(gdb) bt

If X.org crashes look at /var/log/Xorg.0.log (or /var/log/Xorg.0.log.old when a new X session is started after the crash).

With the gathered information try searching at https://bugs.freedesktop.org/query.cgi (also try with google) if someone already reported a similar bug. If not you may want to report a new bug:
for 3D drivers bugs: https://www.mesa3d.org/bugs.html
for X / 2D driver bugs: https://bugs.freedesktop.org/enter_bug.cgi?product=xorg

=== Revert to original drivers ===
To revert to standard Ubuntu drivers type the following in a prompt shell:
$ sudo apt-get install ppa-purge
$ sudo ppa-purge ppa:oibaf/graphics-drivers

=== Donations ===
Some people asked me if I can accept donations to support this work. I don't, but if you have some spare money I would be happy if you could do a donation to a charity of your choice (for the poors, animals, whatever else you may think it might need it). Then feel free to send me a note about it!
 More info: https://launchpad.net/~oibaf/+archive/ubuntu/graphics-drivers
Reading package lists...
Building dependency tree...
Reading state information...

```

```bash $shci call$
cd _shengine && python3 export-simulation.py flappy-circle SHARED && cd build && cmake --build .
-- The C compiler identification is GNU 9.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Found Vulkan: /usr/lib/x86_64-linux-gnu/libvulkan.so  
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE  
-- Found Doxygen: /usr/bin/doxygen (found version "1.8.17") found components: doxygen 
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
-- Setting build type to 'Release' as none was specified.
-- Looking for fcntl.h
-- Looking for fcntl.h - found
-- Looking for inttypes.h
-- Looking for inttypes.h - found
-- Looking for stdarg.h
-- Looking for stdarg.h - found
-- Looking for strings.h
-- Looking for strings.h - found
-- Looking for string.h
-- Looking for string.h - found
-- Looking for syslog.h
-- Looking for syslog.h - found
-- Looking for 4 include files stdlib.h, ..., float.h
-- Looking for 4 include files stdlib.h, ..., float.h - found
-- Looking for unistd.h
-- Looking for unistd.h - found
-- Looking for sys/types.h
-- Looking for sys/types.h - found
-- Looking for sys/resource.h
-- Looking for sys/resource.h - found
-- Looking for dlfcn.h
-- Looking for dlfcn.h - found
-- Looking for endian.h
-- Looking for endian.h - found
-- Looking for limits.h
-- Looking for limits.h - found
-- Looking for locale.h
-- Looking for locale.h - found
-- Looking for memory.h
-- Looking for memory.h - found
-- Looking for stdint.h
-- Looking for stdint.h - found
-- Looking for stdlib.h
-- Looking for stdlib.h - found
-- Looking for sys/cdefs.h
-- Looking for sys/cdefs.h - found
-- Looking for sys/param.h
-- Looking for sys/param.h - found
-- Looking for sys/random.h
-- Looking for sys/random.h - found
-- Looking for sys/stat.h
-- Looking for sys/stat.h - found
-- Looking for xlocale.h
-- Looking for xlocale.h - not found
-- Looking for _isnan
-- Looking for _isnan - not found
-- Looking for _finite
-- Looking for _finite - not found
-- Looking for INFINITY
-- Looking for INFINITY - found
-- Looking for isinf
-- Looking for isinf - found
-- Looking for isnan
-- Looking for isnan - found
-- Looking for nan
-- Looking for nan - found
-- Looking for _doprnt
-- Looking for _doprnt - not found
-- Looking for snprintf
-- Looking for snprintf - found
-- Looking for vasprintf
-- Looking for vasprintf - found
-- Looking for vsnprintf
-- Looking for vsnprintf - found
-- Looking for vprintf
-- Looking for vprintf - found
-- Looking for arc4random
-- Looking for arc4random - not found
-- Looking for bsd/stdlib.h
-- Looking for bsd/stdlib.h - not found
-- Looking for open
-- Looking for open - found
-- Looking for realloc
-- Looking for realloc - found
-- Looking for setlocale
-- Looking for setlocale - found
-- Looking for uselocale
-- Looking for uselocale - found
-- Looking for strcasecmp
-- Looking for strcasecmp - found
-- Looking for strncasecmp
-- Looking for strncasecmp - found
-- Looking for strdup
-- Looking for strdup - found
-- Looking for strerror
-- Looking for strerror - found
-- Looking for vsyslog
-- Looking for vsyslog - found
-- Looking for getrandom
-- Looking for getrandom - found
-- Looking for getrusage
-- Looking for getrusage - found
-- Looking for strtoll
-- Looking for strtoll - found
-- Looking for strtoull
-- Looking for strtoull - found
-- Looking for stddef.h
-- Looking for stddef.h - found
-- Check size of int
-- Check size of int - done
-- Check size of int64_t
-- Check size of int64_t - done
-- Check size of long
-- Check size of long - done
-- Check size of long long
-- Check size of long long - done
-- Check size of size_t
-- Check size of size_t - done
-- Check size of ssize_t
-- Check size of ssize_t - done
-- Performing Test HAS_GNU_WARNING_LONG
-- Performing Test HAS_GNU_WARNING_LONG - Failed
-- Performing Test HAVE_ATOMIC_BUILTINS
-- Performing Test HAVE_ATOMIC_BUILTINS - Success
-- Performing Test HAVE___THREAD
-- Performing Test HAVE___THREAD - Success
-- Wrote /home/oem/Desktop/shci-implementation/bin/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/json-c/config.h
-- Wrote /home/oem/Desktop/shci-implementation/bin/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/json-c/json_config.h
-- Performing Test REENTRANT_WORKS
-- Performing Test REENTRANT_WORKS - Success
-- Performing Test BSYMBOLIC_WORKS
-- Performing Test BSYMBOLIC_WORKS - Success
-- Performing Test VERSION_SCRIPT_WORKS
-- Performing Test VERSION_SCRIPT_WORKS - Success
-- Found Doxygen: /usr/bin/doxygen (found version "1.8.17") found components: doxygen missing components: dot
-- Wrote /home/oem/Desktop/shci-implementation/bin/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/json-c/doc/Doxyfile
-- Wrote /home/oem/Desktop/shci-implementation/bin/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/json-c/apps_config.h
-- The CXX compiler identification is GNU 9.4.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/oem/Desktop/shci-implementation/bin/_shengine/build
loading /home/oem/Desktop/shci-implementation/bin/_shengine/simulations/flappy-circle/source-files.txt
src/flappy-circle.c

cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

option(SH_SIMULATION_NAME emptytarget)
project(${SH_SIMULATION_NAME})

option(SH_SIMULATION_BINARY_TYPE "EXECUTABLE")
if("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "STATIC")
    add_library(${SH_SIMULATION_NAME} STATIC 
    /home/oem/Desktop/shci-implementation/bin/_shengine/simulations/flappy-circle/src/flappy-circle.c
)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_SIMULATION_NAME} SHARED 
    /home/oem/Desktop/shci-implementation/bin/_shengine/simulations/flappy-circle/src/flappy-circle.c
)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_SIMULATION_NAME}  
    /home/oem/Desktop/shci-implementation/bin/_shengine/simulations/flappy-circle/src/flappy-circle.c
)
endif()
target_include_directories(${SH_SIMULATION_NAME} PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/${SH_SIMULATION_NAME}/include
)
target_link_libraries(${SH_SIMULATION_NAME} PUBLIC shengine)
set_target_properties(${SH_SIMULATION_NAME} PROPERTIES 
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin  
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

running command cd /home/oem/Desktop/shci-implementation/bin/_shengine/build && cmake .. -DSH_ENGINE_BUILD_EDITOR=ON -DSIMULATION_PATH=/home/oem/Desktop/shci-implementation/bin/_shengine/simulations -DSH_EDITOR_ASSETS_PATH=/home/oem/Desktop/shci-implementation/bin/_shengine/simulations/flappy-circle/assets/ -DSH_SIMULATION_NAME=flappy-circle -DSH_SIMULATION_BINARY_TYPE=SHARED
Scanning dependencies of target json-c
[  1%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/arraylist.c.o
[  2%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/debug.c.o
[  3%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/json_c_version.c.o
[  4%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/json_object.c.o
[  5%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o
[  6%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/json_tokener.c.o
[  6%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/json_util.c.o
[  7%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/json_visit.c.o
[  8%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/linkhash.c.o
[  9%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/printbuf.c.o
[ 10%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/random_seed.c.o
[ 11%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/strerror_override.c.o
[ 12%] Building C object shengine/[EXCLUDE_FROM_ALL]/json-c/CMakeFiles/json-c.dir/json_pointer.c.o
[ 13%] Linking C static library libjson-c.a
[ 13%] Built target json-c
Scanning dependencies of target shvulkan
[ 14%] Building C object shengine/[EXCLUDE_FROM_ALL]/shvulkan/CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkCore.c.o
[ 15%] Building C object shengine/[EXCLUDE_FROM_ALL]/shvulkan/CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkMemoryInfo.c.o
[ 16%] Building C object shengine/[EXCLUDE_FROM_ALL]/shvulkan/CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkPipelineData.c.o
[ 17%] Building C object shengine/[EXCLUDE_FROM_ALL]/shvulkan/CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkCheck.c.o
[ 18%] Building C object shengine/[EXCLUDE_FROM_ALL]/shvulkan/CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkDrawLoop.c.o
[ 19%] Linking C static library ../../../../bin/libshvulkan.a
[ 19%] Built target shvulkan
Scanning dependencies of target plyimporter
[ 20%] Building C object shengine/[EXCLUDE_FROM_ALL]/plyimporter/CMakeFiles/plyimporter.dir/PlyImporter/src/plyimporter/plyImporter.c.o
[ 21%] Linking C static library ../../../../bin/libplyimporter.a
[ 21%] Built target plyimporter
Scanning dependencies of target shfd
[ 22%] Building C object shengine/CMakeFiles/shfd.dir/shfd/src/shFile.c.o
[ 23%] Building C object shengine/CMakeFiles/shfd.dir/shfd/src/shfd.c.o
