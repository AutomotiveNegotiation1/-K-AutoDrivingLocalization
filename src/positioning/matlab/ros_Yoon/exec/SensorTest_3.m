clear all;
close all;

fname = 'rosbag\test_b1_line_20220803_v1.mat';
% fname = 'rosbag\test_o_line_20220729_v1.mat';
 
load(fname,'Sens');

IMUacc = Sens.imu.acc;
IMUgyro = Sens.imu.gyro;
IMUtime = Sens.imu.time;
% IMUacc(:,2) = -IMUacc(:,2);
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

    P = 0.1*[10 0 1 0;0 10 0 1;0 0 1 0;0 0 0 1];
    H = [eye(2) -eye(2);zeros(2,4)];
    R = [0.1 0 1 1;0 0.1 1 1;0 0 1 0;0 0 0 1];

    
% b_acc = mean(IMUacc(1:100,:));
b_acc = [0 0 0]';
% if(b_acc(3) > 9)
%     m_phi_bias = pi;
% else
%     m_phi_bias = 0;
% end
[acc_phi_bias,acc_theta_bias] = EulerAcc(b_acc(1), b_acc(2), b_acc(3));
% acc_phi_bias = acc_phi_bias + m_phi_bias;
Cbas = eulr2dcm([acc_phi_bias,acc_theta_bias,0]);

% UpSideDown = qua2dcm(EulerToQuaternion(m_phi_bias,0,0));

Cacc_bias = qua2dcm(EulerToQuaternion(acc_phi_bias, acc_theta_bias, 0));
b_gyro = mean(IMUgyro(1:1000,:));
pos = zeros(3,1); vel = zeros(3,1);
b_gyro_his = zeros(3,1);
l = 100;
m = 100;
acc_g_bias = [0 0 0]';
b_acc_b = [0 0 0]';
acc_b_kf_wob = [0 0 0]';
Cb2n_kf = eye(3);
kf_phi_m = 0;
kf_theta_m = 0;
kf_phi = 0;
kf_theta = 0;
for k = 1 : Sens.length
        if k>1
        dt = IMUtime(k)-IMUtime(k-1);
    else
        dt = 0.01;
        end
    
%     b_gyro = [0 0 0]';
    p = (IMUgyro(k,1)-b_gyro(1));    q = (IMUgyro(k,2)-b_gyro(2));    r = (IMUgyro(k,3)-b_gyro(3));
%         p = (IMUgyro(k,1)-b_gyro(1))*pi/180;    q = (IMUgyro(k,2)-b_gyro(2))*pi/180;    r = (IMUgyro(k,3)-b_gyro(3))*pi/180;

    acc_b_t(:,k) = IMUacc(k,:)';
    [acc_phi_bias,acc_theta_bias] = EulerAcc(b_acc(1), b_acc(2), b_acc(3));
%     acc_phi_bias = acc_phi_bias + m_phi_bias;
    Cbas = eulr2dcm([acc_phi_bias,acc_theta_bias,0]);
    
    acc_b(:,k) = Cbas'*acc_b_t(:,k);
    
    [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
    
    [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));
    qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, 0);
    qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);
    AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];

    [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
    
    gravity = norm(b_acc);
    acc_n(:,k) = acc_b(:,k)-gravity*[sin(kf_theta);-cos(kf_theta)*sin(kf_phi);-cos(kf_theta)*cos(kf_phi)];
%     acc_n(:,k) = acc_b(:,k)-gravity*[sin(gyro_theta);-cos(gyro_theta)*sin(gyro_phi);-cos(gyro_theta)*cos(gyro_phi)];


    

    
    pos(:,k+1) = pos(:,k)+vel(:,k)*dt+1/2*dt^2*acc_n(:,k);
    vel(:,k+1) = vel(:,k)+acc_n(:,k)*dt;
    
%     acc_b_euler(:,k) = [acc_b_phi;acc_b_theta];
    Conv = 180/pi;
    kf_phi_t(k) = kf_phi*Conv;           kf_theta_t(k) = kf_theta*Conv;       kf_psi_t(k) = kf_psi*Conv;
    gyro_phi_t(k) = gyro_phi*Conv;      gyro_theta_t(k) = gyro_theta*Conv;    gyro_psi_t(k) = gyro_psi*Conv;
    acc_b_phi_t(k) = acc_b_phi*Conv;      acc_b_theta_t(k) = acc_b_theta*Conv;    acc_b_psi_t(k) = 0*Conv;
end

picnum = 0;
% figure(picnum);hold off;plot(kf_phi_m_t);hold on;plot(kf_phi_t,'m');plot(gyro_phi_t,'r');title('phi compare')
picnum=picnum+1;figure(picnum);hold off;plot(gyro_phi_t*pi/180);hold on;plot(gyro_theta_t*pi/180,'m');plot(gyro_psi_t*pi/180,'r');title('gyro euler');legend('phi','theta','phi')
picnum=picnum+1;figure(picnum);hold off;plot(kf_phi_t);hold on;plot(kf_theta_t,'m');plot(kf_psi_t,'r');title('kf euler');legend('phi','theta','phi')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_phi_t);hold on;plot(acc_b_theta_t,'m');plot(acc_b_psi_t,'r');title('kf euler');legend('phi','theta','phi')

picnum=picnum+1;figure(picnum);hold off;plot(acc_n(1,:));hold on;plot(acc_n(2,:),'r');plot(acc_n(3,:),'g');title('acc n')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t(1,:));hold on;plot(acc_b_t(2,:),'r');plot(acc_b_t(3,:),'g');title('acc b')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b(1,:));hold on;plot(acc_b(2,:),'r');plot(acc_b(3,:),'g');title('acc b bias compansated')
picnum=picnum+1;figure(picnum);hold off;plot(IMUgyro(:,1)-b_gyro(1));hold on;plot(IMUgyro(:,2)-b_gyro(2),'r');title('gyro original - bias')
picnum=picnum+1;figure(picnum);hold off;plot(vel(1,:));hold on;plot(vel(2,:),'m');plot(vel(3,:),'r');title('velocity')
picnum=picnum+1;figure(picnum);hold off;plot(pos(1,:));hold on;plot(pos(2,:),'m');plot(pos(3,:),'r');title('position')






