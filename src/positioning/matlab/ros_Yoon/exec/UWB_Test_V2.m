% clear all;
close all;

fname = 'rosbag\test_b1_line_20220803_v1.mat';

% load(fname,'Sens');
TimeStart = 0;
TimeEnd = 0;
AlignOn = 1;
InitPos = [0 0 0]';
k = 1;
acc_e_t = 0;
NaviMode = 4;
TimeStamp = 0;

H = eye(9);

Q = 1e-4*eye(9);
R = 10*[eye(3) eye(3) eye(3);eye(3) eye(3) eye(3);eye(3) eye(3) eye(3)];

P = 1*eye(9);
dt_acc = 0.1;
Nav.mode = 0;

Nav.imu_bias.gyro = 0;
Nav.imu_align.g_body = 0;
Nav.imu_align.time = 10;
Nav.output.pos = [0 0 0]';
Nav.output.vel = [0 0 0]';
Nav.output.acc = [0 0 0]';
Nav.KF.P = zeros(15,15);
Nav.KF.P(1:3,1:3) = 1 * eye(3,3); % (위치) original
Nav.KF.P(4:6,4:6) = 0.005 * eye(3,3); % (속도) original
Nav.KF.P(7:9,7:9) = 1.0e-03 * 0.4575 * eye(3,3); % (자세) original
Nav.KF.P(10:12,10:12) = 1.0e-05 * eye(3,3); % (3축 가속도) original
Nav.KF.P(13:15,13:15) = 1.0e-05 * eye(3,3); % (3축 각속도) original
Nav.KF.Q = zeros(6,6);
Nav.KF.Q(1:3,1:3) = diag(1.0e-01*ones(1,3)); % original
Nav.KF.Q(4:6,4:6) = diag(1.0e-02*ones(1,3)); % original
acc_e = 0;
gg = 1;
v_ned = 0;
for i = 1 : Sens.length
    
    [IMU] = func_extract_imu(Sens, i);
    if Nav.mode == 0
        [Nav] = ins_alignment(Nav, IMU, i, NaviMode);
%         TimeStamp = IMU.time;
    else
        [Nav] = navi_propagation_15state_5(Nav, IMU, i);
    end
    p_ned = Nav.output.pos;
    v_ned = Nav.output.vel;
    x_att = Nav.output.att;
    acc_e = Nav.output.acc;
    if Sens.flag(i,4) > 0
        dt_prev = dt_acc;
        dt_acc = IMU.time - TimeStamp;
        TimeStamp = IMU.time;
        acc_t = acc_e_t;
        acc_e_t = 0;
        [UWB] = func_extract_uwb(Sens, i, fname);
        [UWB] = uwb_LS_positioning(UWB);
        
        UWB_pos(:,k) = UWB.pos;
        if (k>3) && (Nav.mode~=0) && (sum(isnan(UWB.pos))<1)
            if (((UWB_pos(:,k)-UWB_pos(:,k-1))/dt_acc-(UWB_pos(:,k-1)-UWB_pos(:,k-2))/dt_prev)/dt_acc<10)
                UWB_vel(:,k) = (UWB_pos(:,k)-UWB_pos(:,k-1))/dt_acc;
                UWB_acc(:,k) = ((UWB_pos(:,k)-UWB_pos(:,k-1))/dt_acc-(UWB_pos(:,k-1)-UWB_pos(:,k-2))/dt_prev)/dt_acc;
            else
                UWB_pos(:,k) = UWB_pos(:,k);
                UWB_vel(:,k) = UWB_vel(:,k);
                UWB_acc(:,k) = UWB_acc(:,k);
            end
            x =[p_ned_prev;v_ned_prev;acc_t];
            A = [1 0 0 dt_acc 0 0 dt_acc^2 0 0;
                0 1 0 0 dt_acc 0 0 dt_acc^2 0;
                0 0 1 0 0 dt_acc 0 0 dt_acc^2;
                0 0 0 1 0 0 dt_acc 0 0;
                0 0 0 0 1 0 0 dt_acc 0;
                0 0 0 0 0 1 0 0 dt_acc;
                0 0 0 0 0 0 1 0 0;
                0 0 0 0 0 0 0 1 0;
                0 0 0 0 0 0 0 0 1];
            z = [UWB_pos(:,k);UWB_vel(:,k-1);UWB_acc(:,k)];
                
            xp = A*x;
            Pp = A*P*A' + Q;
            
            K = Pp*H'*inv(H*Pp*H' + R);
            
            x = xp + K*(z - H*xp);
            P = Pp - K*H*Pp;
            Nav.output.pos = x(1:3);
            Nav.output.vel = x(4:6);
            %                 Nav.output.att;
            Nav.output.acc = x(7:9);
            
    
        end
        v_ned_prev = Nav.output.vel;
        p_ned_prev = Nav.output.pos;
        k = k + 1;
        gg = 1;
    else
%         acc_e_t = acc_e_t + acc_e;
        acc_e_t = acc_e_t*(gg-1)/gg + acc_e/gg;

        gg = gg + 1;
        UWB.flag = false;
    end
    
    CONV_pos(:,i) = Nav.output.pos;
    CONV_vel(:,i) = Nav.output.vel;
    CONV_acc(:,i) = Nav.output.acc;
    
end

figure(1);plot(UWB_pos(1,:), UWB_pos(2,:),'r');
% figure(2);hold off;plot(UWB_pos_r(1,:), UWB_pos_r(2,:));
% figure(3);hold off;plot(UWB_pos_r(1,:));hold on;plot(UWB_pos(1,:),'r');
% figure(4);hold off;plot(UWB_pos_r(2,:));hold on;plot(UWB_pos(2,:),'r')

figure(5);hold off;plot(CONV_pos(1,:), CONV_pos(2,:)); hold on;plot(CONV_pos(1,:), CONV_pos(2,:),'r');

