# shengine

[Build status](#build-status)

[![linux_badge](.shci/linux-exit_code.svg)](.shci/linux-log.md)
[![windows_badge](.shci/windows-exit_code.svg)](.shci/windows-log.md)

![noise-3](saved/pictures/noise-3.png)


[Current features](#current-features)
* [To do](#to-do)

[Build from source](#build-from-source)
* [Engine targets only](#engine-targets-only)
* [noise](#noise)
* [Switch between applications in runtime](#switch-between-applications-in-runtime)
---

DISMISSED FLAPPY CIRCLE SERIAL-DEMO APPLICATION-SAMPLE

# [Build status](./shci)

[![linux_badge](.shci/linux-exit_code.svg)](.shci/linux-log.md)
[![windows_badge](.shci/windows-exit_code.svg)](.shci/windows-log.md)

The engine is frequently being tested on Windows 11, Linux Mint (virtual machine and pc) with different compilers (`MSVC`, `gcc`), and hardware configurations (`RX580 4GB GDDR5`, `Radeon V Carrizo 500MB`).

![coulomb](saved/pictures/flappy-circle.png)

# Notes:
 - Python 3.9 is required to generate the project files
 - It's better to stick with a few and simple examples: `triangle` and `noise` 
 - Dismissed `flappy-circle`, `application-sample`, `serial-sample`

# Current features:
 - Immediate mode gui library embedded with the engine (see [shgui](https://github.com/mrsinho/shgui))
 - By default double buffering on `sheditor`
 - Multithreading using [shthreads](https://github.com/mrsinho/shthreads)
    * Main engine thread
    * Application thrad executed in parallel while the window is running
 - Ply mesh loader using [plyimporter](https://github.com/MrSinho/plyimporter).
 - Depth buffer and multisampling support by default.
 - Serial communication using [shserial](https://github.com/MrSinho/shserial).
 - Native scripting in C
 - Project generation using CMake by running the `export-application.py` program. For more info, see the examples and source code.
- Every application aspect is described using [`.smd`](https://github.com/mrsinho/smd) files
   * `ini.smd`
   * `application.smd`
   * `host-memory.smd`
   * `vulkan-memory.smd`
   * `serial.smd`
   * `scene.smd`
- Vulkan API flexibility using shvulkan and `.smd` files. Quickly allocate memory to the gpu and implement your own graphics/compute pipelines through the main 
## To do:
 * Textures
 * Audio
 * Need to find out something new to add
---

# Build from source

```bash
git clone --recursive https://github.com/mrsinho/shengine
cd shengine
``` 

## Engine targets only

```bash
mkdir engine-build-only
cd engine-build-only
cmake ..
cmake --build .
```

---

## noise

![noise-3](saved/pictures/noise-3.png)

Using Windows:
```batch
python export-application.py name=noise target=EXECUTABLE path=applications/noise CLEAN-CACHE
cd applications/noise/windows/build
cmake --build .
```

Using Linux:
```batch
python export-application.py name=noise target=EXECUTABLE path=applications/noise CLEAN-CACHE
cd applications/noise/linux/build
cmake --build .
```

Remember to copy `applications/noise/ini.smd` to `applications/noise/os/bin/ini.smd` in order to load correctly all the `.smd` files. 

Press `H` to hide the GUI and get a full view of the shaded plane. To change the values of the parameters `A` `B` and `S`:

|Parameter    |Increase key    |Decrease key    |
|-------------|----------------|----------------|
| `S`         |       _Z_      |       _X_      |
| `A`         |       _A_      |       _D_      |
| `B`         |       _W_      |       _S_      |

Fragment shader algorithm:

$$c = \cos(\pi x y s)$$

$$
RGB_{raw} = \begin{bmatrix}
\sin(\pi|x|\cfrac{s}{a}) + c\\
\sin(\pi|y|\cfrac{s}{b}) + c\\
\cos(\pi\cfrac{x}{y}s) + c\\
\end{bmatrix}
$$

---

$a = \cfrac{1}{2}$

$b = 1$

$c = -2,5$

![noise-1](saved/pictures/noise-1.png)

---

$a = \cfrac{1}{2}$

$b = 1$

$c = -2,5$

![noise-2](saved/pictures/noise-2.png)

---

$a = 4$

$b = 4,5$

$c = -1,8$

![noise-3](saved/pictures/noise-3.png)

---

$a = 4$

$b = 5$

$c = 7$

![noise-5](saved/pictures/noise-5.png)

---

# Binaries and output

The built targets are in the `application/app-name/os/bin` directory.

---

# Switch between applications in runtime

If your application has been built as a `SHARED` library instead of an `EXECUTABLE`, you can change the application name at `ini.smd` in order to release the old shared library and load the newly specified application. For example, change:

```
STR1024    ^^ 1    ShEngine::ini_properties.application_name        --> noise                                 *
STR1024    ^^ 1    ShEngine::ini_properties.application_smd_path    --> ../../../noise/smd/application.smd    *
STR1024    ^^ 1    ShEngine::ini_properties.scene_smd_path          --> ../../../noise/smd/scene.smd          *
STR1024    ^^ 1    ShEngine::ini_properties.pipelines_smd_path      --> ../../../noise/smd/pipelines.smd      *
```

to this:

```
STR1024    ^^ 1    ShEngine::ini_properties.application_name        --> triangle                                 *
STR1024    ^^ 1    ShEngine::ini_properties.application_smd_path    --> ../../../triangle/smd/application.smd    *
STR1024    ^^ 1    ShEngine::ini_properties.scene_smd_path          --> ../../../triangle/smd/scene.smd          *
STR1024    ^^ 1    ShEngine::ini_properties.pipelines_smd_path      --> ../../../triangle/smd/pipelines.smd      *
```

