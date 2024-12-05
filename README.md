KETI Indoor Positioning Estimation(IPE) 시스템 
===============================================
시스템 개요 
------------
본 리파지토리는 UWB와 Monocular Camera 센서 융합 기반 실내 위치 추정 시스템(이하 IPE 시스템)에 해당한다. IPE는 ROS  
![img](/image/github_ipe.jpg)

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

# launch DeepL vision based in/out determine system 
cd ../inout_deepl/scripts 
rosrun inout_deepl determine_inout.py 
```

Financial Support
------------------ 
This work was supported by Institute of Information & communications Technology Planning & Evaluation (IITP) grant funded by the Korea government (MSIT) (No. 2021-0-01140, Development of ultra high-speed V2X communication-based autonomous driving service technologies).

