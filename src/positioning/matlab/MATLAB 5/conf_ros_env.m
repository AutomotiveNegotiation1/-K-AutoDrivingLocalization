function [subImu, subUwb0, subUwb1, subUwb2, subUwb3] = conf_ros_env

%% Configure custom messages (Run this code only once)
% 0. Install 'Visual studio' for Windows
% 1. Connect Mac and devices in one wireless network
% 2. Check IP of Mac and devices
% 3. Generate messages for Mac
%    >> rosgenmsg("/Users/shyoon11/Documents/GitHub/-K-AutoDrivingLocalization/src/positioning/keti_ws/src")
% 4. 다음을 실행하여 사용자 지정 메시지 폴더를 MATLAB 경로에 추가합니다:
%  
% addpath('/Users/shyoon11/Documents/GitHub/-K-AutoDrivingLocalization/src/positioning/keti_ws/src/matlab_msg_gen_ros1/maci64/install/m')
% savepath
%  
% 5. 모든 메시지 클래스 정의를 새로 고칩니다. 이를 위해서는 다음을 실행해 작업 공간을 삭제해야 합니다:
%  
% clear classes
% rehash toolboxcache
%  
% 6. 사용자 지정 메시지를 사용할 수 있는지 확인합니다. 
%  "rosmsg list"를 입력한 다음, 생성된 사용자 지정 메시지 유형이
%  출력에 포함되어 있는지 확인합니다.
% cf) Message folder: /Users/shyoon11/Documents/GitHub/-K-AutoDrivingLocalization/src/positioning/keti_ws/src

%% To use real UWB, IMU devices, enable thie codes

rosshutdown
setenv('ROS_MASTER_URI','http://192.168.3.201:11311') % Jetson IP
setenv('ROS_IP','192.168.3.200') % Mac IP
setenv('ROS_HOSTNAME','192.168.3.200') % Mac IP

rosinit


subImu=rossubscriber('/zed_f9r/imu','sensor_msgs/Imu', "DataFormat",'struct'); % IMU

subUwb0=rossubscriber('/dwm1001/anchor/ttyUWB0', 'localizer_dwm1001/Anchor','DataFormat','struct'); % UWB0
subUwb1=rossubscriber('/dwm1001/anchor/ttyUWB1', 'localizer_dwm1001/Anchor','DataFormat','struct'); % UWB1
subUwb2=rossubscriber('/dwm1001/anchor/ttyUWB2', 'localizer_dwm1001/Anchor','DataFormat','struct'); % UWB2
subUwb3=rossubscriber('/dwm1001/anchor/ttyUWB3', 'localizer_dwm1001/Anchor','DataFormat','struct'); % UWB3
