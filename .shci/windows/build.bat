> .shci/windows/build-output.txt (

    python export-application.py name=triangle path=applications/triangle
    python export-application.py name=noise path=applications/noise

    cd    applications/triangle
    mkdir build
    cd    build
    mkdir windows
    cd    windows 
    cmake ../../ -DTRIANGLE_BINARY_TYPE=EXECUTABLE -DSH_ENGINE_ROOT_DIR="D:/home/desktop/github/shengine"
    cmake --build .

    cd ../../../noise
    mkdir build
    cd    build
    mkdir windows
    cd    windows 
    cmake ../../ -DNOISE_BINARY_TYPE=EXECUTABLE -DSH_ENGINE_ROOT_DIR="D:/home/desktop/github/shengine"
    cmake --build .
)
