close all
clear
clc

%% To use the custom messages, follow these steps:

% ROS message update command: rosgenmsg("D:\matlab\ros\ros_to_pc\custom_msgs")

% 1. Add the custom message folder to the MATLAB path by executing:
% addpath('D:\matlab\ros\ros_to_pc\custom_msgs\matlab_msg_gen_ros1\win64\install\m')
% savepath

% 2. Refresh all message class definitions, which requires clearing the workspace, by executing:
% clear classes
% rehash toolboxcache

% 3. Verify that you can use the custom messages. 
%    Enter "rosmsg list" and ensure that the output contains the generated
%    custom message types.

%%
rosshutdown
setenv('ROS_MASTER_URI','http://192.168.3.201:11311') % Jetson IP
setenv('ROS_IP','192.168.171.195') % window IP
setenv('ROS_HOSTNAME','192.168.171.195') % window IP

rosinit

subImu=rossubscriber('/ublox/imu',"DataFormat",'struct'); % IMU
% subGps=rossubscriber('/ublox/gnss_pvt','ublox_f9r/gnssPVT','DataFormat','struct'); % GPS
subUwb=rossubscriber('/uwb/ranging', 'decawave_uwb/uwb_anchor','DataFormat','struct');


accel = zeros(1,3);
gyro  = zeros(1,3);

k = 1;

Fs = 50;


% figure(1); hold on; 
figure; hold on;
hAxes(1) = gca;

h1 = animatedline(hAxes(1), 'Color','r');
h2 = animatedline(hAxes(1), 'Color','g');
h3 = animatedline(hAxes(1), 'Color','b');

legend('x (rad)', 'y (rad)', 'z (rad)');
ylabel('Euler Angles (rad)'); xlabel('Samples');
title('Orientation','fontsize', 12);
grid minor;
hold off;

figure; hAxes(2) = gca;
h4 = animatedline(hAxes(2), 'Color','r');
h5 = animatedline(hAxes(2), 'Color','g');
h6 = animatedline(hAxes(2), 'Color','b');

legend('A_x (m/s^2)','A_y (m/s^2)','A_z (m/s^2)');
ylabel('Acceleration (m/s^2)'); xlabel('Samples');
title('Accelerometer','fontsize', 12);
grid minor;
hold off;

figure; hAxes(3) = gca;
h7 = animatedline(hAxes(3), 'Color','r');
h8 = animatedline(hAxes(3), 'Color','g');
h9 = animatedline(hAxes(3), 'Color','b');

legend('x (rad)', 'y (rad)', 'z (rad)');
ylabel('Euler Angles (rad)'); xlabel('Samples');
title('Fused orientation','fontsize', 12);
grid minor;
hold off;

figure; hAxes(4) = gca;
h10 = animatedline(hAxes(4), 'Color','r');
h11 = animatedline(hAxes(4), 'Color','g');
h12 = animatedline(hAxes(4), 'Color','b');

legend('A_x (m/s^2)','A_y (m/s^2)','A_z (m/s^2)');
ylabel('Acceleration (m/s^2)'); xlabel('Samples');
title('Fused Accelerometer','fontsize', 12);
grid minor;
hold off;

figure; hAxes(5) = gca;
h13 = animatedline(hAxes(5), 'Color','r');
h14 = animatedline(hAxes(5), 'Color','g');
h15 = animatedline(hAxes(5), 'Color','b');

% legend('A_x (m/s^2)','A_y (m/s^2)','A_z (m/s^2)');
% ylabel('Acceleration (m/s^2)'); xlabel('Samples');
title('Fused Accelerometer','fontsize', 12);
grid minor;
hold off;

m = 1;
l = 1;

while(1)
    [msgUwb,statusUwb,statustextUwb] = receive(subUwb,1)
    m = m+1

    [msgImu,statusImu,statustextImu] = receive(subImu,1)
%     [msgGps,statusGps,statustextGps] = receive(subGps,1);
    
    l = l+1
%     receiv = receive(sub,1);
    
    accel(k,:) = [msgImu.LinearAcceleration.X msgImu.LinearAcceleration.Y msgImu.LinearAcceleration.Z];
    gyro(k,:)  = [msgImu.AngularVelocity.X msgImu.AngularVelocity.Y msgImu.AngularVelocity.Z];

    fuse = complementaryFilter('SampleRate',Fs,'HasMagnetometer', false, 'ReferenceFrame', 'ENU');
    [orientation, angularVelocity] = fuse(accel(k,:), gyro(k,:));
    euler_q = eulerd(orientation, 'ZYX', 'frame');

%     plot(k, euler_q(1),'r'); plot(k, euler_q(2),'g'); plot(k, euler_q(3),'b');
 
    addpoints(h1,k,gyro(k,1));
    addpoints(h2,k,gyro(k,2));
    addpoints(h3,k,gyro(k,3));

    addpoints(h4,k,accel(k,1));
    addpoints(h5,k,accel(k,2));
    addpoints(h6,k,accel(k,3));

    addpoints(h7,k,euler_q(1));
    addpoints(h8,k,euler_q(2));
    addpoints(h9,k,euler_q(3));

    addpoints(h10,k,angularVelocity(1));
    addpoints(h11,k,angularVelocity(2));
    addpoints(h12,k,angularVelocity(3));

    drawnow

    k = k+1;
end

% figure(2); hold on;
% title('Orientation Estimate');
% 
% ylabel('Degrees');
% q2 = fuse(accel, gyro);
% plot(eulerd(q2, 'XYZ', 'frame'));
% legend('X-rotation', 'Y-rotation', 'Z-rotation');