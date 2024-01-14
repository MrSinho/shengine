function BUILD {
	python3.9 export-application.py name=triangle path=applications/triangle
    python3.9 export-application.py name=noise path=applications/noise

    cd    applications/triangle
    mkdir build
    cd    build
    mkdir linux
    cd    linux 
    cmake ../../ -DTRIANGLE_BINARY_TYPE=EXECUTABLE -DSH_ENGINE_ROOT_DIR="D:/home/desktop/github/shengine"
    cmake --build .

    cd ../../../noise
    mkdir build
    cd    build
    mkdir linux
    cd    linux 
    cmake ../../ -DNOISE_BINARY_TYPE=EXECUTABLE -DSH_ENGINE_ROOT_DIR="D:/home/desktop/github/shengine"
    cmake --build .
}

BUILD > .shci/linux/build-output.txt
