KETI Indoor Positioning Estimation(IPE) 시스템 
===============================================

실내측위시스템 설치 및 빌드
------------------------- 
```bash
git clone https://github.com/AutomotiveNegotiation1/-K-AutoDrivingLocalization.git
cd src/positioning/catkin_ws 
catkin build 
```

# 실행 방법 
```bash 
cd ./-K-AutoDrivingLocalization/src/positioning/catkin_ws/src
./0_ListCOMPorts
./1_ChkttyDevices
./3_MakeAuthUWB

# launch uwb
cd ./-K-AutoDrivingLocalization/src/positioning/catkin_ws/src/dwm1001_ros/launch 
roslaunch dwm1001.launch 

# launch orb-slam3 
cd ../../orb_slam3_ros/launch 
roslaunch realtime_zed2_1K.launch 

# launch sensor fusion system
cd ../../ipe_v4_2/launch 
roslaunch ipe.launch 

# launch viewer 
cd ../../view/launch 
roslaunch view.launch 
```



