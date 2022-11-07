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

% addpath '/Users/shyoon11/matlab/ros/ros_Yoon/exec/libs'

%%
rosshutdown
% setenv('ROS_MASTER_URI','http://192.168.143.219:11311') % Jetson IP
setenv('ROS_MASTER_URI','http://172.20.10.6:11311') % IBM notebook
setenv('ROS_IP','172.20.10.7') % window IP
setenv('ROS_HOSTNAME','172.20.10.7') % window IP

rosinit

subImu=rossubscriber('/ublox/imu',"DataFormat",'struct'); % IMU
% subGps=rossubscriber('/ublox/gnss_pvt','ublox_f9r/gnssPVT','DataFormat','struct'); % GPS
subUwb=rossubscriber('/uwb/ranging', 'decawave_uwb/uwb_anchor','DataFormat','struct');


accel = zeros(1,3);
gyro  = zeros(1,3);

k = 1;

Fs = 50;


% figure(1); hold on; 
% figure; hold on;
% hAxes(1) = gca;
% 
% h1 = animatedline(hAxes(1), 'Color','r');
% h2 = animatedline(hAxes(1), 'Color','g');
% h3 = animatedline(hAxes(1), 'Color','b');
% 
% legend('x (rad)', 'y (rad)', 'z (rad)');
% ylabel('Euler Angles (rad)'); xlabel('Samples');
% title('Orientation','fontsize', 12);
% grid minor;
% hold off;
% 
% figure; hAxes(2) = gca;
% h4 = animatedline(hAxes(2), 'Color','r');
% h5 = animatedline(hAxes(2), 'Color','g');
% h6 = animatedline(hAxes(2), 'Color','b');
% 
% legend('A_x (m/s^2)','A_y (m/s^2)','A_z (m/s^2)');
% ylabel('Acceleration (m/s^2)'); xlabel('Samples');
% title('Accelerometer','fontsize', 12);
% grid minor;
% hold off;
% 
% figure; hAxes(3) = gca;
% h7 = animatedline(hAxes(3), 'Color','r');
% h8 = animatedline(hAxes(3), 'Color','g');
% h9 = animatedline(hAxes(3), 'Color','b');
% 
% legend('x (rad)', 'y (rad)', 'z (rad)');
% ylabel('Euler Angles (rad)'); xlabel('Samples');
% title('Fused orientation','fontsize', 12);
% grid minor;
% hold off;
% 
% figure; hAxes(4) = gca;
% h10 = animatedline(hAxes(4), 'Color','r');
% h11 = animatedline(hAxes(4), 'Color','g');
% h12 = animatedline(hAxes(4), 'Color','b');
% 
% legend('A_x (m/s^2)','A_y (m/s^2)','A_z (m/s^2)');
% ylabel('Acceleration (m/s^2)'); xlabel('Samples');
% title('Fused Accelerometer','fontsize', 12);
% grid minor;
% hold off;

%% Load background image

% BF2
% plot_scaling_f = 10;
% pos_offset.x = 258;
% pos_offset.y = 328;

% BF2
plot_scaling_f = 1;
pos_offset.x = 0;
pos_offset.y = 0;

% back_img = imread("bf1_ver1.png");
% back_img = imread("bf2_gray.png"); % BF2

%%
init_pos.x = -2.43;
init_pos.y = 0;
init_pos.z = 0;

%% Anchor coordinates (based on BF2 map)
% uwb_anchor_991B.x =  (396 - pos_offset.x)/10;
% uwb_anchor_991B.y =  (357 - pos_offset.y)/10;
% uwb_anchor_991B.z =  0.45;
% 
% uwb_anchor_4630.x =  nan;
% uwb_anchor_4630.y =  nan;
% uwb_anchor_4630.z =  nan;
% 
% uwb_anchor_4302.x =  (372 - pos_offset.x)/10;
% uwb_anchor_4302.y =  (497 - pos_offset.y)/10;
% uwb_anchor_4302.z =  0.45;
% 
% uwb_anchor_1221.x =  0;
% uwb_anchor_1221.y =  0;
% uwb_anchor_1221.z =  0.45;
% 
% uwb_anchor_439D.x =  (831 - pos_offset.x)/10;
% uwb_anchor_439D.y =  (434 - pos_offset.y)/10;
% uwb_anchor_439D.z =  0.45;
% 
% uwb_anchor_89A4.x =  (425 - pos_offset.x)/10;
% uwb_anchor_89A4.y =  (230 - pos_offset.y)/10;
% uwb_anchor_89A4.z =  0.45;
% 
% uwb_anchor_4599.x =  nan;
% uwb_anchor_4599.y =  nan;
% uwb_anchor_4599.z =  nan;
% 
% uwb_anchor_9B8F.x =  (550 - pos_offset.x)/10;
% uwb_anchor_9B8F.y =  (375 - pos_offset.y)/10;
% uwb_anchor_9B8F.z =  0.45;

%% Office - UWB anchor coordinates

uwb_anchor_991B.x =  -7.39;
uwb_anchor_991B.y =  2.74;
uwb_anchor_991B.z =  0.5;

uwb_anchor_4630.x =  nan;
uwb_anchor_4630.y =  nan;
uwb_anchor_4630.z =  nan;

uwb_anchor_4302.x =  -0.87;
uwb_anchor_4302.y =  6.1;
uwb_anchor_4302.z =  0.5;

uwb_anchor_1221.x =  0;
uwb_anchor_1221.y =  0;
uwb_anchor_1221.z =  0.5;

uwb_anchor_439D.x =  nan;
uwb_anchor_439D.y =  nan;
uwb_anchor_439D.z =  nan;

uwb_anchor_89A4.x =  -7.27;
uwb_anchor_89A4.y =  0;
uwb_anchor_89A4.z =  0.5;

uwb_anchor_4599.x =  nan;
uwb_anchor_4599.y =  nan;
uwb_anchor_4599.z =  nan;

uwb_anchor_9B8F.x =  -4.42;
uwb_anchor_9B8F.y =  10.53;
uwb_anchor_9B8F.z =  0.5;
%%
figure;
% im = image(back_img);


% set(h, 'YDir','reverse');
h = animatedline('MaximumNumPoints',10,'LineWidth',3,'Color','r', 'Marker','o','MarkerSize',3);
% view(2);

% im.AlphaData = 0.5;

% axis([-2 2 -1 8 0 2]);
axis equal; 
        axis([-30 30 -30 30]);
        
% axis([-5 25 -5 25]);


%         xline(0.5, 'r:');
%         xline(-0.5, 'r:');
%         yline(6.3 - 0.45, 'b:');
%         yline(6.3 + 0.675, 'b:');
%         xline(8.55 - 0.45, 'b:');
%         xline(8.55 + 0.45, 'b:');
%         plot([530, 960], 'ro');

%% BF2 UWB anchor coordinates
im = drawpoint('Position',[plot_scaling_f*uwb_anchor_1221.x + pos_offset.x , ...
                           plot_scaling_f*uwb_anchor_1221.y + pos_offset.y], 'MarkerSize',10, 'Color','r');

im = drawpoint('Position',[plot_scaling_f*uwb_anchor_4302.x + pos_offset.x , ...
                           plot_scaling_f*uwb_anchor_4302.y + pos_offset.y], 'MarkerSize',10,'Color','g');

im = drawpoint('Position',[plot_scaling_f*uwb_anchor_9B8F.x + pos_offset.x ...
                           plot_scaling_f*uwb_anchor_9B8F.y + pos_offset.y], 'MarkerSize',10,'Color','b');

% im = drawpoint('Position',[plot_scaling_f*uwb_anchor_439D.x + pos_offset.x ...
%                            plot_scaling_f*uwb_anchor_439D.y + pos_offset.y], 'MarkerSize',10,'Color','k');

im = drawpoint('Position',[plot_scaling_f*uwb_anchor_991B.x + pos_offset.x ...
                           plot_scaling_f*uwb_anchor_991B.y + pos_offset.y], 'MarkerSize',10,'Color','m');

im = drawpoint('Position',[plot_scaling_f*uwb_anchor_89A4.x + pos_offset.x ...
                           plot_scaling_f*uwb_anchor_89A4.y + pos_offset.y], 'MarkerSize',10,'Color','c');


%%
view(2);
%         plot(-1.8,0, 'Marker','square');

m = 1;
l = 1;

gravity = 9.86;

gyro_phi = 0;
gyro_theta = 0;
gyro_psi = 0;
gyro_delta_phi = 0;
gyro_delta_theta = 0;
gyro_delta_psi = 0;
kf_phi = 0;
kf_theta = 0;
kf_psi = 0;
% b_acc = mean(IMUacc);
% b_acc = mean(IMUacc(1:100,:));
b_acc = [0 0 0]';
b_acc_n = [0 0 0]';

% b_gyro = mean(IMUgyro(1:100,:));
b_gyro = [0 0 0];
pos = zeros(3,1); vel = zeros(3,1);
b_gyro_his = zeros(3,1);
l = 100;
m = 100;
acc_b_m = [0 0 0]';

acc_g_bias = [0 0 0]';
b_acc_b = [0 0 0]';
acc_b_kf_wob = [0 0 0]';
Cb2n_kf = eye(3);
kf_phi_m = 0;
kf_theta_m = 0;
kf_phi = 0;
kf_theta = 0;
Cb2n = eye(3);

Lav = 1;

% b_acc_o =[ 0.0315    0.1288   -0.0798];
% b_acc_o =[ -0.2921+0.5-0.04    0.1928+0.1+0.05   -0.019-0.048];
b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original
% b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original

b_acc = [ -0.2921    0.1928   -0.019];  %% original
i = 1;
Acc_S = [0 0 0]';
Gyro_S = [0 0 0]';
dT = 0.1;
dt = 0.01;
P = 1;

Lv = 10;

IMUacc = zeros(2*Lv, 3);
IMUgyro = zeros(2*Lv, 3);
Acc_Acc = zeros(3,2*Lv );
Acc_Vel = zeros(3,2*Lv);
Acc_Pos = zeros(3,2*Lv);
Acc_PQR = zeros(3,2*Lv);
Acc_Steady = zeros(3,2*Lv);
UWB_LS_Pos = zeros(3,2*Lv);
UWB_M_Pos = zeros(3,2*Lv);
UWB_M_Vel = zeros(3,2*Lv);

%% Camera initialization (Camera on, checkerboard init)
% cam = webcam(1);
% RefChecker = checkerboard(26,4,3);

% [imagePointsRef,boardSize] = detectCheckerboardPoints(RefChecker);


% marker_pos = [0 0 0;0 1825 3650];
% figure(2);hold off;
% plot(marker_pos(1,:),marker_pos(2,:),'Rd');

%%

% cam_cal_idx = 0;

while(1)
%     [msgUwb,statusUwb,statustextUwb] = receive(subUwb);
%     [msgImu,statusImu,statustextImu] = receive(subImu);
    [msgUwb,statusUwb,statustextUwb] = receive(subUwb,1);
    [msgImu,statusImu,statustextImu] = receive(subImu,1);
%     [msgGps,statusGps,statustextGps] = receive(subGps,1);
    
    if(statusUwb ~= 0)
%         uwb.time = cellfun(@(m) double(m.Header.Stamp(1).Sec),msgUwb) + cellfun(@(m) double(m.Header.Stamp(1).Nsec),msgUwb)/10^9;
        uwb.time = double(msgUwb.Header.Stamp.Sec) +  double(msgUwb.Header.Stamp.Nsec/10^9);
        anchorID_tmp = double(msgUwb.AnchorID);
        if ~isempty(anchorID_tmp)
            uwb.anchorID = anchorID_tmp(1)';
        end
        uwb.activeNum = double(msgUwb.ActiveNum);
        range_tmp = double(msgUwb.Range);
        uwb.ranging = zeros(1, length(uwb.anchorID));
        for p=1:length(range_tmp)
            range = range_tmp(p);
            uwb.ranging(p,:) = range';
        end
        UWB = uwb;
        UWB.dist = UWB.ranging';
        UWB.activeIndex = find(UWB.dist>0);
        UWB.num = length(UWB.activeIndex);
        UWB.anchor = UWB.anchorID';
        UWB.anchor(:,4) = -2;
        UWB.anchor(1,2:4) = [uwb_anchor_991B.x uwb_anchor_991B.y uwb_anchor_991B.z]; % 991B [Y, X]
        UWB.anchor(2,2:4) = [uwb_anchor_4630.x uwb_anchor_4630.y uwb_anchor_4630.z]; % 4630
        UWB.anchor(3,2:4) = [uwb_anchor_4302.x uwb_anchor_4302.y uwb_anchor_4302.z]; % 4302
        UWB.anchor(4,2:4) = [uwb_anchor_1221.x uwb_anchor_1221.y uwb_anchor_1221.z]; % 1221
        UWB.anchor(5,2:4) = [uwb_anchor_439D.x uwb_anchor_439D.y uwb_anchor_439D.z]; % 439D
        UWB.anchor(6,2:4) = [uwb_anchor_89A4.x uwb_anchor_89A4.y uwb_anchor_89A4.z]; % 89A4
        UWB.anchor(7,2:4) = [uwb_anchor_4599.x uwb_anchor_4599.y uwb_anchor_4599.z]; % 4599
        UWB.anchor(8,2:4) = [uwb_anchor_9B8F.x uwb_anchor_9B8F.y uwb_anchor_9B8F.z]; % 9B8F

        UWB.flag = true;

%         if(cam_cal_idx == 10)
%             CamLocPos = CamLocTest(cam, imagePointsRef)+[66 1948 0]'
% 
%             cam_cal_idx = 0;
%         else
%             cam_cal_idx = cam_cal_idx+1;
%         end

    else
        uwb = [];
        UWB = uwb;
        UWB.flag = false;
    end

    if statusImu == 1
%     IMUacc(i,:) = [msgImu.LinearAcceleration.X msgImu.LinearAcceleration.Y msgImu.LinearAcceleration.Z];
    IMUacc(1:end-1, :) = IMUacc(2:end, :);
    IMUacc(end,:) = [msgImu.LinearAcceleration.X msgImu.LinearAcceleration.Y msgImu.LinearAcceleration.Z];
%     IMUgyro(i,:)  = [msgImu.AngularVelocity.X msgImu.AngularVelocity.Y msgImu.AngularVelocity.Z];
    IMUgyro(1:end-1, :) = IMUgyro(2:end, :);
    IMUgyro(end,:)  = [msgImu.AngularVelocity.X msgImu.AngularVelocity.Y msgImu.AngularVelocity.Z];

%     if i>1
%         dt = IMUtime(i)-IMUtime(i-1);
%     else
        dt = 0.01;
%     end
    
    
    p = (IMUgyro(end,1)-b_gyro(1));    q = (IMUgyro(end,2)-b_gyro(2));    r = (IMUgyro(end,3)-b_gyro(3));
%     acc_b(:,i) = (IMUacc(end,:)-b_acc_o)';
    acc_b = (IMUacc(end,:)-b_acc_o)';
%     acc_b_filtered(:,i) = LP_Filter(acc_b(3,i)); 
    %%%% Phi, Theta, Psi %%%%%%%%%%%%%%%%%%%%%
    gyro_phi_prev = gyro_phi;
    gyro_theta_prev = gyro_theta;
    gyro_psi_prev = gyro_psi;
    kf_phi_prev = kf_phi;
    kf_theta_prev = kf_theta;
    kf_psi_prev = kf_psi;
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    if i>50
        used_phi = kf_phi;
        used_theta = kf_theta;
        used_psi = kf_psi;
        used_phi_next = kf_phi;
        used_theta_next = kf_theta;
        used_psi_next = kf_psi;

        Acc_Pos(:,1:end-1) = Acc_Pos(:,2:end);
        Acc_Vel(:,1:end-1) = Acc_Vel(:,2:end);
        Acc_PQR(:,1:end-1) = Acc_PQR(:,2:end);
        Acc_Steady(:,1:end-1) = Acc_Steady(:,2:end);
        Acc_Acc(:,1:end-1) = Acc_Acc(:,2:end);
        
%         vel_t = rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,i-1);
%         Acc_PQR(:,i) = 1*([0 vel_t(3) -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]);
%         Acc_Steady(:,i) = gravity*[sin(used_theta_next);-cos(used_theta_next)*sin(used_phi_next);-cos(used_theta_next)*cos(used_phi_next)];
%         Acc_Acc(:,i) = rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b(:,i)-Acc_PQR(:,i)-Acc_Steady(:,i));
        vel_t = rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,end-1);
        Acc_PQR(:,end) = 1*([0 vel_t(3) -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]);
        Acc_Steady(:,end) = gravity*[sin(used_theta_next);-cos(used_theta_next)*sin(used_phi_next);-cos(used_theta_next)*cos(used_phi_next)];
        Acc_Acc(:,end) = rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b-Acc_PQR(:,end)-Acc_Steady(:,end));
        
        [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));
        
        qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, gyro_psi);
        qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);
        AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
        
        [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
        kf_phi =real(kf_phi);
        kf_theta =real(kf_theta);
        kf_psi =real(kf_psi);
        
        Acc_Vel(:,end) = Acc_Vel(:,end-1) + Acc_Acc(:,end)*dt;
        Acc_Pos(:,end) = Acc_Pos(:,end-1) + Acc_Vel(:,end-1)*dt + Acc_Acc(:,end)*dt^2/2;
        
    else
        Acc_Vel(:,1:end-1) = Acc_Vel(:,2:end);
        Acc_Acc(:,1:end-1) = Acc_Acc(:,2:end);
        Acc_Pos(:,1:end-1) = Acc_Pos(:,2:end);

        Acc_Acc(:,end) = [0 0 0]';
        Acc_Vel(:,end) = [0 0 0]';
        Acc_Pos(:,end) = [init_pos.x init_pos.y init_pos.z]';

        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));
        gyro_phi = gyro_phi*(i-1)/i+acc_b_phi/i;
        kf_phi = gyro_phi;
        gyro_theta = gyro_theta*(i-1)/i+acc_b_theta/i;
        kf_theta = gyro_theta;
        gyro_psi = pi/2;
        b_gyro = b_gyro*(i-1)/i+IMUgyro(end,:)/i;
    end
    
    if ((i>50)&&(UWB.flag))  % INS/UWB
        if UWB.num >= 1
            [UWB] = uwb_LS_positioning_3(UWB, Acc_Pos(:,end));
%             [UWB] = uwb_LS_positioning_2(UWB);
            UWB_LS_Pos(:,1:end-1) = UWB_LS_Pos(:,2:end);
            UWB_LS_Pos(:,end) = UWB.pos;
            
        else
            UWB_LS_Pos(:,1:end-1) = UWB_LS_Pos(:,2:end);
            UWB_LS_Pos(:,end) = UWB_LS_Pos(:,end-1);
            
        end
        

        if (k > Lv*2)
            Ta = mean(UWB_LS_Pos(:,end-Lv+1:end)')';
            Tb = mean(UWB_LS_Pos(:,end-2*Lv+1:end-Lv)')';
            Tm = (Ta+Tb)/2;
            Td = Ta-Tm;
            UWB_M_Pos(:,1:end-1) = UWB_M_Pos(:,2:end);
            UWB_M_Vel(:,1:end-1) = UWB_M_Vel(:,2:end);
            UWB_M_Pos(:,end) = (Tm+Td);
            UWB_M_Vel(:,end) = (UWB_M_Pos(:,end)-UWB_M_Pos(:,end-9))/1;
        else
            UWB_M_Pos(:,1:end-1) = UWB_M_Pos(:,2:end);
            UWB_M_Vel(:,1:end-1) = UWB_M_Vel(:,2:end);
            
            UWB_M_Pos(:,end) = [init_pos.x init_pos.y init_pos.z]';
            UWB_M_Vel(:,end) = [0 0 0]';
        end
        
            R = 1e-2*eye(6);
%             AQ = min([abs(UWB_M_Pos(2,end)-0) abs(UWB_M_Pos(2,end)-13) abs(UWB_M_Pos(2,end)-26)]);
%             AQ = min(abs(UWB.ranging(UWB.activeIndex)));
%             R(1,1) = AQ^2/40;
%             R(2,2) = AQ^2/40;
% 
% %             R(1,1) = 1;
%             
%             R(3,3) = 1e-3;
%             R(4,4) = AQ^2/40;
% %             R(4,4) = 1;
%             R(5,5) = AQ^2/40;
% %           
%             R(6,6) = 1e-3;            

        TEMP = [IMUacc(end,:)'-Acc_Steady(:,end)];
        TEMP_bias = b_acc_o';
        x = [Acc_Pos(:,end-1);Acc_Vel(:,end-1);TEMP;TEMP_bias];
        
        Cb2n = rotationVectorToMatrix([used_phi;used_theta;used_psi])';
        A = eye(12);
%         A(1:3,4:6) = dt*eye(3);
%         A(1:3,7:9) = Cb2n*dt^2/2;
%         A(1:3,10:12) = -Cb2n*dt^2/2;
%         A(4:6,7:9) = Cb2n*dt;
%         A(4:6,10:12) = -Cb2n*dt;
        A(1:3,4:6) = dt*eye(3)-1/2*dt^2*[0 -r q;r 0 -p;-q p 0];
        A(1:3,7:9) = Cb2n*dt^2/2;
        A(1:3,10:12) = -Cb2n*dt^2/2;
        A(4:6,4:6) = A(4:6,4:6) - dt*[0 -r q;r 0 -p;-q p 0];
        A(4:6,7:9) = Cb2n*dt;
        A(4:6,10:12) = -Cb2n*dt;
        
        
        z = [UWB_M_Pos(:,end);UWB_M_Vel(:,end)];
%         z = [UWB_LS_Pos(:,k);UWB_LS_Vel(:,k)];
        
        x_n = PosKalman(x,A,z,R);
        
        Acc_Pos(:,end) = x_n(1:3,1);
%         curr_pos    = x_n(1:3,1)
% 
% %         addpoints(h,UWB_M_Pos(1,k), UWB_M_Pos(2,k), UWB_M_Pos(3,k));
%         addpoints(h,curr_pos(1), curr_pos(2), curr_pos(3));
%         xline(0.5, 'r:');
%         xline(-0.5, 'r:');
        
        drawnow;

        Acc_Vel(:,end) = x_n(4:6,1);
        acc_g = x_n(7:9,1);
        b_acc_o = x_n(10:12,1)';
%         
%         [acc_b_phi,acc_b_theta] = EulerAcc(IMUacc(i,1)'-acc_g(1)-Acc_PQR(1,i)-b_acc_o(1), IMUacc(i,2)'-acc_g(2)-Acc_PQR(2,i)-b_acc_o(2), IMUacc(i,3)'-acc_g(3)-Acc_PQR(3,i)-b_acc_o(3));
%         
%         qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, gyro_psi);
%         qua_gyro = EulerToQuaternion(gyro_phi_t(i-1), gyro_theta_t(i-1), gyro_psi_t(i-1));
% 
%         AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
%         [gyro_phi,gyro_theta,gyro_psi] = EulerKalman_2(qua_gyro,AA,qua_acc);
% %         
        k = k + 1;
        
        UWB.flag = false;
        Acc_S = [0 0 0]';
    end
%     plot(k, euler_q(1),'r'); plot(k, euler_q(2),'g'); plot(k, euler_q(3),'b');
%  
%     addpoints(h1,k,gyro(k,1));
%     addpoints(h2,k,gyro(k,2));
%     addpoints(h3,k,gyro(k,3));
% 
%     addpoints(h4,k,accel(k,1));
%     addpoints(h5,k,accel(k,2));
%     addpoints(h6,k,accel(k,3));
% 
%     addpoints(h7,k,euler_q(1));
%     addpoints(h8,k,euler_q(2));
%     addpoints(h9,k,euler_q(3));
% 
%     addpoints(h10,k,angularVelocity(1));
%     addpoints(h11,k,angularVelocity(2));
%     addpoints(h12,k,angularVelocity(3));
% 
%     drawnow
%     if mod(i,10)==0
%     addpoints(h,UWB_M_Pos(1,end), UWB_M_Pos(2,end), UWB_M_Pos(3,end));

%         addpoints(h,Acc_Pos(1,end), Acc_Pos(2,end), Acc_Pos(3,end));
        Acc_Pos(:,end);
%         a_tmp = [Acc_Pos(2,end)*plot_scaling_f + pos_offset.x, Acc_Pos(1,end)*plot_scaling_f + pos_offset.y, Acc_Pos(3,end)*plot_scaling_f]
%         scaled_pos.x = Acc_Pos(2,end)*plot_scaling_f + pos_offset.x;
%         scaled_pos.y = Acc_Pos(1,end)*plot_scaling_f + pos_offset.y;
%         scaled_pos.z = abs(Acc_Pos(3,end)*plot_scaling_f);
        scaled_pos.x = Acc_Pos(1,end)*plot_scaling_f + pos_offset.x;
        scaled_pos.y = Acc_Pos(2,end)*plot_scaling_f + pos_offset.y;
        scaled_pos.z = abs(Acc_Pos(3,end)*plot_scaling_f);
       

%         if(scaled_pos.z < 0)
%             scaled_pos.z = 0;
%         end
%         addpoints(h,scaled_pos.x, scaled_pos.y, scaled_pos.z);
%                 xline(0.5, 'r:');
%         xline(-0.5, 'r:');
        drawnow;
%     end
    i = i+1;
    end
end

% figure(2); hold on;
% title('Orientation Estimate');
% 
% ylabel('Degrees');
% q2 = fuse(accel, gyro);
% plot(eulerd(q2, 'XYZ', 'frame'));
% legend('X-rotation', 'Y-rotation', 'Z-rotation');