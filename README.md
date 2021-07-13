# Foggy-Engine

## Build status: 

Linux: [![Build Status](https://travis-ci.com/MrSinho/Foggy-Engine.svg?token=WEQzvPex7Gf2cPScBckx&branch=main)](https://travis-ci.com/MrSinho/Foggy-Engine)

The engine has been tested on Windows 10, Linux Mint (virtual machine) and Ubuntu on different hardware configurations

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

In case your system does not support Vulkan 1.2 use the commands `cmake -DFGG_USE_VULKAN_1_1=ON` or `cmake -DFGG_USE_VULKAN_1_0=ON` 

### Binaries and output

the binaries

## Tutorial:
no