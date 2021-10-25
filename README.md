# Foggy-Engine

## Build status: 

The engine has been tested on Windows 10, Linux Mint (virtual machine) and Ubuntu with different compilers (`MSVC`, `gcc`), and hardware configurations (`RX580 4GB GDDR5`, `Radeon V Carrizo 500MB`).

![test0](Saved/Pictures/Ive_been_to_Verdun.png)

## Current features (tested):
 - Entity Component System using [FggECS](https://github.com/MrSinho/FggECS) by MrSinho
 - Material creation and usage
 - Ply mesh loader using [PlyImporter](https://github.com/MrSinho/PlyImporter) by MrSinho
 - Indexed and not indexed drawing
 - Uniform buffer object support
 - Real time scene descriptor loader
 - Custom shader handling (more about that in the Coulomb shader and example)

## To do:
 - Depth buffering
 - Double/triple buffering
 - Textures
 - Shader system sucks
 - Scripting (native pls)

## Prerequisites

Make sure you've installed the vulkan sdk on your machine and the required development libraries for glfw on linux. 

```bash
bash linuxPrerequisites.sh
```

## Build from source

### Clone repository

```bash
git clone --recursive https://github.com/MrSinho/Foggy-Engine
cd Foggy-Engine
mkdir build && cd build
``` 

### Generate projects

Use cmake:
```bash
cmake ..
cmake --build .
```

In case your system does not support Vulkan 1.2, use the command `cmake -DFGG_USE_VULKAN_1_1=ON` or `cmake -DFGG_USE_VULKAN_1_0=ON` depending on the api version you're going to use.

### Binaries and output

The built binaries are in `bin`.