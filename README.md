# Foggy-Engine

## Build status: 

Linux: [![Build Status](https://travis-ci.com/MrSinho/Foggy-Engine.svg?token=WEQzvPex7Gf2cPScBckx&branch=main)](https://travis-ci.com/MrSinho/Foggy-Engine)

The engine has been tested on Windows 10, Linux Mint (virtual machine) and Ubuntu with different compilers (`MSVC`, `gcc`), and hardware configurations (`RX580 4GB GDDR5`, `Radeon V Carrizo 500MB`).

![test0](Saved/Pictures/test0.gif)

## Current features (tested):
 - Entity Component System using [EZ-ECS](https://github.com/MrSinho/EZ-ECS) by MrSinho
 - Material creation and usage
 - Ply mesh loader using [PlyImporter](https://github.com/MrSinho/PlyImporter) by MrSinho
 - Indexed and not indexed drawing
 - Uniform buffer object support
 - Dynamic mesh

## To do:
 - Depth buffering
 - Double/triple buffering
 - Runtime scene editor
 - Scene loader

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

## Possible export binary format:

 * define max entities and max components
 * create scene matrix EZ_ECS_MAX_ENTITIES * EZ_ECS_MAX_COMPONENTS
    * write a binary section defining the component index and size
    * write the actual data 
    * everything should be exported to the binary file (for example mesh vertices), exception for vulkan specific structures
    * that's a reason why you have to specify the structure offset and size you want to write and discard what is not needed
    


### Binaries and output

The built binaries are in `bin`.

## Tutorial:

no