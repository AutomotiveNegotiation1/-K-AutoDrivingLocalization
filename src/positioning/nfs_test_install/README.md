# nfs_test_install

```
unzip 20230905_test.zip
cd 20230905_test
./install.sh
# /home/${USER}에 rosbag 및 nfs_client 폴덜 생성되며
# rosbag 폴더에는 test.bag 파일 이동 확인

./KETIApp 
or
python3 KETIApp.py

# sh: 1: source: not found
# ... logging to /home/umaps/.ros/log/a1d1fc08-4b8d-11ee-9fd0-ff03f95684ee/roslaunch-umaps-33124.log
# Checking log directory for disk usage. This may take a while.
# Press Ctrl-C to interrupt
# Done checking log file disk usage. Usage is <1GB.
# 
# started roslaunch server http://umaps:36675/
# ros_comm version 1.15.14
# 
# 
# SUMMARY
# ========
# 
# PARAMETERS
#  * /rosdistro: noetic
#  * /rosversion: 1.15.14
# 
# NODES
# 
# auto-starting new master
# process[master]: started with pid [33133]
# ROS_MASTER_URI=http://umaps:11311/
# 
# setting /run_id to a1d1fc08-4b8d-11ee-9fd0-ff03f95684ee
# process[rosout-1]: started with pid [33144]
# started core service [/rosout]
# 1: test.bag
# 2: back
# Select an option:
# 
# # Select an option: 1 이라 작성 후 Enter
# 1: test.bag
# 2: back
# Select an option: 1
# [ INFO] [1693878297.938820867]: Opening /home/umaps/rosbag/test.bag
# 
# Waiting 0.2 seconds after advertising topics... done.
# 
# Hit space to toggle paused, or 's' to step.
#  [DELAYED]  Bag Time: 1693380966.833686   Duration: 0.000000 / 245.373090   
# [RUNNING]  Bag Time: 1693380966.833686   Duration: 0.000000 / 245.373090        
# [RUNNING]  Bag Time: 1693380966.833686   Duration: 0.000000 / 245.373090        
# [RUNNING]  Bag Time: 1693380966.834150   Duration: 0.000463 / 245.373090        
# [RUNNING]  Bag Time: 1693380966.834256   Duration: 0.000570 / 245.373090        
# [RUNNING]  Bag Time: 1693380966.834364   Duration: 0.000677 / 245.373090        
# [RUNNING]  Bag Time: 1693380966.834471   Duration: 0.000784 / 245.373090        
# [RUNNING]  Bag Time: 1693380966.835094   Duration: 0.001407 / 245.373090        
# [RUNNING]  Bag Time: 1693380966.835190   Duration: 0.001504 / 245.373090
# ...
# 
# # /home/${USER}/nfs_client/logfile.log 생성 및 데이터 저
# logfile.log 
# 1,POS,0.00,0.00,0.00,HEAD,0.00
# 1,POS,0.00,0.00,0.00,HEAD,0.00
# 1,POS,0.00,0.00,0.00,HEAD,0.00
# 1,POS,0.00,0.00,0.00,HEAD,0.00
# 1,POS,0.00,0.00,0.00,HEAD,0.00
# 1,POS,0.00,0.00,0.00,HEAD,0.00
# 1,POS,0.00,0.00,0.00,HEAD,0.00
# 1,POS,0.00,0.00,0.00,HEAD,0.00
# 1,POS,0.00,0.00,0.00,HEAD,0.00
```