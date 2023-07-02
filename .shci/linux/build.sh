> .shci/linux/build-output.txt (
    cd shengine/
    python3.9 export-application.py name=triangle target=EXECUTABLE path=applications/triangle
    python3.9 export-application.py name=noise target=EXECUTABLE path=applications/noise
    cd applications/triangle/linux/build
    cmake --build .
    cd ../../../noise/linux/build
    cmake --build .
)
