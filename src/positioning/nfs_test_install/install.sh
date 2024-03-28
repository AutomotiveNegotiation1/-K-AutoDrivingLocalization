#!/bin/bash
SHELL_PATH=`pwd -P`
echo $SHELL_PATH
sudo apt update
sudo apt install -y nfs-kernel-server
sudo cat /proc/fs/nfsd/versions
mkdir -p /home/${USER}/nfs_client
mkdir -p /home/${USER}/rosbag
cp $SHELL_PATH/rosbag/test.bag /home/${USER}/rosbag/test.bag
IP_ADDR=$(ifconfig eth0 | grep 'inet ' | awk '{print $2}')
echo "/home/${USER}/nfs_client   ${IP_ADDR}/24(rw,sync,no_subtree_check)" | sudo tee -a /etc/exports
sudo exportfs -ra
sudo exportfs -v