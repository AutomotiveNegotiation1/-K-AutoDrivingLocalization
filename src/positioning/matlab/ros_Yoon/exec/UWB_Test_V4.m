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
[ Nav, Grp ] = init_variable();
Nav.mode = 0;

    
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
        
        p_ned = Nav.output.pos;
        v_ned = Nav.output.vel;
        x_att = Nav.output.att;
        acc_e = Nav.output.acc;
        b_gyro = Nav.imu_bias.gyro;
        b_acel = Nav.imu_bias.acel;

        if Sens.flag(i,4) > 0
%             dt_acc = IMU.time - TimeStamp;
%             acc_t = acc_e_t;
%             acc_e_t = 0;
            [UWB] = func_extract_uwb(Sens, i, fname);
            [UWB] = uwb_LS_positioning_2(UWB);
            UWB_pos(:,k) = UWB.pos;
            
            anchor = UWB.anchor(UWB.activeIndex,:);
            P = Nav.KF.P(1:15,1:15);
            
            zhat = p_ned;
            H = zeros(3,15);
            H(:,1:3) = eye(3);
            
            R = 10*eye(3);
                                    

%             zhat_tmp = zeros(1,UWB.num);
%             for ii=1:UWB.num
%                 zhat_tmp(ii) = norm(anchor(ii,2:4) - (p_ned'+[0 0 0.45]));
%             end
%             zhat = zhat_tmp';
%             
%             los = zeros(UWB.num,3);
%             for ii=1:UWB.num
%                 los(ii,:) = (anchor(ii,2:4) - (p_ned'+[0 0 0.45])) / norm(anchor(ii,2:4) - (p_ned'+[0 0 0.45]));
%             end
%             
%             H = zeros(UWB.num,15);
%             H(:,1:3) = -los;
%             
%             R = Nav.KF.R_UWB(1:UWB.num,1:UWB.num);
%             Z = UWB.dist(UWB.activeIndex) - zhat;
             
            Z = UWB_pos(:,k)-zhat;
            
            %     %%% Altitude
            %     H(end+1,3) = 1;
            %     R(end+1,end+1) = 0.01;
            %     Z(end+1) = 0 - p_ned(3);
            
            
            K = P*H'/(H*P*H'+R);
            P = P - K*H*P;
            err_X = zeros(15,1) + K*Z;
            
            
            p_ned = p_ned + err_X(1:3);
            v_ned = v_ned + err_X(4:6);
            x_att = Qua_Update(ang2qua(err_X(7:9)),x_att);
            b_acel = b_acel + err_X(10:12);
            Nav.output.pos = p_ned;
            Nav.output.vel = v_ned;
            Nav.output.att = x_att;
%             Nav.output.acc = acc_e;
            b_gyro = b_gyro + err_X(13:15);
            Nav.imu_bias.gyro = b_gyro;
            k = k + 1;
        end
    end
    TotPos(:,k) = zeros(3,1)+Nav.output.pos;

end





figure(1);hold off;plot(UWB_pos(1,:), UWB_pos(2,:),'b');hold on;plot(TotPos(1,:), TotPos(2,:),'r');

figure(2);hold off;plot(TotPos(1,:));hold on;plot( TotPos(2,:),'r');%plot(TotPos(3,:),'g')

% figure(2);hold off;plot(UWB_pos(1,:));hold on;plot( UWB_pos(2,:),'r');plot(UWB_pos(3,:),'g')
% 
% figure(3);plot(UWB_pos_S(1,:), UWB_pos_S(2,:),'r');
% figure(4);hold off;plot(UWB_pos_S(1,:));hold on;plot( UWB_pos_S(2,:),'r');plot(UWB_pos_S(3,:),'g')
% 
% figure(5);hold off;plot(UWB_pos_S_time-0.5,UWB_pos_S(1,:));hold on;plot(UWB_pos_time,UWB_pos(1,:),'r');
% figure(6);hold off;plot(UWB_pos_S_time-0.5,UWB_pos_S(2,:));hold on;plot(UWB_pos_time,UWB_pos(2,:),'r')
% 
% % figure(5);hold off;plot(CONV_pos(1,:), CONV_pos(2,:)); hold on;plot(CONV_pos(1,:), CONV_pos(2,:),'r');

