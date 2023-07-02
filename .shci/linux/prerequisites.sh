sudo apt -y update | tee .shci/linux/prerequisites-output.txt
sudo apt install -y python3.9 python3-pip | tee -a .shci/linux/prerequisites-output.txt
sudo apt install -y gcc cmake make libc6-dev | tee -a .shci/linux/prerequisites-output.txt
cd shengine/requirements | tee -a .shci/linux/prerequisites-output.txt
sudo bash debian-requirements.sh | tee -a .shci/linux/prerequisites-output.txt