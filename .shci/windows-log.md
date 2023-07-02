
# shengine

![windows-badge](windows-exit_code.svg)

## [windows build logs:](https://github.com/mrsinho/shci)



Build ran for `77.99s`

---

```bash
>> .shci/windows/prerequisites-output.txt (
    choco install -y curl mingw cmake
    mingw-get install gcc mingw32-make gdb
)
```

```bash
Chocolatey v2.0.0
Installing the following packages:
curl;mingw;cmake
By installing, you accept licenses for the packages.
curl v8.1.2 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

Chocolatey installed 0/3 packages. 
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).

Warnings:
 - cmake - cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - curl - curl v8.1.2 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - mingw - mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

```

---

    
```bash
>> .shci/windows/build-output.txt (
    cd shengine/
    python export-application.py name=triangle target=EXECUTABLE path=applications/triangle
    python export-application.py name=noise target=EXECUTABLE path=applications/noise
    cd applications/triangle/windows/build
    cmake --build .
    cd ../../../noise/windows/build
    cmake --build .
)

```

```bash
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- The C compiler identification is MSVC 19.36.32532.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.36.32532/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Found Vulkan: D:/lib/VulkanSDK/1.3.250.0/Lib/vulkan-1.lib (found version "1.3.250") found components: glslc glslangValidator 
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: D:/lib/VulkanSDK/1.3.250.0/Include
-- shvulkan message: Vulkan_LIBRARY:     D:/lib/VulkanSDK/1.3.250.0/Lib/vulkan-1.lib
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE  
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including Win32 support
-- Configuring done (8.4s)
-- Generating done (0.3s)
-- Build files have been written to: D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows/build
--------------------------------------------------------------------------------
EXAMPLE CALL: python export-application.py name=noise target=SHARED
EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise
EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles
EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
platform:       windows
python_src_dir: D:/home/desktop/shci-tests/shci-test/shengine
name:           triangle
path:           D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle
target_type:    EXECUTABLE
generator:      
clean-cache:    
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/source-files.txt
src files:
	src/triangle.c

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/libs.txt
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
        	D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/src/triangle.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/src/triangle.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${SH_APPLICATION_NAME} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
    )
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/src/triangle.c

        D:/home/desktop/shci-tests/shci-test/shengine/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC D:/home/desktop/shci-tests/shci-test/shengine/collection/sheditor/include)
    target_compile_definitions(${SH_APPLICATION_NAME} PUBLIC SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1)
    set_target_properties(${SH_APPLICATION_NAME} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
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
writing to CMake file: D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/CMakeLists.txt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Os path: D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows
Build path: D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows/build

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
creating build directory: D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows/build
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
running command:
	cd D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows/build && cmake D:/home/desktop/shci-tests/shci-test/shengine -DSH_ENGINE_BUILD_EDITOR=ON -DSH_APPLICATION_PATH=D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle -DSH_APPLICATION_NAME=triangle -DSH_APPLICATION_BINARY_TYPE=EXECUTABLE 
--------------------------------------------------------------------------------

-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- The C compiler identification is MSVC 19.36.32532.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.36.32532/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Found Vulkan: D:/lib/VulkanSDK/1.3.250.0/Lib/vulkan-1.lib (found version "1.3.250") found components: glslc glslangValidator 
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: D:/lib/VulkanSDK/1.3.250.0/Include
-- shvulkan message: Vulkan_LIBRARY:     D:/lib/VulkanSDK/1.3.250.0/Lib/vulkan-1.lib
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE  
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including Win32 support
-- Configuring done (6.1s)
-- Generating done (0.3s)
-- Build files have been written to: D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows/build
--------------------------------------------------------------------------------
EXAMPLE CALL: python export-application.py name=noise target=SHARED
EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise
EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles
EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
platform:       windows
python_src_dir: D:/home/desktop/shci-tests/shci-test/shengine
name:           noise
path:           D:/home/desktop/shci-tests/shci-test/shengine/applications/noise
target_type:    EXECUTABLE
generator:      
clean-cache:    
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/source-files.txt
src files:
	src/noise.c

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/libs.txt
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
        	D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/src/noise.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/src/noise.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${SH_APPLICATION_NAME} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
    )
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/src/noise.c

        D:/home/desktop/shci-tests/shci-test/shengine/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC D:/home/desktop/shci-tests/shci-test/shengine/collection/sheditor/include)
    target_compile_definitions(${SH_APPLICATION_NAME} PUBLIC SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1)
    set_target_properties(${SH_APPLICATION_NAME} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
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
writing to CMake file: D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/CMakeLists.txt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Os path: D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows
Build path: D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows/build

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
creating build directory: D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows/build
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
running command:
	cd D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows/build && cmake D:/home/desktop/shci-tests/shci-test/shengine -DSH_ENGINE_BUILD_EDITOR=ON -DSH_APPLICATION_PATH=D:/home/desktop/shci-tests/shci-test/shengine/applications/noise -DSH_APPLICATION_NAME=noise -DSH_APPLICATION_BINARY_TYPE=EXECUTABLE 
--------------------------------------------------------------------------------

MSBuild version 17.6.3+07e294721 for .NET Framework

  Checking Build System
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/cglm/CMakeLists.txt
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
  Generazione del codice in corso...
  Compilazione in corso...
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
  Generazione del codice in corso...
     Creazione della libreria D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows/build/collection/externals/cglm/Debug/cglm.lib e dell'oggetto D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows/build/collection/externals/cglm/Debug/cglm.exp
  cglm.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\triangle\windows\build\collection\externals\cglm\Debug\cglm-0.dll
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/glfw/src/CMakeLists.txt
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
  Generazione del codice in corso...
  Compilazione in corso...
  wgl_context.c
  Generazione del codice in corso...
  glfw.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\triangle\windows\build\collection\externals\glfw\src\Debug\glfw3.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/plyimporter/CMakeLists.txt
  plyImporter.c
  plyimporter.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\plyimporter.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/shvulkan/CMakeLists.txt
  shVulkan.c
  shvulkan.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shvulkan.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/smd/CMakeLists.txt
  smd.c
  smd.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\smd.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/CMakeLists.txt
  shEnvironment.c
  shFileUtilities.c
  Generazione del codice in corso...
  shenvironment.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shenvironment.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/shserial/CMakeLists.txt
  shSerial.c
  shserial.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\collection\externals\shserial\bin\Debug\shserial.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/CMakeLists.txt
  shSharedHost.c
  shsharedhost.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\triangle\windows\build\collection\Debug\shsharedhost.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/shthreads/CMakeLists.txt
  shthreads.c
  shthreads.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shthreads.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/CMakeLists.txt
  shEngine.c
  shInput.c
  shTime.c
  shWindow.c
  shLinearAlgebra.c
  shScene.c
  Generazione del codice in corso...
  shengine.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shengine.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/CMakeLists.txt
  shEditor.c
  sheditor.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\sheditor.exe
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/CMakeLists.txt
  triangle.c
  shEditor.c
  Generazione del codice in corso...
     Creazione della libreria D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows/bin/Debug/triangle.lib e dell'oggetto D:/home/desktop/shci-tests/shci-test/shengine/applications/triangle/windows/bin/Debug/triangle.exp
  triangle.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\triangle\windows\bin\Debug\triangle.exe
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/CMakeLists.txt
MSBuild version 17.6.3+07e294721 for .NET Framework

  Checking Build System
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/cglm/CMakeLists.txt
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
  Generazione del codice in corso...
  Compilazione in corso...
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
  Generazione del codice in corso...
     Creazione della libreria D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows/build/collection/externals/cglm/Debug/cglm.lib e dell'oggetto D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows/build/collection/externals/cglm/Debug/cglm.exp
  cglm.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\noise\windows\build\collection\externals\cglm\Debug\cglm-0.dll
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/glfw/src/CMakeLists.txt
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
  Generazione del codice in corso...
  Compilazione in corso...
  wgl_context.c
  Generazione del codice in corso...
  glfw.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\noise\windows\build\collection\externals\glfw\src\Debug\glfw3.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/shvulkan/CMakeLists.txt
  shVulkan.c
  shvulkan.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shvulkan.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/smd/CMakeLists.txt
  smd.c
  smd.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\smd.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/CMakeLists.txt
  shEnvironment.c
  shFileUtilities.c
  Generazione del codice in corso...
  shenvironment.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shenvironment.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/shserial/CMakeLists.txt
  shSerial.c
  shserial.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\collection\externals\shserial\bin\Debug\shserial.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/CMakeLists.txt
  shSharedHost.c
  shsharedhost.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\noise\windows\build\collection\Debug\shsharedhost.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/shthreads/CMakeLists.txt
  shthreads.c
  shthreads.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shthreads.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/CMakeLists.txt
  shEngine.c
  shInput.c
  shTime.c
  shWindow.c
  shLinearAlgebra.c
  shScene.c
  Generazione del codice in corso...
  shengine.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shengine.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/CMakeLists.txt
  noise.c
  shEditor.c
  Generazione del codice in corso...
     Creazione della libreria D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows/bin/Debug/noise.lib e dell'oggetto D:/home/desktop/shci-tests/shci-test/shengine/applications/noise/windows/bin/Debug/noise.exp
  noise.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\noise\windows\bin\Debug\noise.exe
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/externals/plyimporter/CMakeLists.txt
  plyImporter.c
  plyimporter.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\plyimporter.lib
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/collection/CMakeLists.txt
  shEditor.c
  sheditor.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\sheditor.exe
  Building Custom Rule D:/home/desktop/shci-tests/shci-test/shengine/CMakeLists.txt

```

---

    

Build terminated with exit code 0

---

