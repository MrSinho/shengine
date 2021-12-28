# SH-Engine

## [Build status](https://github.com/MrSinho/Foggy-Engine/tree/main/.ShCI)

[![linux_badge](.ShCI/linux-status.svg)](https://github.com/MrSinho/Sh-Engine/tree/main/.ShCI/linux-log.md)
[![windows_badge](.ShCI/windows-status.svg)](https://github.com/MrSinho/Sh-Engine/tree/main/.ShCI/windows-log.md)

The engine has been tested on Windows 10, Linux Mint (virtual machine) and Ubuntu with different compilers (`MSVC`, `gcc`), and hardware configurations (`RX580 4GB GDDR5`, `Radeon V Carrizo 500MB`).

![test0](Saved/Pictures/coulomb.png)

## Current features (tested):
 - Entity Component System using [ShECS](https://github.com/MrSinho/ShECS) by MrSinho
 - Material creation and usage
 - Ply mesh loader using [PlyImporter](https://github.com/MrSinho/PlyImporter) by MrSinho
 - Indexed and not indexed drawing
 - Uniform buffer object support
 - Real time scene descriptor loader
 - Custom shader handling (more about that in the Coulomb shader and example)
 - Depth buffer
 
## To do:
 - Double/triple buffering
 - Textures
 - Shader system sucks
 - Scripting (native preferred)
 - Audio
 - Import/export scene binaries
 - Edit shortcuts when navigating in the window

## Prerequisites

Make sure you've installed the vulkan sdk on your machine and the required development libraries for GLFW and Vulkan on Linux. 

```bash
bash linuxPrerequisites.sh
```

## Build from source

### Clone repository

```bash
git clone --recursive https://github.com/MrSinho/SH-Engine
cd SH-Engine
``` 

### Generate projects

On Windows:
```batch
type configure.txt|cmd
```

On Linux:
```bash
bash configure.txt
```

Use cmake:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

In case your system does not support Vulkan 1.2, use the command `cmake -DSH_USE_VULKAN_1_1=ON` or `cmake -DSH_USE_VULKAN_1_0=ON` depending on the api version you're going to use.

### Binaries and output

The built binaries are in `bin`.
