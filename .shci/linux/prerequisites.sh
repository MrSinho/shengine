prerequisites() 
{
    sudo apt -y update
    sudo apt install -y python3.9 python3-pip
    sudo apt install -y gcc cmake make libc6-dev
    cd shengine/requirements
    sudo bash debian-requirements.sh
}

prerequisites