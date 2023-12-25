> .shci/windows/build-output.txt (
    python export-application.py name=triangle target=EXECUTABLE path=applications/triangle
    python export-application.py name=noise target=EXECUTABLE path=applications/noise
    cd applications/triangle/build/windows
    cmake --build .
    cd ../../../noise/build/windows
    cmake --build .
)
