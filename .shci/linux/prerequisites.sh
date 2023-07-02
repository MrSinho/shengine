prerequisites_output=".shci/linux/prerequisites-output.txt"
sudo apt -y update > $prerequisites_output
sudo apt install -y python3.9 python3-pip >> $prerequisites_output
sudo apt install -y gcc cmake make libc6-dev >> $prerequisites_output
cd shengine/requirements >> $prerequisites_output
sudo bash debian-requirements.sh >> $prerequisites_output