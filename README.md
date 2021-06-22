# Foggy-Engine (WORK IN PROGRESS) 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

A c++ engine based on Vulkan written for developing quick cross-platform 3d applications and videogames. I intentioally decided to use no gui in the editor for a faster developing session.

# What it uses

 * Vulkan SDK by LunarG
 * GLFW for cross platform windows, handling input events and time.
 * EZ-ECS by MrSinho.
 * ObjImporter by MrSinho.
 * stb image for loading image data
 * json by Nlohmann for loading scenes

# Building the source code 
```bash
git clone --recursive https://github.com/MrSinho/Foggy-Engine.git
cd Foggy-Engine 
mkdir build && cd build
cmake ..
```

# Quick Tutorial

Open the Editor executable and press `CTRL-N` for creating a new scene. The commands creates a raw `scene.json` at `appInfo/scenes/scene.json`. Then open `appInfo/settings/settings.json` and set the `default scene path` as the path relative to the executable path: `"../../appInfo/scenes/scene.json"`. 