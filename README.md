KETI Indoor Positioning Estimation(IPE) 시스템 
===============================================
시스템 개요 
------------
본 리파지토리는 UWB와 Monocular Camera 센서 융합 기반 실내 위치 추정 시스템(이하 IPE 시스템)을 제공한다. IPE는 Jetpack 5.0 및 ROS Noetic 환경에서 동작하며 catkin 빌드시스템을 사용한다. IPE는 센서 구동, 센서 융합, 측위 정보 표출 기능을 포함하며 각 기능은 ROS package로 모듈화 되었다. 아래 실행 방법 항목은 각 센서 구동(UWB 및 Zed2 카메라)및 센서 융합 알고리즘 실행 및 측위 정보 표출용 뷰어 실행을 위한 패키지 launch 명령어에 해당한다.       
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

