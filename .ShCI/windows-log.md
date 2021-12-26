# SH-Engine
![windows_badge](windows-status.svg)
## [Windows build logs:](https://github.com/MrSinho/ShCI)
  `Build ran for 122.66s`
```bash $shci call$
choco install -y curl mingw cmake
Chocolatey v0.11.2
Installing the following packages:
curl;mingw;cmake
By installing, you accept licenses for the packages.
curl v7.80.0 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
mingw v11.2.0.07112021 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
cmake v3.22.0 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

Chocolatey installed 0/3 packages. 
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).

Warnings:
 - cmake - cmake v3.22.0 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - curl - curl v7.80.0 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - mingw - mingw v11.2.0.07112021 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

```

```bash $shci call$
mingw-get install gcc mingw32-make gdb

```

```bash $shci call$
cd SH-Engine && mkdir build && cd build && cmake -G"MinGW Makefiles" .. && mingw32-make
-- The C compiler identification is GNU 6.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/MinGW/bin/gcc.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - not found
-- Found Threads: TRUE  
-- Found Doxygen: C:/ProgramData/chocolatey/bin/doxygen.exe (found version "1.9.1") found components: doxygen 
-- Including Win32 support
-- Looking for dinput.h
-- Looking for dinput.h - not found
-- Looking for xinput.h
-- Looking for xinput.h - not found
-- Setting build type to 'Release' as none was specified.
-- Looking for sys/resource.h
-- Looking for sys/resource.h - not found
-- Wrote C:/Users/sino3/Desktop/GitHub/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/apps_config.h
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
-- Looking for syslog.h - not found
-- Looking for 4 include files stdlib.h, ..., float.h
-- Looking for 4 include files stdlib.h, ..., float.h - found
-- Looking for unistd.h
-- Looking for unistd.h - found
-- Looking for sys/types.h
-- Looking for sys/types.h - found
-- Looking for dlfcn.h
-- Looking for dlfcn.h - found
-- Looking for endian.h
-- Looking for endian.h - not found
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
-- Looking for sys/cdefs.h - not found
-- Looking for sys/param.h
-- Looking for sys/param.h - found
-- Looking for sys/random.h
-- Looking for sys/random.h - not found
-- Looking for sys/stat.h
-- Looking for sys/stat.h - found
-- Looking for xlocale.h
-- Looking for xlocale.h - not found
-- Looking for _isnan
-- Looking for _isnan - found
-- Looking for _finite
-- Looking for _finite - found
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
-- Looking for vasprintf - not found
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
-- Looking for uselocale - not found
-- Looking for strcasecmp
-- Looking for strcasecmp - found
-- Looking for strncasecmp
-- Looking for strncasecmp - found
-- Looking for strdup
-- Looking for strdup - found
-- Looking for strerror
-- Looking for strerror - found
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
-- Wrote C:/Users/sino3/Desktop/GitHub/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/config.h
-- Wrote C:/Users/sino3/Desktop/GitHub/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/json_config.h
-- Performing Test REENTRANT_WORKS
-- Performing Test REENTRANT_WORKS - Success
-- Performing Test BSYMBOLIC_WORKS
-- Performing Test BSYMBOLIC_WORKS - Success
-- Performing Test VERSION_SCRIPT_WORKS
-- Performing Test VERSION_SCRIPT_WORKS - Success
-- Found Doxygen: C:/ProgramData/chocolatey/bin/doxygen.exe (found version "1.9.1") found components: doxygen missing components: dot
-- Wrote C:/Users/sino3/Desktop/GitHub/ShCI-implementation/bin/SH-Engine/build/Externals/json-c/doc/Doxyfile
-- Found Vulkan: C:/VulkanSDK/1.2.182.0/Lib32/vulkan-1.lib  
-- looking for glslc
-- found glslc
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/sino3/Desktop/GitHub/ShCI-implementation/bin/SH-Engine/build
[  1%] Building C object CMakeFiles/ShUtilities.dir/ShUtilities/src/shUtilities.c.obj
[  2%] Linking C static library ..\bin\libShUtilities.a
[  2%] Built target ShUtilities
[  3%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shIntersection.c.obj
[  4%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shLineSolver.c.obj
[  5%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shMatrix.c.obj
[  6%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shScalar.c.obj
[  7%] Building C object Externals/ShPhysics/CMakeFiles/ShLinearAlgebra.dir/ShLinearAlgebra/src/shVector.c.obj
[  8%] Linking C static library libShLinearAlgebra.a
[  8%] Built target ShLinearAlgebra
[  9%] Building C object Externals/ShPhysics/CMakeFiles/ShPhysics.dir/ShPhysics/src/shDynamics.c.obj
[ 10%] Building C object Externals/ShPhysics/CMakeFiles/ShPhysics.dir/ShPhysics/src/shPhysicsCollision.c.obj
[ 10%] Linking C static library ..\..\..\bin\libShPhysics.a
[ 10%] Built target ShPhysics
[ 11%] Building C object Externals/PlyImporter/CMakeFiles/PlyImporter.dir/PlyImporter/src/PlyImporter.c.obj
[ 12%] Linking C static library ..\..\..\bin\libPlyImporter.a
[ 12%] Built target PlyImporter
[ 13%] Building C object Externals/json-c/CMakeFiles/json-c.dir/arraylist.c.obj
[ 14%] Building C object Externals/json-c/CMakeFiles/json-c.dir/debug.c.obj
[ 15%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_c_version.c.obj
[ 15%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_object.c.obj
[ 16%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_object_iterator.c.obj
[ 17%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_tokener.c.obj
[ 18%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_util.c.obj
[ 19%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_visit.c.obj
[ 20%] Building C object Externals/json-c/CMakeFiles/json-c.dir/linkhash.c.obj
[ 21%] Building C object Externals/json-c/CMakeFiles/json-c.dir/printbuf.c.obj
[ 22%] Building C object Externals/json-c/CMakeFiles/json-c.dir/random_seed.c.obj
[ 23%] Building C object Externals/json-c/CMakeFiles/json-c.dir/strerror_override.c.obj
[ 24%] Building C object Externals/json-c/CMakeFiles/json-c.dir/json_pointer.c.obj
[ 25%] Linking C static library libjson-c.a
[ 25%] Built target json-c
[ 26%] Building C object CMakeFiles/ShDescriptorHandle.dir/ShDescriptorHandle/src/shDescriptorHandle.c.obj
[ 27%] Linking C static library libShDescriptorHandle.a
[ 27%] Built target ShDescriptorHandle
[ 28%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/context.c.obj
[ 29%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/init.c.obj
[ 30%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/input.c.obj
[ 31%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/monitor.c.obj
[ 32%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/platform.c.obj
[ 33%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/vulkan.c.obj
[ 34%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/window.c.obj
[ 35%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/egl_context.c.obj
[ 36%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/osmesa_context.c.obj
[ 37%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_init.c.obj
[ 38%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_monitor.c.obj
[ 39%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_window.c.obj
[ 39%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/null_joystick.c.obj
[ 40%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/win32_module.c.obj
[ 41%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/win32_time.c.obj
[ 42%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/win32_thread.c.obj
[ 43%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/win32_init.c.obj
[ 44%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/win32_joystick.c.obj
[ 45%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/win32_monitor.c.obj
[ 46%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/win32_window.c.obj
[ 47%] Building C object Externals/glfw/src/CMakeFiles/glfw.dir/wgl_context.c.obj
[ 48%] Linking C static library ..\..\..\..\bin\libglfw3.a
[ 48%] Built target glfw
[ 49%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/euler.c.obj
[ 50%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/affine.c.obj
[ 51%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/io.c.obj
[ 52%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/quat.c.obj
[ 53%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/cam.c.obj
[ 54%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec2.c.obj
[ 54%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec3.c.obj
[ 55%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/vec4.c.obj
[ 56%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat2.c.obj
[ 57%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat3.c.obj
[ 58%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/mat4.c.obj
[ 59%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/plane.c.obj
[ 60%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/frustum.c.obj
[ 61%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/box.c.obj
[ 62%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/project.c.obj
[ 63%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/sphere.c.obj
[ 64%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/ease.c.obj
[ 65%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/curve.c.obj
[ 66%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/bezier.c.obj
[ 67%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/ray.c.obj
[ 67%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/affine2d.c.obj
[ 68%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_lh_zo.c.obj
[ 69%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_rh_zo.c.obj
[ 70%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_lh_no.c.obj
[ 71%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/persp_rh_no.c.obj
[ 72%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_lh_zo.c.obj
[ 73%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_rh_zo.c.obj
[ 74%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_lh_no.c.obj
[ 75%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/ortho_rh_no.c.obj
[ 76%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_lh_zo.c.obj
[ 77%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_rh_zo.c.obj
[ 78%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_lh_no.c.obj
[ 79%] Building C object Externals/cglm/CMakeFiles/cglm.dir/src/clipspace/view_rh_no.c.obj
[ 79%] Linking C shared library ..\..\..\bin\libcglm-0.dll
[ 79%] Built target cglm
[ 80%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shDrawLoop.c.obj
[ 81%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shEuler.c.obj
[ 82%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shInput.c.obj
[ 83%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shMaterial.c.obj
[ 84%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shProjection.c.obj
[ 85%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shSceneHandle.c.obj
[ 86%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shTime.c.obj
[ 87%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shView.c.obj
[ 88%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkCore.c.obj
[ 88%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkMemoryInfo.c.obj
[ 89%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shVkPipelineData.c.obj
[ 90%] Building C object CMakeFiles/SH-Engine.dir/SH-Engine/src/shWindow.c.obj
[ 91%] Linking C static library ..\bin\libSH-Engine.a
[ 91%] Built target SH-Engine
[ 92%] Building C object Externals/ShSerial/CMakeFiles/ShSerial.dir/ShSerial/src/ShSerial.c.obj
[ 93%] Linking C static library ..\..\..\Externals\ShSerial\bin\libShSerial.a
[ 93%] Built target ShSerial
[ 93%] Building C object CMakeFiles/Editor.dir/Editor/src/Editor.c.obj
[ 94%] Linking C executable ..\bin\Editor.exe
[ 94%] Built target Editor
[ 95%] Generating HTML documentation
[ 95%] Built target docs
[ 96%] Building C object Externals/json-c/apps/CMakeFiles/json_parse.dir/json_parse.c.obj
[ 97%] Linking C executable json_parse.exe
[ 97%] Built target json_parse
[ 98%] Building C object Externals/ShPhysics/CMakeFiles/ShPhysicsExample.dir/ShPhysicsExample/src/shPhysicsExample.c.obj
[100%] Linking C executable ..\..\..\bin\ShPhysicsExample.exe
[100%] Built target ShPhysicsExample
