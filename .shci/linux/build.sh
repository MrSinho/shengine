function BUILD {
	python3.9 export-application.py name=triangle target=EXECUTABLE path=applications/triangle
	python3.9 export-application.py name=noise target=EXECUTABLE path=applications/noise
	cd applications/triangle/build/linux
	cmake --build .
	cd ../../../noise/build/linux
	cmake --build .
}

BUILD > .shci/linux/build-output.txt
