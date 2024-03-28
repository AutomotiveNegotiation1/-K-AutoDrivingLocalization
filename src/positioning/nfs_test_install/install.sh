#!/bin/bash
SHELL_PATH=`pwd -P`
echo $SHELL_PATH
sudo apt update
sudo apt install -y nfs-kernel-server
sudo cat /proc/fs/nfsd/versions
sudo mkdir -p /mnt/nfs_client
sudo touch /mnt/nfs_client/logfile.log
sudo chown ${USER}:${USER} /mnt/nfs_client/logfile.log
echo "/mnt/nfs_client   192.168.2.127/24(rw,wdelay, root_squash, no__subtree_check, sec=sys, secure, no_all_squash)" | sudo tee -a /etc/exports
sudo exportfs -ra
sudo exportfs -v