PREREQUISITES_OUTPUT = ".shci/linux/prerequisites-output.txt"
sudo apt -y update > $PREREQUISITES_OUTPUT
sudo apt install -y python3.9 python3-pip $PREREQUISITES_OUTPUT
sudo apt install -y gcc cmake make libc6-dev $PREREQUISITES_OUTPUT
cd shengine/requirements $PREREQUISITES_OUTPUT
sudo bash debian-requirements.sh $PREREQUISITES_OUTPUT