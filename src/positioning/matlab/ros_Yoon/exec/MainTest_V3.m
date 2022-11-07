clear;

close all; clc;clear all;

addpath('libs');
addpath('rosbag');
addpath('map');


NaviMode =1 % (1)INS/GPS  (2)INS/LiDAR  (3)INS/UWB  (4)Navi(INS/GPS/UWB/LiDAR)
Ani.save = false;
Ani.play = false;

fname = 'rosbag\test_b1_line_20220803_v1.mat';
% fname = 'rosbag\line001_220304.mat';
% fname = 'rosbag\test_6F_1_20220727_v2.mat';
% fname = 'rosbag\test_o_line_20220729_v3.mat';
% fname = 'rosbag\test_20220801_v1.mat';

% fname = 'rosbag\stop002_220304.mat';



if exist(fname)
    disp('data loading..');
    load(fname)
else
    disp('data parsing..');
    fname_tmp = extractBefore(fname,".");
    fname = insertAfter(fname_tmp,length(fname_tmp),".bag");
%     [Sens] = rosbag_parsing(fname);
    [Sens] = rosbag_parsing_sh_v1(fname);   % revised by Seonghyun Jang
end

if ~isempty(extractBefore(extractAfter(fname,'sensor'),'.mat'))
    [Ref] = reference_rosbag_parsing(fname);
end

dt = Sens.imu.time(2:end)-Sens.imu.time(1:end-1);
acc = Sens.imu.acc;
gyro = Sens.imu.gyro;
simtime = Sens.imu.time(2:end)-Sens.imu.time(1);
qua_gyro = [1 0 0 0]';
gyro_bias = mean(gyro(1:500,:));
acc_bias = mean(acc(1:500,:));
gravity = norm(acc_bias);

[phi_bias, theta_bias] = EulerAcc(acc_bias(1), acc_bias(2), acc_bias(3));
Cacc_bias = qua2dcm(EulerToQuaternion(phi_bias, theta_bias, 0));
% Cacc_bias = Cacc_bias';
% phi_bias = 0; theta_bias = 0;
phi = 0; theta = 0; psi = 0;

eurl_gyro = zeros(3,1);
v_e = zeros(3,1000);
v_b = zeros(3,1000);
a_e = zeros(3,1000);
a_b = zeros(3,1000);
pos = zeros(3,1000);
C_b2n_next = eye(3);
C_n2b_next = eye(3);
qua_gyro = EulerToQuaternion(phi_bias, theta_bias, 0);    
att_kf = [1 0 0 0]';

for k = 2 : length(dt)
    p = (gyro(k,1)-gyro_bias(1));
    q = (gyro(k,2)-gyro_bias(2));
    r = (gyro(k,3)-gyro_bias(3));
    
    ax = acc(k,1);
    ay = acc(k,2);
    az = acc(k,3);
    
    a_b = Cacc_bias*[ax ay az]';
    ax = a_b(1); ay = a_b(2); az = a_b(3);
    
    A = eye(4)+dt(k-1)*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    qua_gyro = A*qua_gyro;
    eurl_gyro(:,k)= dcm2eulr(qua2dcm(qua_gyro));

    [phi_temp, theta_temp] = EulerAcc(ax, ay, az);
    phi_org(k) = phi_temp;
    theta_org(k) = theta_temp;
    ax_c(k) = ax;
    ay_c(k) = ay;
    az_c(k) = az;
%     gravity_t = norm([ax,ay,az]);
%     ax_c(k) = gravity_t*sin(theta_org(k));
%     ay_c(k) = -gravity_t*cos(theta_org(k))*sin(phi_org(k));
%     az_c(k) = -gravity_t*cos(theta_org(k))*cos(phi_org(k));
    
     z = EulerToQuaternion(phi_org(k)+phi_bias, theta_org(k)+theta_bias, eurl_gyro(3,k));
    [phi(k), theta(k), psi(k)] = EulerKalman(A, z);
    att_kf = EulerToQuaternion(phi(k)-phi_bias, theta(k)-theta_bias, psi(k));
    C_b2n = C_b2n_next;
    C_n2b = C_n2b_next;
    C_b2n_next = qua2dcm(att_kf);
    C_n2b_next = C_b2n_next';
%     C_b2n = C_b2n_next;
%     C_n2b = C_n2b_next;
%     C_n2b_next = qua2dcm(att_kf);
%     C_b2n_next = C_n2b_next';   
    v_b = C_n2b*v_e(:,k-1);
    vx_b = v_b(1);vy_b=v_b(2);vz_b=v_b(3);
%     a_e(:,k) = C_b2n*[ax ay az]';
    a_e(:,k) = C_b2n*[ax_c(k) ay_c(k) az_c(k)]';
    ax_e = a_e(1,k);ay_e = a_e(2,k);az_e = a_e(3,k);
    
    [euler_angle] = dcm2eulr(qua2dcm(att_kf));
    temp_b(:,k) = gravity*[sin(theta(k)-theta_bias);-cos(theta(k)-theta_bias)*sin(phi(k)-phi_bias);-cos(theta(k)-theta_bias)*cos(phi(k)-phi_bias)];
    temp_c(:,k) = Cacc_bias*[0 vz_b -vy_b; -vz_b 0 vx_b;vy_b -vx_b 0]*[p;q;r];
%     acc_b(:,k) = ([ax_c(k) ay_c(k) az_c(k)]'-temp_b);
    acc_b(:,k) = ([ax_c(k) ay_c(k) az_c(k)]'+[0 vz_b -vy_b; -vz_b 0 vx_b;vy_b -vx_b 0]*[p;q;r]-temp_b(:,k));
%     acc_b(:,k) = ([ax_c(k) ay_c(k) az_c(k)]'-temp_c(:,k)-temp_b(:,k));
%     acc_b(:,k) = ([ax_c(k) ay_c(k) az_c(k)]'-temp_b(:,k));

    acc_e(:,k) = C_b2n*(acc_b(:,k));
    
    if k < 500
        v_e(:,k) = 0;
        pos(:,k) = 0;
        att_kf = EulerToQuaternion(0, 0, 0);
        C_b2n = eye(3);
        C_n2b = eye(3);
        C_b2n_next = qua2dcm(att_kf);
        C_n2b_next = C_b2n_next';
    else
        v_e(:,k) = v_e(:,k-1)+acc_e(:,k)*dt(k-1);
        pos(:,k) = pos(:,k-1) + v_e(:,k-1)*dt(k-1) + 1/2*dt(k-1)^2*acc_e(:,k);
    end
        
end


figure(1);hold off;plot(eurl_gyro(1,:));hold on;plot(eurl_gyro(2,:),'r');plot(eurl_gyro(3,:),'g');plot(phi,'m');hold on;plot(theta,'k');plot(psi,'.b')
figure(3);hold off;plot(eurl_gyro(1,:));hold on;plot(eurl_gyro(2,:),'r');plot(eurl_gyro(3,:),'g');plot(phi_org,'m');hold on;plot(theta_org,'k');
figure(4);hold off;plot(pos(1,:),pos(2,:))
figure(4);hold on;plot([pos(1,:);pos(1,:)+acc_e(1,:)],[pos(2,:);pos(2,:)+acc_e(2,:)],'-r')

figure(5);hold off;plot(simtime,v_e(1,:));hold on;plot(simtime,v_e(2,:),'r');plot(simtime,v_e(3,:),'g')
figure(6);hold off;plot(simtime,ax_c);hold on;plot(simtime,ay_c,'r');plot(simtime,az_c,'g')
figure(7);hold off;plot(simtime,acc_e(1,:));hold on;plot(simtime,acc_e(2,:),'r');plot(simtime,acc_e(3,:),'g')
figure(8);hold off;plot(simtime,acc_b(1,:));hold on;plot(simtime,acc_b(2,:),'r');plot(simtime,acc_b(3,:),'g')
figure(9);hold off;plot(simtime,temp_b(1,:));hold on;plot(simtime,temp_b(2,:),'r');plot(simtime,temp_b(3,:),'g')
figure(10);hold off;plot(simtime,temp_c(1,:));hold on;plot(simtime,temp_c(2,:),'r');plot(simtime,temp_c(3,:),'g')
figure(11);hold off;plot(simtime,ax_c-temp_b(1,:));hold on;plot(simtime,ay_c-temp_b(2,:),'r');plot(simtime,az_c-temp_b(3,:),'g')

figure(2);hold off;plot(phi);hold on;plot(theta,'r');plot(psi,'g')

% figure(13);hold off;plot(att_euler(1,:));hold on;plot(att_euler(2,:),'r');plot(att_euler(3,:),'g')

    
    