import sys
import pathlib

#or manually edit bin/loader.ini
def main():#python set-simulation.py noise <optional-custom-path>

    print("EXAMPLE CALL: python set-simulation.py noise")
    print("EXAMPLE CALL: python set-simulation.py noise ../simulations/noise/assets/")

    python_src_dir:str  = str(pathlib.Path(__file__).parent.resolve().as_posix())
    simulation_name:str = str(sys.argv[1])
    assets_path:str     = ""
    if (len(sys.argv) > 2):
        assets_path = sys.argv[2]

    loader_ini:str = f"{simulation_name}\n"
    if (assets_path == ""):
        loader_ini += f"../simulations/{simulation_name}/assets/"
    else:
        loader_ini += assets_path

    loader_stream = open(f"{python_src_dir}/bin/loader.ini", "w")
    loader_stream.write(loader_ini)
    loader_stream.close()

    print("Edited bin/loader.ini")

    return

if __name__ == '__main__':
    main()