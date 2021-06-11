cd Externals/glfw
mkdir build 
cd build
cmake -D CMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug

cd ../../../
cd build
cmake -D CMAKE_BUILD_TYPE=Debug ..

pause

cmake --build . --config Debug 

pause