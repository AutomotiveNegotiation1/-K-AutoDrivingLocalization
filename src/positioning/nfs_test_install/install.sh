#!/bin/bash
SHELL_PATH=`pwd -P`
echo $SHELL_PATH
sudo apt update
sudo apt install -y nfs-kernel-server
sudo cat /proc/fs/nfsd/versions
sudo mkdir -p /mnt/nfs_share
sudo chown -R nobody:nogroup /mnt/nfs_share
sudo touch /mnt/nfs_share/logfile.log
sudo chown ${USER}:${USER} /mnt/nfs_share/logfile.log
echo "/mnt/nfs_client 192.168.2.127/24(rw,sync,no_subtree_check,no_root_squash)" | sudo tee -a /etc/exports
sudo exportfs -ra
sudo exportfs -v
