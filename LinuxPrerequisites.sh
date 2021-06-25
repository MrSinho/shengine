sudo apt install -y doxygen
sudo apt install -y libx11-dev
sudo apt install -y libxrandr-dev
sudo apt install -y libxinerama-dev
sudo apt install -y libxcursor-dev
sudo apt install -y libxi-dev
sudo apt install -y libvulkan-dev

sudo add-apt-repository ppa:oibaf/graphics-drivers
sudo apt update
sudo apt upgrade
sudo apt install libvulkan1 mesa-vulkan-drivers vulkan-utils