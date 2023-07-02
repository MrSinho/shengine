sudo apt -y update | tee .shci/linux/prerequisites-output.txt
sudo apt install -y python3.9 python3-pip | tee .shci/linux/prerequisites-output.txt
sudo apt install -y gcc cmake make libc6-dev | tee .shci/linux/prerequisites-output.txt
cd shengine/requirements | tee .shci/linux/prerequisites-output.txt
sudo bash debian-requirements.sh | tee .shci/linux/prerequisites-output.txt