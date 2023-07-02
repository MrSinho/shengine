sudo apt -y update > .shci/linux/prerequisites-output.txt 2>&1
sudo apt install -y python3.9 python3-pip >> .shci/linux/prerequisites-output.txt 2>&1
sudo apt install -y gcc cmake make libc6-dev >> .shci/linux/prerequisites-output.txt 2>&1
cd shengine/requirements >> .shci/linux/prerequisites-output.txt 2>&1
sudo bash debian-requirements.sh >> .shci/linux/prerequisites-output.txt 2>&1