# shengine
![windows_badge](windows-status.svg)
## [Windows build logs:](https://github.com/MrSinho/ShCI)
  `Build ran for 118.90s`
```bash $shci call$
choco install -y curl mingw cmake
Chocolatey v0.11.2
Chocolatey detected you are not running from an elevated command shell
 (cmd/powershell).

 You may experience errors - many functions/packages
 require admin rights. Only advanced users should run choco w/out an
 elevated shell. When you open the command shell, you should ensure
 that you do so with "Run as Administrator" selected. If you are
 attempting to use Chocolatey in a non-administrator setting, you
 must select a different location other than the default install
 location. See
 https://docs.chocolatey.org/en-us/choco/setup#non-administrative-install
 for details.

For the question below, you have 20 seconds to make a selection.

 Do you want to continue?([Y]es/[N]o): 
Timeout or your choice of '' is not a valid selection.
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
cd _shengine && python export-simulation.py flappy-circle SHARED && cd build && cmake --build .
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.19041.0 to target Windows 10.0.21996.
-- The C compiler identification is MSVC 19.31.31106.2
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.31.31103/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Found Vulkan: C:/VulkanSDK/1.2.198.1/Lib/vulkan-1.lib  
-- Looking for pthread.h
-- Looking for pthread.h - not found
-- Found Threads: TRUE  
-- Found Doxygen: C:/ProgramData/chocolatey/bin/doxygen.exe (found version "1.9.1") found components: doxygen 
-- Including Win32 support
-- Looking for fcntl.h
-- Looking for fcntl.h - found
-- Looking for inttypes.h
-- Looking for inttypes.h - found
-- Looking for stdarg.h
-- Looking for stdarg.h - found
-- Looking for strings.h
-- Looking for strings.h - not found
-- Looking for string.h
-- Looking for string.h - found
-- Looking for syslog.h
-- Looking for syslog.h - not found
-- Looking for 4 include files stdlib.h, ..., float.h
-- Looking for 4 include files stdlib.h, ..., float.h - found
-- Looking for unistd.h
-- Looking for unistd.h - not found
-- Looking for sys/types.h
-- Looking for sys/types.h - found
-- Looking for sys/resource.h
-- Looking for sys/resource.h - not found
-- Looking for dlfcn.h
-- Looking for dlfcn.h - not found
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
-- Looking for sys/param.h - not found
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
-- Looking for open - not found
-- Looking for realloc
-- Looking for realloc - found
-- Looking for setlocale
-- Looking for setlocale - found
-- Looking for uselocale
-- Looking for uselocale - not found
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
-- Check size of SSIZE_T
-- Check size of SSIZE_T - done
-- Performing Test HAS_GNU_WARNING_LONG
-- Performing Test HAS_GNU_WARNING_LONG - Failed
-- Performing Test HAVE_ATOMIC_BUILTINS
-- Performing Test HAVE_ATOMIC_BUILTINS - Failed
-- Performing Test HAVE___THREAD
-- Performing Test HAVE___THREAD - Failed
-- Wrote C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/json-c/config.h
-- Wrote C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/json-c/json_config.h
-- Found Doxygen: C:/ProgramData/chocolatey/bin/doxygen.exe (found version "1.9.1") found components: doxygen missing components: dot
-- Wrote C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/json-c/doc/Doxyfile
-- The CXX compiler identification is MSVC 19.31.31106.2
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.31.31103/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/build
loading C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/simulations/flappy-circle/source-files.txt
src/flappy-circle.c

cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

option(SH_SIMULATION_NAME emptytarget)
project(${SH_SIMULATION_NAME})

option(SH_SIMULATION_BINARY_TYPE "EXECUTABLE")
if("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "STATIC")
    add_library(${SH_SIMULATION_NAME} STATIC 
    C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/simulations/flappy-circle/src/flappy-circle.c
)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_SIMULATION_NAME} SHARED 
    C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/simulations/flappy-circle/src/flappy-circle.c
)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_SIMULATION_NAME}  
    C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/simulations/flappy-circle/src/flappy-circle.c
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

running command cd C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/build && cmake .. -DSH_ENGINE_BUILD_EDITOR=ON -DSIMULATION_PATH=C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/simulations -DSH_EDITOR_ASSETS_PATH=C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/simulations/flappy-circle/assets/ -DSH_SIMULATION_NAME=flappy-circle -DSH_SIMULATION_BINARY_TYPE=SHARED
Microsoft (R) Build Engine version 17.1.0+ae57d105c for .NET Framework
Copyright (C) Microsoft Corporation. All rights reserved.

  Checking Build System
  Generating HTML documentation
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/externals/glfw/docs/CMakeLists.txt
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/externals/cglm/CMakeLists.txt
  euler.c
  affine.c
  io.c
  quat.c
  cam.c
  vec2.c
  vec3.c
  vec4.c
  ivec2.c
  ivec3.c
  ivec4.c
  mat2.c
  mat3.c
  mat4.c
  plane.c
  frustum.c
  box.c
  project.c
  sphere.c
  ease.c
  Generating Code...
  Compiling...
  curve.c
  bezier.c
  ray.c
  affine2d.c
  persp_lh_zo.c
  persp_rh_zo.c
  persp_lh_no.c
  persp_rh_no.c
  ortho_lh_zo.c
  ortho_rh_zo.c
  ortho_lh_no.c
  ortho_rh_no.c
  view_lh_zo.c
  view_rh_zo.c
  view_lh_no.c
  view_rh_no.c
  Generating Code...
     Creating library C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/cglm/Debug/cglm.lib and object C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/build/shengine/[EXCLUDE_FROM_ALL]/cglm/Debug/cglm.exp
  cglm.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\cglm\Debug\cglm-0.dll
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/externals/glfw/src/CMakeLists.txt
  context.c
  init.c
  input.c
  monitor.c
  platform.c
  vulkan.c
  window.c
  egl_context.c
  osmesa_context.c
  null_init.c
  null_monitor.c
  null_window.c
  null_joystick.c
  win32_module.c
  win32_time.c
  win32_thread.c
  win32_init.c
  win32_joystick.c
  win32_monitor.c
  win32_window.c
  Generating Code...
  Compiling...
  wgl_context.c
  Generating Code...
  glfw.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\glfw\src\Debug\glfw3.lib
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/externals/json-c/CMakeLists.txt
  arraylist.c
  debug.c
  json_c_version.c
  json_object.c
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_object.c(204,52): warning C4267: 'function': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_object.c(231,29): warning C4267: 'function': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_object.c(242,50): warning C4267: 'function': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_object.c(685,44): warning C4267: 'function': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_object.c(1139,27): warning C4267: 'initializing': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
  json_object_iterator.c
  json_tokener.c
  json_util.c
  json_visit.c
  linkhash.c
  printbuf.c
  random_seed.c
  strerror_override.c
  json_pointer.c
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_pointer.c(32,19): warning C4267: 'initializing': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_pointer.c(33,27): warning C4267: 'initializing': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_pointer.c(83,32): warning C4267: '=': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\json-c\json_pointer.c(46,21): warning C4267: 'initializing': conversion from 'size_t' to 'int', possible loss of data [C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\json-c.vcxproj]
  Generating Code...
  json-c.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\[EXCLUDE_FROM_ALL]\json-c\Debug\json-c.lib
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/externals/plyimporter/CMakeLists.txt
  plyImporter.c
  plyimporter.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\bin\Debug\plyimporter.lib
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/externals/shvulkan/CMakeLists.txt
  shVkCore.c
  shVkMemoryInfo.c
  shVkPipelineData.c
  shVkCheck.c
  shVkDrawLoop.c
  Generating Code...
  shvulkan.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\bin\Debug\shvulkan.lib
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/shengine/CMakeLists.txt
  shFile.c
  shfd.c
  Generating Code...
  shfd.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\bin\Debug\shfd.lib
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/externals/shserial/CMakeLists.txt
  shSerial.c
  shserial.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\externals\shserial\bin\Debug\shserial.lib
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/shengine/CMakeLists.txt
  shSharedHost.c
  shsharedhost.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\build\shengine\Debug\shsharedhost.lib
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/shengine/CMakeLists.txt
  shInput.c
  shTime.c
  shWindow.c
  shProjection.c
  shView.c
  shScene.c
  shMaterialHost.c
  Generating Code...
  shengine.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\bin\Debug\shengine.lib
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/simulations/CMakeLists.txt
  flappy-circle.c
     Creating library C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/bin/Debug/flappy-circle.lib and object C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/bin/Debug/flappy-circle.exp
  flappy-circle.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\bin\Debug\flappy-circle.dll
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/shengine/CMakeLists.txt
  shEditor.c
  sheditor.vcxproj -> C:\Users\sino3\Desktop\GitHub\ShCI-implementation\build\_shengine\bin\Debug\sheditor.exe
  Building Custom Rule C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shengine/CMakeLists.txt
