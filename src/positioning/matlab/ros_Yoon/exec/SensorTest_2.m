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

    
b_acc = mean(IMUacc(1:100,:));

[acc_phi_bias,acc_theta_bias] = EulerAcc(b_acc(1), b_acc(2), b_acc(3));
if(b_acc(3) > 9)
    m_phi_bias = pi;
else
    m_phi_bias = 0;
end
acc_phi_bias = acc_phi_bias + m_phi_bias;
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
    
    b_gyro = [0 0 0]';
    p = (IMUgyro(k,1)-b_gyro(1));    q = (IMUgyro(k,2)-b_gyro(2));    r = (IMUgyro(k,3)-b_gyro(3));
    acc_b(:,k) = Cbas'*IMUacc(k,:)';
    acc_b_t(:,k) = IMUacc(k,:)';
    kf_phi_a = atan(acc_b(2,k)/acc_b(3,k));
    kf_theta_a = atan(acc_b(1,k)/sqrt(acc_b(2,k)^2+acc_b(3,k)^2));
    
%     qua_acc = EulerToQuaternion(acc_phi_b, acc_theta_b, 0);    %% current
    qua_gyro = EulerToQuaternion(gyro_phi, gyro_theta, gyro_psi);   %% previous
    qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);           %% previous
    
    AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    qua_gyro = AA*qua_gyro;
    
    [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
    [zhat_phi,zhat_theta,zhat_psi] = EulerGyroUpdate(p,q,r,dt,kf_phi,kf_theta,kf_psi);

    zhat = [gyro_phi;gyro_theta];
%     zhat = [zhat_phi;zhat_theta];
    
%     if k < 100
%         Z = zhat - H(1:2,:)*[kf_phi_a;kf_theta_a;0;0];
%     else
        Z = zhat - H(1:2,:)*[kf_phi_a;kf_theta_a;kf_phi_m;kf_theta_m];
%     end

    K = P*H'/(H*P*H'+R);
    P = P - K*H*P;                
    err_X = zeros(4,1) + K(:,1:2)*Z;
    

    kf_phi = kf_phi+err_X(1);       kf_theta = kf_theta+err_X(2);
    kf_phi_m = kf_phi_m+err_X(3);   kf_theta_m = kf_theta_m+err_X(4);
    kf_phi_m_t(k) = kf_phi_m;       kf_theta_m_t(k) = kf_theta_m;
    kf_phi_t(k) = kf_phi;           kf_theta_t(k) = kf_theta;

    gyro_phi_t(k) = gyro_phi;       gyro_theta_t(k) = gyro_theta;
    
    acc_n(:,k) = acc_b(:,k)-norm(b_acc)*[sin(kf_theta);-cos(kf_theta)*sin(kf_phi);-cos(kf_theta)*cos(kf_phi)];
end

picnum = 1;
figure(picnum);hold off;plot(kf_phi_m_t);hold on;plot(kf_phi_t,'m');plot(gyro_phi_t,'r');title('phi compare')
picnum=picnum+1;figure(picnum);hold off;plot(kf_theta_m_t);hold on;plot(kf_theta_t,'m');plot(gyro_theta_t,'r');title('theta compare')
picnum=picnum+1;figure(picnum);hold off;plot(acc_n(1,:));hold on;plot(acc_n(2,:),'r');plot(acc_n(3,:),'g');title('acc n')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t(1,:));hold on;plot(acc_b_t(2,:),'r');plot(acc_b_t(3,:),'g');title('acc b')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b(1,:));hold on;plot(acc_b(2,:),'r');plot(acc_b(3,:),'g');title('acc b bias compansated')




