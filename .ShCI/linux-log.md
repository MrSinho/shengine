# SH-Engine
![linux_badge](linux-status.svg)
## [Linux/Unix build logs:](https://github.com/MrSinho/ShCI)
  
```bash
$$shci call$$:
apt install -y wget gcc cmake make gdb
```

```bash
$$shci call$$:
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
```

```bash
$$shci call$$:
cd SH-Engine && mkdir build && cd build && cmake .. && cmake --build .
-- The C compiler identification is GNU 11.2.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
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
-- Found Doxygen: /usr/bin/doxygen (found version "1.9.2") found components: doxygen 
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
-- Found Doxygen: /usr/bin/doxygen (found version "1.9.2") found components: doxygen missing components: dot
-- Wrote /home/mrsinho/Desktop/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/doc/Doxyfile
-- Found Vulkan: /usr/lib/x86_64-linux-gnu/libvulkan.so  
-- looking for glslc
-- glslc not found, required for shader compilation, install before executing the engine in debug mode
-- Configuring done
-- Generating done
-- Build files have been written to: /home/mrsinho/Desktop/ShCI-implementation/bin/SH-Engine/build
[  1%] Building C object Externals/json-c/CMakeFiles/json-c.dir/arraylist.c.o
[  2%] Building C object Externals/json-c/CMakeFiles/json-c.dir/debug.c.o
[  3%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_c_version.c.o
[  4%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_object.c.o
[  5%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_object_iterator.c.o
[  6%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_tokener.c.o
[  7%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_util.c.o
[  8%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_visit.c.o
[  9%] Building C object Externals/json-c/CMakeFiles/json-c.dir/linkhash.c.o
[ 10%] Building C object Externals/json-c/CMakeFiles/json-c.dir/printbuf.c.o
[ 11%] Building C object Externals/json-c/CMakeFiles/json-c.dir/random_seed.c.o
[ 12%] Building C object Externals/json-c/CMakeFiles/json-c.dir/strerror_override.c.o
[ 13%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_pointer.c.o
[ 14%] Linking C static library libjson-c.a
[ 14%] Built target json-c
[ 15%] Building C object CMakeFiles/ShUtilities.dir/ShUtilities/src/shUtilities.c.o
[ 16%] Linking C static library ../bin/libShUtilities.a
[ 16%] Built target ShUtilities
[ 17%] Building C object Externals/PlyImporter/CMakeFiles/PlyImporter.dir/PlyImporter/src/PlyImporter.c.o
[ 18%] Linking C static library ../../../bin/libPlyImporter.a
[ 18%] Built target PlyImporter
[ 20%] Building C object CMakeFiles/ShDescriptorHandle.dir/ShDescriptorHandle/src/shDescriptorHandle.c.o
[ 21%] Linking C static library libShDescriptorHandle.a
[ 21%] Built target ShDescriptorHandle
[ 22%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/context.c.o
[ 23%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/init.c.o
[ 24%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/input.c.o
[ 25%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/monitor.c.o
[ 26%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/platform.c.o
[ 27%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/vulkan.c.o
[ 28%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/window.c.o
[ 29%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/egl_context.c.o
[ 30%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/osmesa_context.c.o
[ 31%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_init.c.o
[ 32%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_monitor.c.o
[ 33%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_window.c.o
[ 34%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_joystick.c.o
[ 35%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/posix_module.c.o
[ 36%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/posix_time.c.o
[ 37%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/posix_thread.c.o
[ 38%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/x11_init.c.o
[ 40%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/x11_monitor.c.o
[ 41%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/x11_window.c.o
[ 42%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/xkb_unicode.c.o
[ 43%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/glx_context.c.o
[ 44%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/linux_joystick.c.o
[ 45%] Linking C static library ../../../../bin/libglfw3.a
[ 45%] Built target glfw
[ 46%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/euler.c.o
[ 47%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/affine.c.o
[ 48%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/io.c.o
[ 49%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/quat.c.o
[ 50%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/cam.c.o
[ 51%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec2.c.o
[ 52%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec3.c.o
[ 53%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec4.c.o
[ 54%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat2.c.o
[ 55%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat3.c.o
[ 56%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat4.c.o
[ 57%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/plane.c.o
[ 58%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/frustum.c.o
[ 60%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/box.c.o
[ 61%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/project.c.o
[ 62%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/sphere.c.o
[ 63%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/ease.c.o
[ 64%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/curve.c.o
[ 65%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/bezier.c.o
[ 66%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/ray.c.o
[ 67%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/affine2d.c.o
[ 68%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_lh_zo.c.o
[ 69%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_rh_zo.c.o
[ 70%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_lh_no.c.o
[ 71%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_rh_no.c.o
[ 72%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_lh_zo.c.o
[ 73%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_rh_zo.c.o
[ 74%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_lh_no.c.o
[ 75%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_rh_no.c.o
[ 76%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_lh_zo.c.o
[ 77%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_rh_zo.c.o
[ 78%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_lh_no.c.o
[ 80%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_rh_no.c.o
[ 81%] Linking C shared library libcglm.so
[ 81%] Built target cglm
[ 82%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shDrawLoop.c.o
[ 83%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shEuler.c.o
[ 84%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shInput.c.o
[ 85%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shMaterial.c.o
[ 86%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shProjection.c.o
[ 87%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shSceneHandle.c.o
[ 88%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shTime.c.o
[ 89%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shView.c.o
[ 90%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkCore.c.o
[ 91%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkMemoryInfo.c.o
[ 92%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkPipelineData.c.o
[ 93%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shWindow.c.o
[ 94%] Linking C static library ../bin/libSH-Engine.a
[ 94%] Built target SH-Engine
[ 95%] Building C object CMakeFiles/Editor.dir/Editor/src/Editor.c.o
[ 96%] Linking C executable ../bin/Editor
[ 96%] Built target Editor
[ 97%] Generating HTML documentation
[ 97%] Built target docs
[ 98%] Building C object Externals/json-c/apps/CMakeFiles/json_parse.dir/json_parse.c.o
[100%] Linking C executable json_parse
[100%] Built target json_parse
