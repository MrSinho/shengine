# shengine

![](https://img.shields.io/badge/Sinho_softworks-0080FF?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
[![](https://img.shields.io/badge/GitHub_repository-000000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/mrsinho/shengine)

[TOC]


## Overview

A cross-platform Vulkan engine made for personal projects written completely in C. It's not a plug-and-play 3d engine, but a series of libraries and programs built to speed-up the creation of vulkan based applications and applications in general. The engine is on active development.

The engine is frequently being tested on Windows 11, Linux Mint (virtual machine and pc) with different compilers (`MSVC`, `gcc`), and hardware configurations (`RX580 4GB GDDR5`, `Radeon V Carrizo 500MB`).

![](/.shci/linux/linux-exit-code.svg)
![](/.shci/windows/windows-exit-code.svg)

![](https://img.shields.io/badge/Written_in_C-0080FF?style=for-the-badge&logo=c&logoColor=white&labelColor=0030AA#.svg)
![](https://img.shields.io/badge/Compatible_with_C%2b%2b-0080FF?style=for-the-badge&logo=c%2b%2b&logoColor=white&labelColor=0030AA#.svg)

![](docs/media/noise-5.png)


## Notes
 - Python 3.9 is required to generate the project files
 - It's better for me to stick with a few and simple examples: `triangle` and `noise` 
 - Dismissed `flappy-circle`, `application-sample`, `serial-sample`
 - Texture examples to do
 - Audio manipulation library to do (for gaia-universe-model soundtrack)
 - ply export (to export gaia-universe-model data to something like blender)
 - speed up shCopyBuffer, avoid mapping and unmapping memory every time

![](docs/media/coulomb.png)

## Extended documentation

See the `Related pages` for some basic documentation and tutorials. The engine collects many independent libraries, which are bound to their own documentation.

## Features
 - By default double buffering.
 - Depth buffer and multisample anti-aliasing support by default.
 - Color and alpha blending optional support.
 - Native scripting in C (see `Related pages`).
 - Multithreading using [`shthreads`](https://github.com/mrsinho/shthreads) (not tested with new features).
 - Immediate mode simple UI creation using [`shgui`](https://mrsinho.github.io/shgui-docs).
 - Timing features, ticks, and profiling.
 - Serial communication with [`shserial`](https://github.com/mrsinho/shserial).
 - Ply mesh loader using [`plyimporter`](https://github.com/mrsinho/plyimporter).
 - Compute queues and pipelines (need to make an example).
 - CMake project generation by running the `export-application.py` program (see `Related pages`).
 - Serialization and parsing of [`.smd`](https://github.com/mrsinho/smd) files (see `Related pages`).
 - Vulkan API implementation using [`shvulkan`](https://github.com/mrsinho/shvulkan). The responsibility of the developer stands on creating a pipeline building system and GPU calls. Command buffer synchronization for swapchain images and presentation is automatically handled by the engine.

## Contributions and support

Should you encounter any issues with the library and tools or wish to introduce new features, I welcome your contributions to the repository. You can contribute by either opening an issue or submitting a pull request.

Should you find value in the provided resources or have enjoyed using the project, your support in the form of a tip would be greatly appreciated. It goes a long way in sustaining and enhancing the development efforts. Thank you for considering!

[![](https://img.shields.io/badge/Buy_Me_A_Coffee-FFDD00?style=for-the-badge&logo=buy-me-a-coffee&logoColor=black)](https://www.buymeacoffee.com/mrsinho)
![](https://img.shields.io/badge/Sinho_softworks-0080FF?style=for-the-badge&logo=&logoColor=white&labelColor=990042)