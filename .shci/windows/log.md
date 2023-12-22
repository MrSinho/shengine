
# Windows build logs

![](windows-exit-code.svg)

```
system        : Windows
version       : 10.0.22621
platform      : Windows-10-10.0.22621-SP0
processor     : AMD64 Family 23 Model 17 Stepping 0, AuthenticAMD
min frequency : 0.0 MHz
max frequency : 3500.0 MHz
cores         : 4
```

build ran for `71.30 s` and terminated with exit code `0`

---

```bash
> .shci/windows/prerequisites-output.txt (
    choco install -y curl mingw cmake
    mingw-get install gcc mingw32-make gdb
)
```

```bash
Chocolatey v2.0.0
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
> .shci/windows/build-output.txt (
    python export-application.py name=triangle target=EXECUTABLE path=applications/triangle
    python export-application.py name=noise target=EXECUTABLE path=applications/noise
    cd applications/triangle/windows/build
    cmake --build .
    cd ../../../noise/windows/build
    cmake --build .
)

```

```bash
-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: D:/lib/VulkanSDK/1.3.250.0/Include
-- shvulkan message: Vulkan_LIBRARY:     D:/lib/VulkanSDK/1.3.250.0/Lib/vulkan-1.lib
-- Found Doxygen: D:/bin/doxygen/bin/doxygen.exe (found version "1.9.8") found components: doxygen 
-- Including Win32 support
-- Found Doxygen: D:/bin/doxygen/bin/doxygen.exe (found version "1.9.8") found components: doxygen missing components: dot
-- Configuring D:/home/desktop/github/shengine/docs/Doxyfile from D:/home/desktop/github/shengine/docs/Doxyfile.in
-- Creating custom docs command: D:/bin/doxygen/bin/doxygen.exe D:/home/desktop/github/shengine/docs/Doxyfile
-- Configuring done (0.7s)
-- Generating done (5.9s)
-- Build files have been written to: D:/home/desktop/github/shengine/applications/triangle/windows/build
--------------------------------------------------------------------------------
EXAMPLE CALL: python export-application.py name=noise target=SHARED
EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise
EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles
EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
platform:       windows
python_src_dir: D:/home/desktop/github/shengine
name:           triangle
path:           D:/home/desktop/github/shengine/applications/triangle
target_type:    EXECUTABLE
generator:      
clean-cache:    
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading D:/home/desktop/github/shengine/applications/triangle/source-files.txt
src files:
	src/triangle.c

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading D:/home/desktop/github/shengine/applications/triangle/libs.txt
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
        	D:/home/desktop/github/shengine/applications/triangle/src/triangle.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	D:/home/desktop/github/shengine/applications/triangle/src/triangle.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${SH_APPLICATION_NAME} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
    )
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	D:/home/desktop/github/shengine/applications/triangle/src/triangle.c

        D:/home/desktop/github/shengine/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC D:/home/desktop/github/shengine/collection/sheditor/include)
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
writing to CMake file: D:/home/desktop/github/shengine/applications/triangle/CMakeLists.txt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Os path: D:/home/desktop/github/shengine/applications/triangle/windows
Build path: D:/home/desktop/github/shengine/applications/triangle/windows/build

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
creating build directory: D:/home/desktop/github/shengine/applications/triangle/windows/build
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
running command:
	cd D:/home/desktop/github/shengine/applications/triangle/windows/build && cmake D:/home/desktop/github/shengine -DSH_ENGINE_BUILD_EDITOR=ON -DSH_APPLICATION_PATH=D:/home/desktop/github/shengine/applications/triangle -DSH_APPLICATION_NAME=triangle -DSH_APPLICATION_BINARY_TYPE=EXECUTABLE 
--------------------------------------------------------------------------------

-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: D:/lib/vulkan-sdk-1.3.261.1/Include
-- shvulkan message: Vulkan_LIBRARY:     D:/lib/vulkan-sdk-1.3.261.1/Lib/vulkan-1.lib
-- Found Doxygen: D:/bin/doxygen/bin/doxygen.exe (found version "1.9.8") found components: doxygen 
-- Including Win32 support
-- Found Doxygen: D:/bin/doxygen/bin/doxygen.exe (found version "1.9.8") found components: doxygen missing components: dot
-- Configuring D:/home/desktop/github/shengine/docs/Doxyfile from D:/home/desktop/github/shengine/docs/Doxyfile.in
-- Creating custom docs command: D:/bin/doxygen/bin/doxygen.exe D:/home/desktop/github/shengine/docs/Doxyfile
-- Configuring done (0.7s)
-- Generating done (4.5s)
-- Build files have been written to: D:/home/desktop/github/shengine/applications/noise/windows/build
--------------------------------------------------------------------------------
EXAMPLE CALL: python export-application.py name=noise target=SHARED
EXAMPLE CALL: python export-application.py name=noise target=STATIC path=applications/noise
EXAMPLE CALL: python export-application.py name=noise target=SHARED path=applications/noise generator=MinGW-Makefiles
EXAMPLE CALL: python export-application.py name=noise target=EXECUTABLE path=applications/noise generator=MinGW-Makefiles CLEAN-CACHE
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
platform:       windows
python_src_dir: D:/home/desktop/github/shengine
name:           noise
path:           D:/home/desktop/github/shengine/applications/noise
target_type:    EXECUTABLE
generator:      
clean-cache:    
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading D:/home/desktop/github/shengine/applications/noise/source-files.txt
src files:
	src/noise.c

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
loading D:/home/desktop/github/shengine/applications/noise/libs.txt
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
        	D:/home/desktop/github/shengine/applications/noise/src/noise.c

    )
    
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "SHARED")
    add_library(${SH_APPLICATION_NAME} SHARED 
        	D:/home/desktop/github/shengine/applications/noise/src/noise.c

    )
    target_compile_definitions(shengine PUBLIC SH_APPLICATION_TARGET_TYPE_SHARED=1)
    set_target_properties(${SH_APPLICATION_NAME} sheditor PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/ 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/windows/bin/
    )
elseif("${SH_APPLICATION_BINARY_TYPE}" STREQUAL "EXECUTABLE")
    add_executable(${SH_APPLICATION_NAME}  
        	D:/home/desktop/github/shengine/applications/noise/src/noise.c

        D:/home/desktop/github/shengine/collection/sheditor/src/shEditor.c
    )
    target_include_directories(${SH_APPLICATION_NAME} PUBLIC D:/home/desktop/github/shengine/collection/sheditor/include)
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
writing to CMake file: D:/home/desktop/github/shengine/applications/noise/CMakeLists.txt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Os path: D:/home/desktop/github/shengine/applications/noise/windows
Build path: D:/home/desktop/github/shengine/applications/noise/windows/build

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
creating build directory: D:/home/desktop/github/shengine/applications/noise/windows/build
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
running command:
	cd D:/home/desktop/github/shengine/applications/noise/windows/build && cmake D:/home/desktop/github/shengine -DSH_ENGINE_BUILD_EDITOR=ON -DSH_APPLICATION_PATH=D:/home/desktop/github/shengine/applications/noise -DSH_APPLICATION_NAME=noise -DSH_APPLICATION_BINARY_TYPE=EXECUTABLE 
--------------------------------------------------------------------------------

Versione di MSBuild L17.7.2+d6990bcfa per .NET Framework

  cglm.vcxproj -> D:\home\desktop\github\shengine\applications\triangle\windows\build\collection\externals\cglm\Debug\cglm.lib
  glfw.vcxproj -> D:\home\desktop\github\shengine\applications\triangle\windows\build\collection\externals\glfw\src\Debug\glfw3.lib
  plyimporter.vcxproj -> D:\home\desktop\github\shengine\bin\Debug\plyimporter.lib
  shVulkan.c
D:\home\desktop\github\shengine\collection\externals\shvulkan\shvulkan\include\shvulkan/shVulkan.h(10,10): fatal  error C1083: Non Š possibile aprire il file inclusione: 'vulkan/vulkan.h': No such file or directory [D:\home\desktop\github\shengine\applications\triangle\windows\build\collection\externals\shvulkan\shvulkan.vcxproj]
  smd.vcxproj -> D:\home\desktop\github\shengine\bin\Debug\smd.lib
  shthreads.vcxproj -> D:\home\desktop\github\shengine\bin\Debug\shthreads.lib
  shserial.vcxproj -> D:\home\desktop\github\shengine\collection\externals\shserial\bin\Debug\shserial.lib
Versione di MSBuild L17.7.2+d6990bcfa per .NET Framework

  cglm.vcxproj -> D:\home\desktop\github\shengine\applications\noise\windows\build\collection\externals\cglm\Debug\cglm.lib
  glfw.vcxproj -> D:\home\desktop\github\shengine\applications\noise\windows\build\collection\externals\glfw\src\Debug\glfw3.lib
  shvulkan.vcxproj -> D:\home\desktop\github\shengine\bin\Debug\shvulkan.lib
  smd.vcxproj -> D:\home\desktop\github\shengine\bin\Debug\smd.lib
  shEnvironment.c
  shenvironment.vcxproj -> D:\home\desktop\github\shengine\applications\noise\windows\build\collection\OFF\windows\Debug\shenvironment.lib
  shthreads.vcxproj -> D:\home\desktop\github\shengine\bin\Debug\shthreads.lib
  shApplicationHost.c
  shapplication-host.vcxproj -> D:\home\desktop\github\shengine\applications\noise\windows\build\collection\OFF\windows\Debug\shapplication-host.lib
  shserial.vcxproj -> D:\home\desktop\github\shengine\collection\externals\shserial\bin\Debug\shserial.lib
  shEngine.c
  shInput.c
  shLinearAlgebra.c
  shProfiling.c
  shScene.c
  shTime.c
  shWindow.c
  Generazione del codice in corso...
  shengine.vcxproj -> D:\home\desktop\github\shengine\applications\noise\windows\build\collection\OFF\windows\Debug\shengine.lib
  noise.c
  shEditor.c
  Generazione del codice in corso...
  noise.vcxproj -> D:\home\desktop\github\shengine\applications\noise\windows\bin\Debug\noise.exe
  plyimporter.vcxproj -> D:\home\desktop\github\shengine\bin\Debug\plyimporter.lib
  shEditor.c
  sheditor.vcxproj -> D:\home\desktop\github\shengine\applications\noise\windows\build\collection\OFF\windows\Debug\sheditor.exe

```

---
    
build ran for `71.30 s` and terminated with exit code `0`

---

