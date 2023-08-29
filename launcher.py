import tkinter as tk
import ttkbootstrap as ttk

import sys
import os

from tkinter                import *
from zipfile                import *
from threading              import Thread
from ttkbootstrap.constants import *



#sadly this is the closest thing to a C struct
class shInfo:
    
    root:ttk.Window                       = ""
    application_name:ttk.StringVar        = ""
    application_target_type:ttk.StringVar = ""
    application_path:ttk.StringVar        = ""
    generator:ttk.StringVar               = ""
    local_source_files:ttk.StringVar      = ""

    clean_cache:ttk.BooleanVar

    new_project:ttk.BooleanVar

    output_text:ttk.ScrolledText
    progress:ttk.Floodgauge



def shlauncher_read_file(path:str) -> str:
    file = open(path, "r")
    code = file.read()
    file.close()
    return code

def shlauncher_ui_command(info:shInfo, _cmd:str, catch_code:bool) -> int:
    if (catch_code == True):
        _cmd += " > .launcher"

    msg:str = f"shlauncher command: {_cmd}\n"
    info.output_text.insert(INSERT, msg)
    print(msg)

    os.system(_cmd)

    output:str = shlauncher_read_file(f".launcher")

    info.output_text.insert(INSERT, output)

    return 1

def shlauncher_msg(msg:str) -> None:
    print(f"shinstaller message: {msg}\n")
    return

def shlauncher_ui_msg(info:shInfo, msg:str) -> None:
    _msg:str = f"shinstaller message: {msg}\n"
    
    info.output_text.insert(INSERT, _msg)
    print(_msg)
    return

def getBool(literal:str) -> bool:
    if (literal == "True" or literal == "true"):
        return True
    else:
        return False
    
    return

def get_info(info:shInfo) -> None:
    info.application_name        = StringVar(master=info.root,  value="newproject")
    info.application_target_type = StringVar(master=info.root,  value="EXECUTABLE")
    info.application_path        = StringVar(master=info.root,  value="applications/newproject")
    info.generator               = StringVar(master=info.root,  value="")
    info.clean_cache             = BooleanVar(master=info.root, value=False)
    info.local_source_files      = StringVar(master=info.root,  value="src/newproject.c")
    info.new_project             = BooleanVar(master=info.root, value=False)

    shlauncher_msg(f"""
    shInfo:
        application_name           : {info.application_name.get()}
        application_target_type    : {str(info.application_target_type.get())}
        application_path           : {str(info.application_path.get())}
        generator                  : {str(info.generator.get())}
""")

def shlauncher_generate_files(info:shInfo) -> None:
    shlauncher_ui_msg(info, "generating files...")

    app_path:str = os.path.abspath(info.application_path.get())

    #Write files for new projects
    if (info.new_project.get() == True):
        shlauncher_ui_msg(info, "writing files for new project")

        shlauncher_ui_command(info, "mkdir " + os.path.abspath("applications/" + info.application_name.get()), False)

        shlauncher_ui_command(info, f"mkdir " + os.path.abspath(app_path + "/smd"), False)
        shlauncher_ui_command(info, f"mkdir " + os.path.abspath(app_path + "/shaders"), False)
        shlauncher_ui_command(info, f"mkdir " + os.path.abspath(app_path + "/src"), False)
    
        shlauncher_ui_command(info, f"mkdir " + os.path.abspath(app_path + "/shaders/bin"), False)
        shlauncher_ui_command(info, f"mkdir " + os.path.abspath(app_path + "/shaders/src"), False)
    
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/source-files.txt"), False)
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/libs.txt"), False)
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/ini.smd"), False)
    
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/source-files.txt"), False)
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/smd/application.smd"), False)
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/smd/host-memory.smd"), False)
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/smd/vulkan-memory.smd"), False)
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/smd/scene.smd"), False)
        shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(app_path + "/smd/serial.smd"), False)

        #Write ini
        shlauncher_ui_msg(info, "writing ini file")
        ini_src:str = f"""STR1024    ^^ 1    !! ShEngine::ini_properties.application_name        --> {info.application_name.get()}    ***
STR1024    ^^ 1    !! ShEngine::ini_properties.application_smd_path    --> ../../smd/application.smd    ***
STR1024    ^^ 1    !! ShEngine::ini_properties.host_memory_smd_path    -->                              ***
STR1024    ^^ 1    !! ShEngine::ini_properties.vulkan_memory_smd_path  -->                              ***
STR1024    ^^ 1    !! ShEngine::ini_properties.serial_smd_path         -->                              ***
STR1024    ^^ 1    !! ShEngine::ini_properties.scene_smd_path          -->                              ***

copy_to_windows/bin_or_linux/bin
"""
    
        os_folder:str = "windows"
        if (sys.platform != "win32"):
            os_folder = "linux"

        ini_path:str     = os.path.abspath(f"{app_path}/ini.smd")
        ini_bin_path:str = os.path.abspath(f"{app_path}/{os_folder}/bin/ini.smd")

        shlauncher_ui_command(info, "mkdir " + os.path.abspath(f"{app_path}/{os_folder}"), True)
        shlauncher_ui_command(info, "mkdir " + os.path.abspath(f"{app_path}/{os_folder}/bin"), True)

        file = open(ini_path, "w")
        file.write(ini_src)
        file.close()

        file = open(ini_bin_path, "w")
        file.write(ini_src)
        file.close()

        #Write source-files.txt
        shlauncher_ui_command(info, f"echo. " + info.local_source_files.get() + f" > " + os.path.abspath(info.application_path.get() + "/source-files.txt"), False)
        for local_source_path in info.local_source_files.get().split():
            shlauncher_ui_command(info, f"echo. 2>> " + os.path.abspath(f"{app_path}/{local_source_path}"), False)

        #write application.smd
        shlauncher_ui_msg(info, "writing application.smd")

        application_smd_path:str = os.path.abspath(f"{app_path}/smd/application.smd")
        application_src:str = f"""STR256    ^^ 1    !! ShEngine::application_properties.shared_name             --> {info.application_name.get()}                 ***
STR256    ^^ 1    !! ShEngine::application_properties.s_start                 --> {info.application_name.get()}_start           ***
STR256    ^^ 1    !! ShEngine::application_properties.s_update                --> {info.application_name.get()}_update          ***
STR256    ^^ 1    !! ShEngine::application_properties.s_main_cmd_buffer       --> {info.application_name.get()}_main_cmd_buffer ***
STR256    ^^ 1    !! ShEngine::application_properties.s_main_renderpass       --> {info.application_name.get()}_main_renderpass ***
STR256    ^^ 1    !! ShEngine::application_properties.s_frame_resize          --> {info.application_name.get()}_frame_resize    ***
STR256    ^^ 1    !! ShEngine::application_properties.s_close                 --> {info.application_name.get()}_close           ***
"""
        file = open(application_smd_path, "w")
        file.write(application_src)
        file.close()

        #write main source file
        shlauncher_ui_msg(info, "writing main source files")
        shlauncher_ui_command(info, f"echo. " + info.local_source_files.get() + f" > " + os.path.abspath(app_path + "/source-files.txt"), False)

        main_src:str = f"""#ifdef __cplusplus
extern "C" {{
#endif//__cplusplus

#include <shengine/shEngine.h>//most of engine declarations are here
#include <shengine/shExport.h>//required when building a shared library



uint8_t SH_ENGINE_EXPORT_FUNCTION {info.application_name.get()}_start(ShEngine* p_engine) {{
    // First function to be called chronologically.
    return 1;
}}

uint8_t SH_ENGINE_EXPORT_FUNCTION {info.application_name.get()}_update(ShEngine* p_engine) {{
    // This function is called once per frame.
	return 1;
}}

uint8_t SH_ENGINE_EXPORT_FUNCTION {info.application_name.get()}_main_cmd_buffer(ShEngine* p_engine) {{
    // This function is called once per frame while the main engine command buffer is recording
	return 1;
}}

uint8_t SH_ENGINE_EXPORT_FUNCTION {info.application_name.get()}_main_renderpass(ShEngine* p_engine) {{
    // This function is called once per frame inside the main engine renderpass (you can setup your own of course using direcly the Vulkan API or the shvulkan library, by running on the {info.application_name.get()}_update function).
    return 1;
}}


uint8_t SH_ENGINE_EXPORT_FUNCTION {info.application_name.get()}_frame_resize(ShEngine* p_engine) {{
    // When the window is resized, this function is called. It's useful to destroy pipelines and reload them with a new viewport.
    return 1;
}}

uint8_t SH_ENGINE_EXPORT_FUNCTION {info.application_name.get()}_close(ShEngine* p_engine) {{
    // When the engine is released/restarted, this function is called.
    return 1;
}}



#ifdef SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#include <sheditor/shEditor.h>

int main() {{
    ShEngine* p_engine = shAllocateEngine();
    shEngineError(
        p_engine == NULL,
        "main: invalid engine memory",
        return -1
    );

    p_engine->application_host.p_start           = (ShApplicationFunc*) &{info.application_name.get()}_start;
    p_engine->application_host.p_update          = (ShApplicationFunc*) &{info.application_name.get()}_update;
    p_engine->application_host.p_main_cmd_buffer = (ShApplicationFunc*) &{info.application_name.get()}_main_cmd_buffer;
    p_engine->application_host.p_main_renderpass = (ShApplicationFunc*) &{info.application_name.get()}_main_renderpass;
    p_engine->application_host.p_frame_resize    = (ShApplicationFunc*) &{info.application_name.get()}_frame_resize;
    p_engine->application_host.p_close           = (ShApplicationFunc*) &{info.application_name.get()}_close;
    p_engine->window.title                       = "{info.application_name.get()}";
    
    shEditorMain(p_engine);
    return 0;
}}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}}
#endif//__cplusplus
"""
        main_src_path:str = os.path.abspath(f"{app_path}/{info.local_source_files.get().split()[0]}")
        file = open(main_src_path, "w")
        file.write(main_src)
        file.close()


    return

def setup_project(info:shInfo) -> None:
    info.progress["bootstyle"] = "success"#for second attempts

    info.progress["value"] = 10

    cmd:str = "python"

    if (sys.platform != "win32"):
        cmd += "3.9"

    cmd += f" export-application.py name={info.application_name.get()} target={info.application_target_type.get()} path={info.application_path.get()}"

    if (info.generator.get() != ""):
        cmd += f" generator={info.generator.get()}"

    if (info.clean_cache.get() == True):
        cmd += f" CLEAN-CACHE"

    info.progress["value"] = 50

    #generate files
    shlauncher_generate_files(info)

    info.progress["value"] = 75

    #call command
    shlauncher_ui_command(info, cmd, True)

    info.progress["value"] = 100

    shlauncher_ui_msg(info, "DONE")

    return


def set_target_type(info:shInfo, m:ttk.Menubutton) -> None:
    m.config(text=info.application_target_type.get())


def main() -> int:#example call: python shlauncher.py
    
    info:shInfo = shInfo()

    #ROOT
    #
    #
    info.root = ttk.Window(themename="cyborg")
    info.root.geometry("500x800")
    info.root.title("shengine launcher")

    info.root.rowconfigure(0, weight=1)
    info.root.rowconfigure(1, weight=4)
    info.root.columnconfigure(0, weight=1)

    #_image = Image.open("media/noise.png")
    #icon   = ImageTk.PhotoImage(image=_image)
    #root.wm_iconphoto(False, icon)

    get_info(info)

    #SETUP
    #
    #
    f0 = ttk.LabelFrame(master=info.root, text="setup", bootstyle="primary")
    f0.rowconfigure(0, weight=1)
    f0.rowconfigure(1, weight=1)
    f0.rowconfigure(2, weight=1)
    f0.rowconfigure(3, weight=1)
    f0.rowconfigure(4, weight=1)
    f0.rowconfigure(5, weight=1)
    f0.columnconfigure(0, weight=1)
    f0.columnconfigure(1, weight=2)

    f0.grid(row=0, column=0, sticky="nsew", padx=5, pady=5)

    #APPLICATION NAME
    #
    t = ttk.Label(master=f0, text="application name", bootstyle="light")
    t.grid(row=0, column=0, padx=5, pady=5, sticky="w")

    t = ttk.Entry(master=f0, textvariable=info.application_name, bootstyle="light") 
    t.grid(row=0, column=1, padx=5, pady=5, sticky="ew")

    #APPLICATION PATH
    #
    t = ttk.Label(master=f0, text="application path", bootstyle="light")
    t.grid(row=1, column=0, padx=5, pady=5, sticky="w")

    t = ttk.Entry(master=f0, textvariable=info.application_path, bootstyle="light")
    t.grid(row=1, column=1, padx=5, pady=5, sticky="ew")

    #TARGET TYPE
    #
    m = ttk.Label(master=f0, text="target type", bootstyle="light")
    m.grid(row=2, column=0, padx=5, pady=5, sticky="w")

    m = ttk.Menubutton(master=f0, text=info.application_target_type.get(), bootstyle="light-outline")

    _t = ttk.Menu(t)
    _t.add_radiobutton(label="EXECUTABLE", value="EXECUTABLE", variable=info.application_target_type, command=lambda:set_target_type(info, m))
    _t.add_radiobutton(label="SHARED",     value="SHARED",     variable=info.application_target_type, command=lambda:set_target_type(info, m))

    m["menu"] = _t
    m.grid(row=2, column=1, padx=5, pady=5, sticky="nsew")

    #GENERATOR
    #
    t = ttk.Label(master=f0, text="generator", bootstyle="light")
    t.grid(row=3, column=0, padx=5, pady=5, sticky="w")

    t = ttk.Entry(master=f0, textvariable=info.generator, bootstyle="light")
    t.grid(row=3, column=1, padx=5, pady=5, sticky="ew")

    #CLEAN CACHE
    #
    t = ttk.Label(master=f0, text="clean cache", bootstyle="light")
    t.grid(row=4, column=0, padx=5, pady=5, sticky="w")

    t = ttk.Checkbutton(master=f0, variable=info.clean_cache, onvalue=True, offvalue=False, bootstyle="round-toggle-light")
    t.grid(row=4, column=1, padx=5, pady=5, sticky="e")

    #NEW PROJECT
    #
    t = ttk.Label(master=f0, text="new project", bootstyle="light")
    t.grid(row=5, column=0, padx=5, pady=5, sticky="w")

    t = ttk.Checkbutton(master=f0, variable=info.new_project, onvalue=True, offvalue=False, bootstyle="round-toggle-light")
    t.grid(row=5, column=1, padx=5, pady=5, sticky="e")

    #SOURCE FILES
    #
    t = ttk.Label(master=f0, text="source files (ignored if not new project)", bootstyle="light")
    t.grid(row=6, column=0, padx=5, pady=5, sticky="w")

    t = ttk.Entry(master=f0, textvariable=info.local_source_files, bootstyle="light")
    t.grid(row=6, column=1, padx=5, pady=5, sticky="ew")



    #SETUP PROJECT
    #just downloads plain files
    t = ttk.Button(master=f0, text="setup project", bootstyle="outline-toolbutton-primary", command=lambda:Thread(target=setup_project, args=[info]).start())
    t.grid(row=7,  column=1, padx=5, pady=5, sticky="nsew")



    #OUTPUT AND STATUS
    #
    #
    f1 = ttk.LabelFrame(master=info.root, text="output", bootstyle="success")
    f1.rowconfigure(0, weight=1)
    f1.rowconfigure(1, weight=1)
    f1.columnconfigure(0, weight=1)

    f1.grid(row=1, column=0, sticky="nsew", padx=5, pady=5)
    
    #OUTPUT SCROLLED TEXT
    #
    info.output_text = ttk.ScrolledText(master=f1)
    info.output_text.grid(row=0, column=0, padx=5, pady=5, sticky="nsew", )

    #PROGRESS BAR
    #
    info.progress = ttk.Floodgauge(master=f1, text="progress...", bootstyle="success")
    info.progress.grid(row=1, column=0, padx=5, pady=5, sticky="sew")

    info.root.mainloop()

    return 0


if (__name__ == "__main__"):
    main()