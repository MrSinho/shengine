# shengine

[Build status](#build-status)

[![linux_badge](.shci/linux-exit_code.svg)](.shci/linux-log.md)
[![windows_badge](.shci/windows-exit_code.svg)](.shci/windows-log.md)

![noise-3](saved/pictures/noise-3.png)


[Current features](#current-features)
* [To do](#to-do)

[Build from source](#build-from-source)
* [Engine targets only](#engine-targets-only)
* [simulation-sample](#simulation-sample)
* [flappy-circle](#flappy-circle)
* [serial-demo](#serial-demo)
   * [Pinout for the Raspberry Pi pico and UF2 binary](#pinout-for-the-raspberry-pi-pico-and-uf2-binary)
* [noise](#noise)
* [Switch between applications in runtime](#switch-between-applications-in-runtime)
---

# [Build status](./shci)

[![linux_badge](.shci/linux-exit_code.svg)](.shci/linux-log.md)
[![windows_badge](.shci/windows-exit_code.svg)](.shci/windows-log.md)

The engine has been tested on Windows 10, Linux Mint (virtual machine) and Ubuntu with different compilers (`MSVC`, `gcc`), and hardware configurations (`RX580 4GB GDDR5`, `Radeon V Carrizo 500MB`).

![coulomb](saved/pictures/flappy-circle.png)

# Current features:
 - Immediate mode gui library embedded with the engine (see [shgui](https://github.com/mrsinho/shgui))
 - Multithreading using [shthreads](https://github.com/mrsinho/shthreads)
    * Main engine thread
    * the "`load`" can safely read and write memory at `void* ShEngine::p_engine_extension` without the need of synchronization objects: the memory pointed is reserved for the simulation modules. However, using the extension memory inside the default functions ("`ShEngine::ShSharedHost::p_start`", "`ShEngine::ShSharedHost::p_update`" "`ShEngine::ShSharedHost::p_close`" etc.) won't be thread safe without synchronization objects. You can easily set up a mutex using `ShThreads` library. The engine will still have to wait the end of the thread to complete the release operations. 
 - Entity Component System using [shecs](https://github.com/MrSinho/shecs).
 - Ply mesh loader using [plyimporter](https://github.com/MrSinho/plyimporter).
 - Depth buffer support.
 - Serial communication using [shserial](https://github.com/MrSinho/shserial).
 - Scene customization by setting up a `scene.json` file.
    * Do do: delete material parameters, makes no sense.
 - Glsl shader customization.
    * define shader general properties in materials.json, the engine will automatically set up the boilerplate code.
    * deal with vulkan objects, draw calls and material pipelines from the scriptable modules.
    * set up shader input parameters for each entity (if required) in scene.json [--> not reccomended, i will delete that option].
    * material extension structures support [--> also not reccomended, i would like to delete that feature too].
 - Native scripting in C:
    * `loader.ini` is in the same directory of the main executable, and specifies the assets path.
    * `simulation.json` sets up some properties for the specified shared library, which functions are called at the start of the main thread, in runtime, and at the end of the application.
    * to generate the cmake target given the external application name, run the `export-simulation.py` script.

## To do:
 * External launcher
 * Improve [shgui](https://github.com/mrsinho/shgui)
 * Create compute shaders from descriptors
 * Textures
 * Audio

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

## simulation-sample

Just a random scene.

![simulation-sample](saved/pictures/simulation-sample.png)

Using Windows:
```batch
python export-simulation.py "simulation-sample" SHARED
cd windows-builds/simulation-sample
cmake --build .
```

Using Linux:
```batch
python export-simulation.py "simulation-sample" SHARED
cd linux-builds/simulation-sample
cmake --build .
```

Write at `bin/loader.ini`:
```batch
python set-simulation.py "simulation-sample" SHARED
```

---

## flappy-circle

A Flappy Bird clone in a circular map. Press `SPACE` to let it survive.

![flappy-circle](saved/pictures/flappy-circle.png)

Using Windows:
```batch
python export-simulation.py "flappy-circle" SHARED
cd windows-builds/flappy-circle
cmake --build .
```

Using Linux:
```batch
python export-simulation.py "flappy-circle" SHARED
cd linux-builds/flappy-circle
cmake --build .
```

Write at `bin/loader.ini`:
```batch
python set-simulation.py "flappy-circle" SHARED
```

---

## serial-demo

Any variation of analog input for the Raspberry Pi Pico affects the lighting in the scene.

![serial-demo](saved/pictures/serial-demo.png)

Using Windows:
```batch
python export-simulation.py "serial-demo" SHARED
cd windows-builds/serial-demo
cmake --build .
```

Using Linux:
```batch
python export-simulation.py "serial-demo" SHARED
cd linux-builds/serial-demo
cmake --build .
```

Write at `bin/loader.ini`:
```batch
python set-simulation.py "serial-demo" SHARED
```

Note: because the simulation does not include multithreading, reading serial data blocks all gpu calls.

## Pinout for the Raspberry Pi Pico and UF2 binary
It's connected to a `1.5V` solar panel (it could be a potentiometer or any analogic sensor). The negative charged cable (in red) is connected to `ADC input 0`, `GPIO 26` (for pinout check [saved/pictures/serial-demo-pinout.jpg](saved/pictures/serial-demo-pinout.jpg)). The program to run on the Raspberry Pi Pico is located at [simulations/serial-demo/pico-bin/shengine_sample_raw.uf2](simulations/serial-demo/pico-bin/shengine_sample_raw.uf2).

You should correct the serial port name in case the one specified at [simulations/serial-demo/src/serial-demo.c](simulations/serial-demo/src/serial-demo.c) doesn't match.

---

## noise

![noise-3](saved/pictures/noise-3.png)

Using Windows:
```batch
python export-simulation.py "noise" SHARED
cd windows-builds/noise
cmake --build .
```

Using Linux:
```batch
python export-simulation.py "noise" SHARED
cd linux-builds/noise
cmake --build .
```

Write at `bin/loader.ini`:
```batch
python set-simulation.py "noise" SHARED
```

Press `H` to hide the GUI and get a full view of the shaded plane. To change the values of the parameters `A` `B` and `S`:

||||
|-------------|----------------|----------------|
| Parameter   | Increase key   | Decrease key   |
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



# Binaries and output

The built targets are in the `bin` directory.

# Switch between applications in runtime

Use the python script [`set-simulation.py`](set-simulation.py) or edit manually the `loader.ini` file located at the `bin` directory, then press `LEFT_CTRL + R` to reload the engine editor:

```batch
python set-simulation.py noise
```

Produces:

```
noise
../simulations/noise/assets/
```
Where the first line is used for comment purposes, while `../simulations/noise/assets/` (second line) corresponds to the relative assets path. 