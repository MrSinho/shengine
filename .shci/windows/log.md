
# shengine

![windows-badge](exit_code.svg)

## [windows build logs:](https://github.com/mrsinho/shci)



Build ran for `9.73s`

---

```bash
> .shci/windows/prerequisites-output.txt (
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
> .shci/windows/build-output.txt (
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
-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: D:/lib/VulkanSDK/1.3.250.0/Include
-- shvulkan message: Vulkan_LIBRARY:     D:/lib/VulkanSDK/1.3.250.0/Lib/vulkan-1.lib
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including Win32 support
-- Configuring done (0.2s)
-- Generating done (1.1s)
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

-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: D:/lib/VulkanSDK/1.3.250.0/Include
-- shvulkan message: Vulkan_LIBRARY:     D:/lib/VulkanSDK/1.3.250.0/Lib/vulkan-1.lib
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including Win32 support
-- Configuring done (0.2s)
-- Generating done (0.9s)
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

  cglm.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\triangle\windows\build\collection\externals\cglm\Debug\cglm.lib
  glfw.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\triangle\windows\build\collection\externals\glfw\src\Debug\glfw3.lib
  plyimporter.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\plyimporter.lib
  shvulkan.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shvulkan.lib
  smd.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\smd.lib
  shenvironment.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shenvironment.lib
  shserial.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\collection\externals\shserial\bin\Debug\shserial.lib
  shsharedhost.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\triangle\windows\build\collection\Debug\shsharedhost.lib
  shthreads.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shthreads.lib
  shengine.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shengine.lib
  sheditor.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\sheditor.exe
  triangle.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\triangle\windows\bin\Debug\triangle.exe
MSBuild version 17.6.3+07e294721 for .NET Framework

  cglm.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\noise\windows\build\collection\externals\cglm\Debug\cglm.lib
  glfw.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\noise\windows\build\collection\externals\glfw\src\Debug\glfw3.lib
  shvulkan.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shvulkan.lib
  smd.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\smd.lib
  shenvironment.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shenvironment.lib
  shserial.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\collection\externals\shserial\bin\Debug\shserial.lib
  shsharedhost.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\noise\windows\build\collection\Debug\shsharedhost.lib
  shthreads.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shthreads.lib
  shengine.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\shengine.lib
  noise.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\applications\noise\windows\bin\Debug\noise.exe
  plyimporter.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\plyimporter.lib
  sheditor.vcxproj -> D:\home\desktop\shci-tests\shci-test\shengine\bin\Debug\sheditor.exe

```

---

    

Build terminated with exit code 0

---
