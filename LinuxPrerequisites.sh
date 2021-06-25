sudo apt install -y doxygen
sudo apt install -y libx11-dev
sudo apt install -y libxrandr-dev
sudo apt install -y libxinerama-dev
sudo apt install -y libxcursor-dev
sudo apt install -y libxi-dev
sudo apt install -y libvulkan-dev

wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-bionic.list http://packages.lunarg.com/vulkan/lunarg-vulkan-bionic.list
sudo apt update
sudo apt install -y vulkan-sdk
