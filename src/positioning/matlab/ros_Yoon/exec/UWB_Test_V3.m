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
UWB_pos_ave = [0 0 0]';
IMU_a_ave_prev = [0 0 0]';
kk = 1;kkk = 1;ll = 1;
UWB_pos_S = [0 0 0]';
for i = 1 : Sens.length
    
    [IMU] = func_extract_imu(Sens, i);
    if Nav.mode == 0
        [Nav] = ins_alignment(Nav, IMU, i, NaviMode);
%         TimeStamp = IMU.time;
    else
        [Nav] = navi_propagation_15state_5(Nav, IMU, i);
        IMU_a = IMU.acc;
        IMU_w = IMU.gyro;
    end
    
    p_ned = Nav.output.pos;
    v_ned = Nav.output.vel;
    x_att = Nav.output.att;
    acc_e = Nav.output.acc;

    if Sens.flag(i,4) > 0
        dt_acc = IMU.time - TimeStamp;
        acc_t = acc_e_t;
        acc_e_t = 0;
        [UWB] = func_extract_uwb(Sens, i, fname);
        [UWB] = uwb_LS_positioning_2(UWB);
        UWB_pos(:,k) = UWB.pos;
        UWB_pos_time(k) = IMU.time;
        
        if dt_acc > 1
            TimeStamp = IMU.time;
            UWB_pos_S(:,kkk) = zeros(3,1)+UWB_pos_ave;
            UWB_pos_S_time(kkk) = IMU.time;
            IMU_a_S(:,kkk) = zeros(3,1)+IMU_a_ave;
            
            p = IMU_w_ave(1); q = IMU_w_ave(2); r = IMU_w_ave(3);
            AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0];

            att_next_hat = AA*att_prev;
            
            temp = (UWB_pos_S(:,kkk)*UWB_pos_S(:,kkk)')*(
            z = EulerToQuaternion(phi_org+phi_bias, theta_org+theta_bias, eurl_gyro(3));
            [phi, theta, psi] = EulerKalman(AA, z);
            att_hor = EulerToQuaternion(phi-phi_bias, theta-theta_bias, psi);
            
            
            
            IMU_a_ave_prev = IMU_a_ave;
            att_prev = Nav.output.att;
            kk = 1;
            kkk = kkk + 1;
            UWB_pos_ave = [0 0 0]';
            IMU_a_ave = [0 0 0]';
            ll = 1;
        else
            UWB_pos_ave = (kk-1)/kk*UWB_pos_ave+UWB_pos(:,k)/kk;
            kk = kk + 1;
        end
        
        IMU_a_ave = IMU_a_ave*(ll-1)/ll+IMU_a/ll;
        IMU_w_ave = IMU_w_ave*(ll-1)/ll+IMU_w/ll;
        
        k = k + 1;
    end
    
end



figure(1);plot(UWB_pos(1,:), UWB_pos(2,:),'r');
figure(2);hold off;plot(UWB_pos(1,:));hold on;plot( UWB_pos(2,:),'r');plot(UWB_pos(3,:),'g')

figure(3);plot(UWB_pos_S(1,:), UWB_pos_S(2,:),'r');
figure(4);hold off;plot(UWB_pos_S(1,:));hold on;plot( UWB_pos_S(2,:),'r');plot(UWB_pos_S(3,:),'g')

figure(5);hold off;plot(UWB_pos_S_time-0.5,UWB_pos_S(1,:));hold on;plot(UWB_pos_time,UWB_pos(1,:),'r');
figure(6);hold off;plot(UWB_pos_S_time-0.5,UWB_pos_S(2,:));hold on;plot(UWB_pos_time,UWB_pos(2,:),'r')

% figure(5);hold off;plot(CONV_pos(1,:), CONV_pos(2,:)); hold on;plot(CONV_pos(1,:), CONV_pos(2,:),'r');

