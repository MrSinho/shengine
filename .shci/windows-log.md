
# shengine

![windows-badge](windows-status.svg)

## [windows build logs:](https://github.com/mrsinho/shci)

        
```bash
choco install -y curl mingw cmake
mingw-get install gcc mingw32-make gdb
```

```bash
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

Did you know the proceeds of Pro (and some proceeds from other
 licensed editions) go into bettering the community infrastructure?
 Your support ensures an active community, keeps Chocolatey tip-top,
 plus it nets you some awesome features!
 https://chocolatey.org/compare

```

---

    
```bash
cd shengine && python export-simulation.py flappy-circle SHARED && cd build && cmake --build .
```

```bash
-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.21996.
-- Found Doxygen: C:/ProgramData/chocolatey/bin/doxygen.exe (found version "1.9.1") found components: doxygen 
-- Including Win32 support
-- Wrote C:/Users/sino3/Desktop/shci-test/shengine/shengine/build/shengine/externals/json-c/config.h
-- Wrote C:/Users/sino3/Desktop/shci-test/shengine/shengine/build/shengine/externals/json-c/json_config.h
-- Found Doxygen: C:/ProgramData/chocolatey/bin/doxygen.exe (found version "1.9.1") found components: doxygen missing components: dot
-- Wrote C:/Users/sino3/Desktop/shci-test/shengine/shengine/build/shengine/externals/json-c/doc/Doxyfile
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/sino3/Desktop/shci-test/shengine/shengine/build
loading C:/Users/sino3/Desktop/shci-test/shengine/shengine/simulations/flappy-circle/source-files.txt
src/flappy-circle.c
loading C:/Users/sino3/Desktop/shci-test/shengine/shengine/simulations/flappy-circle/libs.txt

subdirectories:
		

cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

	

option(SH_SIMULATION_NAME CACHE emptytarget)
project(${SH_SIMULATION_NAME})

option(SH_SIMULATION_BINARY_TYPE CACHE "EXECUTABLE")
if("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "STATIC")
    add_library(${SH_SIMULATION_NAME} STATIC 
    	C:/Users/sino3/Desktop/shci-test/shengine/shengine/simulations/flappy-circle/src/flappy-circle.c

)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_SIMULATION_NAME} SHARED 
    	C:/Users/sino3/Desktop/shci-test/shengine/shengine/simulations/flappy-circle/src/flappy-circle.c

)
elseif("${SH_SIMULATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_SIMULATION_NAME}  
    	C:/Users/sino3/Desktop/shci-test/shengine/shengine/simulations/flappy-circle/src/flappy-circle.c

)
endif()
target_include_directories(${SH_SIMULATION_NAME} PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/${SH_SIMULATION_NAME}/include
)
target_link_libraries(${SH_SIMULATION_NAME} PUBLIC shengine 	)
set_target_properties(${SH_SIMULATION_NAME} PROPERTIES 
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin  
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

running command cd C:/Users/sino3/Desktop/shci-test/shengine/shengine/build && cmake .. -DSH_ENGINE_BUILD_EDITOR=ON -DSIMULATION_PATH=C:/Users/sino3/Desktop/shci-test/shengine/shengine/simulations -DSH_EDITOR_ASSETS_PATH=C:/Users/sino3/Desktop/shci-test/shengine/shengine/simulations/flappy-circle/assets/ -DSH_SIMULATION_NAME=flappy-circle -DSH_SIMULATION_BINARY_TYPE=SHARED 
Microsoft (R) Build Engine version 17.2.1+52cd2da31 for .NET Framework
Copyright (C) Microsoft Corporation. All rights reserved.

  Checking Build System
  euler.c
  affine.c
  io.c
  quat.c
  cam.c
  vec2.c
  ivec2.c
  vec3.c
  ivec3.c
  vec4.c
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
  ortho_lh_no.c
  ortho_lh_zo.c
  ortho_rh_no.c
  ortho_rh_zo.c
  persp_lh_no.c
  persp_lh_zo.c
  persp_rh_no.c
  persp_rh_zo.c
  view_lh_no.c
  view_lh_zo.c
  view_rh_no.c
  view_rh_zo.c
  project_no.c
  project_zo.c
  Generating Code...
  cglm.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\build\shengine\externals\cglm\Debug\cglm.lib
  native-export.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\native-export.lib
  export-shaders.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\build\shengine\externals\shgui\Debug\export-shaders.exe
  glfw.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\build\shengine\externals\glfw\src\Debug\glfw3.lib
  Building Custom Rule C:/Users/sino3/Desktop/shci-test/shengine/shengine/shengine/externals/json-c/CMakeLists.txt
  json-c.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\build\shengine\externals\json-c\Debug\json-c.lib
  plyimporter.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\plyimporter.lib
  shvulkan.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\shvulkan.lib
  shfd.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\shfd.lib
  shgui.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\shgui.lib
  shserial.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\shengine\externals\shserial\bin\Debug\shserial.lib
  shsharedhost.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\build\shengine\Debug\shsharedhost.lib
  shthreads.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\shthreads.lib
  shengine.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\shengine.lib
  Building Custom Rule C:/Users/sino3/Desktop/shci-test/shengine/shengine/simulations/CMakeLists.txt
  flappy-circle.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\flappy-circle.dll
  shecs-example.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\shecs-example.exe
  sheditor.vcxproj -> C:\Users\sino3\Desktop\shci-test\shengine\shengine\bin\Debug\sheditor.exe
  Building Custom Rule C:/Users/sino3/Desktop/shci-test/shengine/shengine/CMakeLists.txt

```

---

    