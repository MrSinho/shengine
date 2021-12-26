# SH-Engine
![linux_badge](linux-status.svg)
## [Linux/Unix build logs:](https://github.com/MrSinho/ShCI)
  `Build ran for 35.77s`
```bash $shci call$
apt install -y wget gcc cmake make gdb
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.16.3-1ubuntu1).
gcc is already the newest version (4:9.3.0-1ubuntu2).
make is already the newest version (4.2.1-1.2).
gdb is already the newest version (9.2-0ubuntu1~20.04).
wget is already the newest version (1.20.3-1ubuntu2).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.

```

```bash $shci call$
sudo apt update -y
sudo apt install -y libx11-dev
sudo apt install -y doxygen
sudo apt install -y libxrandr-dev

sudo apt install -y libxinerama-dev
sudo apt install -y libxcursor-dev
sudo apt install -y libxi-dev

sudo add-apt-repository -y ppa:oibaf/graphics-drivers
sudo apt update -y
sudo apt install -y libvulkan-dev 
sudo apt install -y libvulkan1 
sudo apt install -y mesa-vulkan-drivers 
sudo apt install -y vulkan-utils

sudo apt install -y libusb-dev
Ign:1 http://dl.google.com/linux/chrome-remote-desktop/deb stable InRelease
Hit:2 http://ppa.launchpad.net/oibaf/graphics-drivers/ubuntu focal InRelease
Hit:3 http://archive.ubuntu.com/ubuntu focal InRelease
Hit:4 http://security.ubuntu.com/ubuntu focal-security InRelease
Hit:5 http://archive.ubuntu.com/ubuntu focal-updates InRelease
Hit:6 http://dl.google.com/linux/chrome-remote-desktop/deb stable Release
Hit:7 http://ppa.launchpad.net/openshot.developers/ppa/ubuntu focal InRelease
Hit:8 http://archive.ubuntu.com/ubuntu focal-backports InRelease
Hit:9 http://archive.canonical.com/ubuntu focal InRelease
Get:10 https://packages.microsoft.com/repos/edge stable InRelease [7.343 B]
Ign:11 http://packages.linuxmint.com uma InRelease
Hit:12 http://packages.linuxmint.com uma Release
Err:10 https://packages.microsoft.com/repos/edge stable InRelease
  The following signatures couldn't be verified because the public key is not available: NO_PUBKEY EB3E94ADBE1229CF
Reading package lists...
Reading package lists...
Building dependency tree...
Reading state information...
libx11-dev is already the newest version (2:1.6.9-2ubuntu1.2).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
doxygen is already the newest version (1.8.17-0ubuntu2).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxrandr-dev is already the newest version (2:1.5.2-0ubuntu1).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxinerama-dev is already the newest version (2:1.1.4-2).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxcursor-dev is already the newest version (1:1.2.0-2).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxi-dev is already the newest version (2:1.7.10-0ubuntu1).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Executing: /tmp/apt-key-gpghome.j3yltbKsuM/gpg.1.sh --keyserver hkps://keyserver.ubuntu.com:443 --recv-keys 5ABCE68FF4633EA42E219156957D2708A03A4626
You are about to add the following PPA:
 PLEASE READ: don't email me to report bugs, unless you are sure it's a packaging bug. Not only is email not a good tool for tracking bugs, it also excludes anybody else from tracking or working on the issue. Please read the section "Debugging and reporting problems" below.

Also, please don't ask me to include non-free drivers, I won't do it.

Patches and suggestions are welcomed.

=============

All Ubuntu architectures are supported.

Supported Ubuntu versions:
- 20.04 (focal) <- supported
- 21.04 (hirsute) <- supported
- 21.10 (impish) <- supported

For forum support see: http://goo.gl/qoUpWK

=== Introduction ===
This PPA provides updated free graphics drivers packages for radeon, intel and nvidia hardware. Updates packages provide:
 * all driver packages are automagically built every day, when there are upstream changes
 * Vulkan 1.2+ and OpenGL 4.6+ : http://mesamatrix.net
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
* Old Intel i965 OpenGL driver (iris is the default):
  Set MESA_LOADER_DRIVER_OVERRIDE=i965
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
Hit:1 http://archive.ubuntu.com/ubuntu focal InRelease
Hit:2 http://archive.canonical.com/ubuntu focal InRelease
Hit:3 http://ppa.launchpad.net/oibaf/graphics-drivers/ubuntu focal InRelease
Hit:4 http://archive.ubuntu.com/ubuntu focal-updates InRelease
Ign:5 http://dl.google.com/linux/chrome-remote-desktop/deb stable InRelease
Hit:6 http://archive.ubuntu.com/ubuntu focal-backports InRelease
Hit:7 http://ppa.launchpad.net/openshot.developers/ppa/ubuntu focal InRelease
Hit:8 http://dl.google.com/linux/chrome-remote-desktop/deb stable Release
Get:9 https://packages.microsoft.com/repos/edge stable InRelease [7.343 B]
Ign:10 http://packages.linuxmint.com uma InRelease
Hit:11 http://security.ubuntu.com/ubuntu focal-security InRelease
Hit:12 http://packages.linuxmint.com uma Release
Err:9 https://packages.microsoft.com/repos/edge stable InRelease
  The following signatures couldn't be verified because the public key is not available: NO_PUBKEY EB3E94ADBE1229CF
Reading package lists...
Reading package lists...
Building dependency tree...
Reading state information...
libvulkan-dev is already the newest version (1.2.131.2-1).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libvulkan1 is already the newest version (1.2.131.2-1).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
mesa-vulkan-drivers is already the newest version (22.0~git2112250600.222487~oibaf~f).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
vulkan-utils is already the newest version (1.2.131.1+dfsg1-1).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libusb-dev is already the newest version (2:0.1.12-32).
0 upgraded, 0 newly installed, 0 to remove and 15 not upgraded.

```

```bash $shci call$
cd SH-Engine && mkdir build && cd build && cmake .. && make
-- The C compiler identification is GNU 9.3.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
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
-- Looking for IceConnectionNumber in ICE
-- Looking for IceConnectionNumber in ICE - found
-- Setting build type to 'Release' as none was specified.
-- Looking for sys/resource.h
-- Looking for sys/resource.h - found
-- Looking for getrusage
-- Looking for getrusage - found
-- Wrote /home/mrsinho/Desktop/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/apps_config.h
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
-- Wrote /home/mrsinho/Desktop/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/config.h
-- Wrote /home/mrsinho/Desktop/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/json_config.h
-- Performing Test REENTRANT_WORKS
-- Performing Test REENTRANT_WORKS - Success
-- Performing Test BSYMBOLIC_WORKS
-- Performing Test BSYMBOLIC_WORKS - Success
-- Performing Test VERSION_SCRIPT_WORKS
-- Performing Test VERSION_SCRIPT_WORKS - Success
-- Found Doxygen: /usr/bin/doxygen (found version "1.8.17") found components: doxygen missing components: dot
-- Wrote /home/mrsinho/Desktop/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/doc/Doxyfile
-- Found Vulkan: /usr/lib/x86_64-linux-gnu/libvulkan.so  
-- looking for glslc
-- glslc not found, required for shader compilation, install before executing the engine in debug mode
-- Configuring done
-- Generating done
-- Build files have been written to: /home/mrsinho/Desktop/ShCI-implementation/bin/SH-Engine/build
Scanning dependencies of target ShSerial
[  1%] Building C object Externals/ShSerial/CMakeFiles/ShSerial.dir/ShSerial/src/ShSerial.c.o
[  2%] Linking C static library ../../../Externals/ShSerial/bin/libShSerial.a
[  2%] Built target ShSerial
Scanning dependencies of target ShLinearAlgebra
[  3%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shIntersection.c.o
[  4%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shLineSolver.c.o
[  5%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shMatrix.c.o
[  6%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shScalar.c.o
[  7%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shVector.c.o
[  7%] Linking C static library libShLinearAlgebra.a
[  7%] Built target ShLinearAlgebra
Scanning dependencies of target ShUtilities
[  8%] Building C object CMakeFiles/ShUtilities.dir/ShUtilities/src/shUtilities.c.o
[  9%] Linking C static library ../bin/libShUtilities.a
[  9%] Built target ShUtilities
Scanning dependencies of target PlyImporter
[ 10%] Building C object Externals/PlyImporter/CMakeFiles/PlyImporter.dir/PlyImporter/src/PlyImporter.c.o
[ 11%] Linking C static library ../../../bin/libPlyImporter.a
[ 11%] Built target PlyImporter
Scanning dependencies of target json-c
[ 12%] Building C object Externals/json-c/CMakeFiles/json-c.dir/arraylist.c.o
[ 13%] Building C object Externals/json-c/CMakeFiles/json-c.dir/debug.c.o
[ 14%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_c_version.c.o
[ 15%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_object.c.o
[ 15%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o
[ 16%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_tokener.c.o
[ 17%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_util.c.o
[ 18%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_visit.c.o
[ 19%] Building C object Externals/json-c/CMakeFiles/json-c.dir/linkhash.c.o
[ 20%] Building C object Externals/json-c/CMakeFiles/json-c.dir/printbuf.c.o
[ 21%] Building C object Externals/json-c/CMakeFiles/json-c.dir/random_seed.c.o
[ 22%] Building C object Externals/json-c/CMakeFiles/json-c.dir/strerror_override.c.o
[ 23%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_pointer.c.o
[ 24%] Linking C static library libjson-c.a
[ 24%] Built target json-c
Scanning dependencies of target ShPhysics
[ 25%] Building C object Externals/ShPhysics/CMakeFiles/ShPhysics.dir/ShPhysics/src/shDynamics.c.o
[ 26%] Building C object Externals/ShPhysics/CMakeFiles/ShPhysics.dir/ShPhysics/src/shPhysicsCollision.c.o
[ 27%] Linking C static library ../../../bin/libShPhysics.a
[ 27%] Built target ShPhysics
Scanning dependencies of target ShDescriptorHandle
[ 28%] Building C object CMakeFiles/ShDescriptorHandle.dir/ShDescriptorHandle/src/shDescriptorHandle.c.o
[ 29%] Linking C static library libShDescriptorHandle.a
[ 29%] Built target ShDescriptorHandle
Scanning dependencies of target glfw
[ 30%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/context.c.o
[ 31%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/init.c.o
[ 32%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/input.c.o
[ 32%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/monitor.c.o
[ 33%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/platform.c.o
[ 34%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/vulkan.c.o
[ 35%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/window.c.o
[ 36%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/egl_context.c.o
[ 37%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/osmesa_context.c.o
[ 38%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_init.c.o
[ 39%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_monitor.c.o
[ 40%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_window.c.o
[ 41%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_joystick.c.o
[ 42%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/posix_module.c.o
[ 43%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/posix_time.c.o
[ 43%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/posix_thread.c.o
[ 44%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/x11_init.c.o
[ 45%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/x11_monitor.c.o
[ 46%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/x11_window.c.o
[ 47%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/xkb_unicode.c.o
[ 48%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/glx_context.c.o
[ 49%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/linux_joystick.c.o
[ 50%] Linking C static library ../../../../bin/libglfw3.a
[ 50%] Built target glfw
Scanning dependencies of target cglm
[ 51%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/euler.c.o
[ 52%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/affine.c.o
[ 52%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/io.c.o
[ 53%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/quat.c.o
[ 54%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/cam.c.o
[ 55%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec2.c.o
[ 56%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec3.c.o
[ 57%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec4.c.o
[ 58%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat2.c.o
[ 59%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat3.c.o
[ 60%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat4.c.o
[ 61%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/plane.c.o
[ 62%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/frustum.c.o
[ 63%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/box.c.o
[ 63%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/project.c.o
[ 64%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/sphere.c.o
[ 65%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/ease.c.o
[ 66%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/curve.c.o
[ 67%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/bezier.c.o
[ 68%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/ray.c.o
[ 69%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/affine2d.c.o
[ 70%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_lh_zo.c.o
[ 71%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_rh_zo.c.o
[ 72%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_lh_no.c.o
[ 73%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_rh_no.c.o
[ 74%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_lh_zo.c.o
[ 74%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_rh_zo.c.o
[ 75%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_lh_no.c.o
[ 76%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_rh_no.c.o
[ 77%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_lh_zo.c.o
[ 78%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_rh_zo.c.o
[ 79%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_lh_no.c.o
[ 80%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_rh_no.c.o
[ 81%] Linking C shared library libcglm.so
[ 81%] Built target cglm
Scanning dependencies of target SH-Engine
[ 82%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shDrawLoop.c.o
[ 83%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shEuler.c.o
[ 84%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shInput.c.o
[ 85%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shMaterial.c.o
[ 86%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shProjection.c.o
[ 87%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shSceneHandle.c.o
[ 88%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shTime.c.o
[ 89%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shView.c.o
[ 89%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkCore.c.o
[ 90%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkMemoryInfo.c.o
[ 91%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkPipelineData.c.o
[ 92%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shWindow.c.o
[ 93%] Linking C static library ../bin/libSH-Engine.a
[ 93%] Built target SH-Engine
Scanning dependencies of target Editor
[ 93%] Building C object CMakeFiles/Editor.dir/Editor/src/Editor.c.o
