# Generate projects

*_Check the linux and windows build logs:_*

[![linux_badge](../.shci/linux/exit_code.svg)](../.shci/linux/log.md)
[![windows_badge](../.shci/windows/exit_code.svg)](../.shci/windows/log.md)

Be sure to have installed `Python 3.9` or later in order to run the python script without errors. The script generates a CMake file relative to your application and a bash/shell command running CMake. For example, in case you need to generate a Makefiles or a Visual Studio solution for the application located at `application/triangle`, do:

```bash
python3.9 export-application.py name=triangle target=EXECUTABLE path=applications/triangle
```

You can build your application as a single `EXECUTABLE`, which is going to be linked with the engine libraries and embedded with the `shEditor.c` source file. If you build your application as a `SHARED` library, an executable target named `sheditor` will be built. It must be located in the same directory of your shared target, which is going to be linked at runtime.

![clifford](media/clifford.png)

---

## Additional args

The additional argument `"generator"` allows you to specify which build system to select, by choosing between the CMake generators:

```bash
cmake --help
```

```bash
Generators

The following generators are available on this platform (* marks default):
* Visual Studio 17 2022        = Generates Visual Studio 2022 project files.
                                 Use -A option to specify architecture.
  Visual Studio 16 2019        = Generates Visual Studio 2019 project files.
                                 Use -A option to specify architecture.
  Visual Studio 15 2017 [arch] = Generates Visual Studio 2017 project files.
                                 Optional [arch] can be "Win64" or "ARM".
  Visual Studio 14 2015 [arch] = Generates Visual Studio 2015 project files.
                                 Optional [arch] can be "Win64" or "ARM".
  Visual Studio 12 2013 [arch] = Generates Visual Studio 2013 project files.
                                 Optional [arch] can be "Win64" or "ARM".
  Visual Studio 11 2012 [arch] = Deprecated.  Generates Visual Studio 2012
                                 project files.  Optional [arch] can be
                                 "Win64" or "ARM".
  Visual Studio 9 2008 [arch]  = Generates Visual Studio 2008 project files.
                                 Optional [arch] can be "Win64" or "IA64".
  Borland Makefiles            = Generates Borland makefiles.
  NMake Makefiles              = Generates NMake makefiles.
  NMake Makefiles JOM          = Generates JOM makefiles.
  MSYS Makefiles               = Generates MSYS makefiles.
  MinGW Makefiles              = Generates a make file for use with
                                 mingw32-make.
  Green Hills MULTI            = Generates Green Hills MULTI files
                                 (experimental, work-in-progress).
  Unix Makefiles               = Generates standard UNIX makefiles.
  Ninja                        = Generates build.ninja files.
  Ninja Multi-Config           = Generates build-<Config>.ninja files.
  Watcom WMake                 = Generates Watcom WMake makefiles.
  CodeBlocks - MinGW Makefiles = Generates CodeBlocks project files.
  CodeBlocks - NMake Makefiles = Generates CodeBlocks project files.
  CodeBlocks - NMake Makefiles JOM
                               = Generates CodeBlocks project files.
  CodeBlocks - Ninja           = Generates CodeBlocks project files.
  CodeBlocks - Unix Makefiles  = Generates CodeBlocks project files.
  CodeLite - MinGW Makefiles   = Generates CodeLite project files.
  CodeLite - NMake Makefiles   = Generates CodeLite project files.
  CodeLite - Ninja             = Generates CodeLite project files.
  CodeLite - Unix Makefiles    = Generates CodeLite project files.
  Eclipse CDT4 - NMake Makefiles
                               = Generates Eclipse CDT 4.0 project files.
  Eclipse CDT4 - MinGW Makefiles
                               = Generates Eclipse CDT 4.0 project files.
  Eclipse CDT4 - Ninja         = Generates Eclipse CDT 4.0 project files.
  Eclipse CDT4 - Unix Makefiles= Generates Eclipse CDT 4.0 project files.
  Kate - MinGW Makefiles       = Generates Kate project files.
  Kate - NMake Makefiles       = Generates Kate project files.
  Kate - Ninja                 = Generates Kate project files.
  Kate - Unix Makefiles        = Generates Kate project files.
  Sublime Text 2 - MinGW Makefiles
                               = Generates Sublime Text 2 project files.
  Sublime Text 2 - NMake Makefiles
                               = Generates Sublime Text 2 project files.
  Sublime Text 2 - Ninja       = Generates Sublime Text 2 project files.
  Sublime Text 2 - Unix Makefiles
                               = Generates Sublime Text 2 project files.
```

Be careful when choosing the generator to replace the space character with a dash:

```bash
python export-application.py name=triangle target=EXECUTABLE path=applications/triangle generator=Visual-Studio-17-2022
```

If you are running the command for a second time and you need to clear che `CMakeCache.txt` file, use the optional argument `CLEAN-CACHE`:

```bash
python export-application.py name=triangle target=EXECUTABLE path=applications/triangle generator=MinGW-Makefiles CLEAN-CACHE
```

## Compiling

Go to the build directory and run `cmake --build .` or the command you prefer for your generator of choice.

All build files are located at `application-path/os/build`, while libraries and executables are all located at `application-path/os/bin`.

![serial-demo](media/serial-demo.png)

---

## Prerequisites for running the script

```
cd applications/triangle
tree triangle /f
```

```
D:.
│   ini.smd
│   libs.txt
│   source-files.txt
│
├───shaders
│   ├───bin
│   │       mesh.frag.spv
│   │       mesh.vert.spv
│   │
│   └───src
│           mesh.frag
│           mesh.vert
│
├───smd
│       application.smd
│       host-memory.smd
│       scene.smd
│       serial.smd
│       vulkan-memory.smd
│
└───src
        triangle.c
```

## Loading source files

The paths of your application source files are defined at `source-files.txt`. The formatting is very easy: use the endline character to load a new source file.

## External libraries

The `libs.txt` file allows you to add an external CMake file to your application's CMake file and link all the specified libraries. Even if you have no external libraries to link, you must have this file on your application's root directory.

## Smd files

If you want to know more about the `smd` file format you can check the repository at [`mrsinho/smd`](https://github.com/mrsinho/smd).

 * `ini.smd` must be on the same directory of your executable file and it cannot be empty, it allows the engine to reach the application shared library, application.smd, host-memory.smd, scene.smd, serial.smd, vulkan-memory.smd;
 * `application.smd` is required to call the application functions when your project has been built as a `SHARED` library;
 * `host-memory.smd` is useful to automatically allocate some heap memory on your RAM before running the `p_start` function of your application (this file can be empty since it's not necessary);
 * `vulkan-memory.smd` is useful to automatically allocate some vulkan memory before running the `p_start` function of your application (this file can be empty since it's not necessary);
 * `scene.smd` allows you to load components from [`collection/shcomponents/include`](../collection/shcomponents/include/shcomponents/) (this file can be empty since it's not necessary);
 * `serial.smd` is useful to load a serial port properties before actually opening a serial port, which is a developer's responsibility by implementing (for example) the cross-platform [`shserial`](https://github.com/mrsinho/shserial) library.

You can create your own smd filse and setup your own implementation for the file format. The [smd](https://github.com/mrsinho/smd) library is already linked to the application.

## Assets

Since this application renders a triangle on a window using a standard pipeline (for some reason the example is named `triangle`), shaders and assets have to be stored somewhere. You're free to put them anywhere you want, what matters is to load them correctly from your application scripts (see [`Native scripting`](./native-scripting.md)).

---

## Template application

The `triangle` application is a very good example which could be copied and pasted in order to setup a new project. You can also check the template-documented source code [here](./native-scripting.md).

![triangle](media/triangle.png)

Another good and documented example is the [`noise`](./noise-example.md) application.

---

## Switch between applications in runtime

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



---

## Reloading application

If one of your application's functions returned with an error (non `1` value) or if you missed some information on your `.smd` files, the engine will move to a safe mode. To free al resources and reload the engine press `LEFT_CTRL` + `R` to restart the program. You can restart the application at any time by pressing the same key sequence.