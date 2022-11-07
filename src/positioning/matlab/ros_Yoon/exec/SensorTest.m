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
b_acc = mean(IMUacc(1:100,:));

[acc_phi_bias,acc_theta_bias] = EulerAcc(b_acc(1), b_acc(2), b_acc(3));
if(b_acc(3) > 9)
    acc_phi_bias = acc_phi_bias + pi;
end

Cacc_bias = qua2dcm(EulerToQuaternion(acc_phi_bias, acc_theta_bias, 0));
b_gyro = mean(IMUgyro(1:100,:));
pos = zeros(3,1); vel = zeros(3,1);
b_gyro_his = zeros(3,1);
l = 100;
m = 100;
acc_g_bias = [0 0 0]';
b_acc_b = [0 0 0]';
acc_b_kf_wob = [0 0 0]';
Cb2n_kf = eye(3);
for k = 1 : Sens.length

    
    p = (IMUgyro(k,1));    q = (IMUgyro(k,2));    r = (IMUgyro(k,3));
%     p = (IMUgyro(k,1)-b_gyro(1));    q = (IMUgyro(k,2)-b_gyro(2));    r = (IMUgyro(k,3)-b_gyro(3));
%     p = (IMUgyro(k,1)-b_gyro(1))*pi/180;    q = (IMUgyro(k,2)-b_gyro(2))*pi/180;    r = (IMUgyro(k,3)-b_gyro(3))*pi/180;
    acc_b = IMUacc(k,:)';
    acc_b_kf = Cb2n_kf'*acc_b;
%     Te = [0 -r q;r 0 -p;-q p 0];
    
%     acc_g(:,k) = Cacc_bias*acc_b-[0;0;-norm(b_acc)]-acc_g_bias;
    acc_g(:,k) = acc_b;
%     acc_g(:,k) = Cacc_bias*acc_b-[0;0;-norm(b_acc)]-Te*vel(:,k)-acc_g_bias;
%     acc_amp(k) = norm(acc_g(:,k));
%     gyro_amp(k) = norm([p;q;r]);
    
    if k>1
        dt = IMUtime(k)-IMUtime(k-1);
    else
        dt = 0.01;
    end

    
%     [acc_phi,acc_theta] = EulerAcc(acc_g(1,k), acc_g(2,k), acc_g(3,k)-norm(b_acc));
%     acc_euler(:,k) = [acc_phi;acc_theta];
    [acc_phi_b,acc_theta_b] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));
    acc_b_euler(:,k) = [acc_phi_b;acc_theta_b];
    qua_acc = EulerToQuaternion(acc_phi_b, acc_theta_b, 0);    %% current
    qua_gyro = EulerToQuaternion(gyro_phi, gyro_theta, gyro_psi);   %% previous
    qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);           %% previous
    
    AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    [kf_phi, kf_theta, kf_psi] = EulerKalman_2(qua_kf, AA, qua_acc);
    qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);
    Cb2n_kf = qua2dcm(qua_kf);
%     acc_b_kf = Cb2n_kf'*acc_b-[0;0;norm(b_acc)];
%     acc_b_kf = Cb2n_kf'*acc_b-b_acc_b;
%         acc_b_kf = Cb2n_kf'*acc_b-[0;0;norm(b_acc)]-b_acc_b;

    kf_euler(:,k) = dcm2eulr(Cb2n_kf);
    
    qua_gyro = AA*qua_gyro;
    
    if k < 1000
        b_acc_b = b_acc_b*(k-1)/k+acc_b_kf/k;
        pos(:,k+1) = [0 0 0]';
        vel(:,k+1) = [0 0 0]';
    else
        alpha = 0.01;
        b_acc_b = b_acc_b*(1-alpha)+acc_b_kf*alpha;
        acc_b_kf_wob = acc_b_kf - b_acc_b;
        pos(:,k+1) = pos(:,k)+vel(:,k)*dt+1/2*dt^2*acc_b_kf_wob;
        vel(:,k+1) = vel(:,k)+acc_b_kf_wob*dt;
    end
   
    
    gyro_euler(:,k) = dcm2eulr(qua2dcm(qua_gyro));
    gyro_phi = gyro_euler(1); gyro_theta = gyro_euler(2); gyro_psi = gyro_euler(3);
    b_gyro_his(:,k) = zeros(3,1)+b_gyro';
    acc_bias_his(:,k) = acc_b_kf_wob;
end

% figure(1);hold off;plot(phi_bias_his);hold on;plot(gyro_euler(1,:),'g');
% figure(2);hold off;plot(theta_bias_his);hold on;plot(gyro_euler(2,:),'g');

figure(1);hold off;plot(kf_euler(1,:)*180/pi);hold on;plot(gyro_euler(1,:)*180/pi,'g');title('kf euler vs gyro euler phi')
figure(2);hold off;plot(kf_euler(2,:)*180/pi);hold on;plot(gyro_euler(2,:)*180/pi,'g');title('kf euler vs gyro euler theta')
figure(21);hold off;plot(kf_euler(3,:)*180/pi);hold on;plot(gyro_euler(3,:)*180/pi,'g');title('kf euler vs gyro euler psi')

figure(3);hold off;plot(acc_b_euler(1,:));hold on;plot(acc_b_euler(2,:),'r');grid on;title('acc euler')
figure(4);hold off;plot(gyro_euler(1,:)*180/pi);hold on;plot(gyro_euler(2,:)*180/pi,'g');plot(gyro_euler(3,:)*180/pi,'r');grid on;title('gyro euler')
% figure(3);hold off;plot(acc_euler(3,:));hold on;plot(gyro_euler(3,:),'g');

figure(5);hold off;plot(acc_g(1,:));hold on;plot(acc_g(2,:),'r');plot(acc_g(3,:),'g');grid on;title('acc global')

figure(6);hold off;plot(pos(1,:));hold on;plot(pos(2,:),'r');plot(pos(3,:),'g'); title('position')
figure(7);hold off;plot(vel(1,:));hold on;plot(vel(2,:),'r');plot(vel(3,:),'g');title('velocity')
figure(16);hold off;plot(pos(1,:),pos(2,:),'.');grid on;axis equal

% figure(17);hold off;plot(vel_gyro(1,:));hold on;plot(vel_gyro(2,:),'g');plot(vel_gyro(3,:),'r');

% figure(8);plot(acc_amp); title('acc amp')
% figure(9);plot(gyro_amp); title('gyro amp')

figure(10);hold off;plot(b_gyro_his(1,:));hold on;plot(b_gyro_his(2,:),'r');plot(b_gyro_his(3,:),'g');title('gyro bias history')
figure(11);hold off;plot(acc_bias_his(1,:));hold on;plot(acc_bias_his(2,:),'r');plot(acc_bias_his(3,:),'g');title('acc new')



