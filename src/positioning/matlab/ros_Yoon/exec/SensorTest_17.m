clear all;
fname = 'rosbag\test_b1_line_20220803_v1.mat';
load(fname,'Sens');
SensNormal = Sens;
IMUacc = SensNormal.imu.acc;
IMUgyro = SensNormal.imu.gyro;
IMUtime = SensNormal.imu.time;
UWM = SensNormal.uwb;


%
% fname = 'rosbag\test_o_line_20220729_v1.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccU = SensUpsideDown.imu.acc;
%
% fname = 'rosbag\test_o_line_20220729_v2.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccR = SensUpsideDown.imu.acc;
%
% fname = 'rosbag\test_o_line_20220729_v3.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccD = SensUpsideDown.imu.acc;
%
% fname = 'rosbag\test_o_line_20220729_v4.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccL = SensUpsideDown.imu.acc;
% IMUacc(:,2) = -IMUacc(:,2);


gravity = 9.7964;

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
b_acc = [-0.0002 0.0019 -0.0694]';
b_acc_n = [0 0 0]';

b_gyro = mean(IMUgyro(1:1000,:));
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
for k = 1 : Sens.length
    if k>1
        dt = IMUtime(k)-IMUtime(k-1);
    else
        dt = 0.01;
    end
    
    p = (IMUgyro(k,1)-b_gyro(1));    q = (IMUgyro(k,2)-b_gyro(2));    r = (IMUgyro(k,3)-b_gyro(3));
    
    acc_b(:,k) = IMUacc(k,:)';
    
    if k > Lav
        dTheta = kf_theta_t(k-1)-kf_theta_t(k-Lav);
        dPhi = kf_phi_t(k-1)-kf_phi_t(k-Lav);
        acc_b_m(:,k) = acc_b(:,k)-mean(acc_b(:,k-Lav:k-1),2)+mean(acc_b_m(:,k-Lav:k-1),2)-gravity*[dTheta*cos(kf_theta_t(k-Lav));-dPhi*cos(kf_theta_t(k-Lav))*cos(kf_phi_t(k-Lav))+dTheta*sin(kf_theta_t(k-Lav))*sin(kf_phi_t(k-Lav));dPhi*cos(kf_theta_t(k-Lav))*sin(kf_phi_t(k-Lav))+dTheta*sin(kf_theta_t(k-Lav))*cos(kf_phi_t(k-Lav))];
        acc_b_t(:,k) = acc_b(:,k)-acc_b_m(:,k)-b_acc;
        acc_b_t_b(:,k) = acc_b_t(:,k)-gravity*[sin(kf_theta);-cos(kf_theta)*sin(kf_phi);-cos(kf_theta)*cos(kf_phi)];
        acc_n_m(:,k) = Cb2n*(acc_b_t_b(:,k));
    else
        acc_b_m(:,k) = [0 0 0]';
        acc_b_t(:,k) = acc_b(:,k)-acc_b_m(:,k)-b_acc;
        acc_b_t_b(:,k) = acc_b_t(:,k)-gravity*[sin(kf_theta);-cos(kf_theta)*sin(kf_phi);-cos(kf_theta)*cos(kf_phi)];
        acc_n_m(:,k) = Cb2n*(acc_b_t_b(:,k));
    end
    [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
%     [acc_b_phi,acc_b_theta] = EulerAcc_2(acc_b_t(1,k), acc_b_t(2,k), acc_b_t(3,k), gravity);
        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b_t(1,k), acc_b_t(2,k), acc_b_t(3,k));
    
    qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, 0);
    qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);
    AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    
    [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
    
    %     acc_n(:,k) = acc_b(:,k)-gravity*[sin(gyro_theta);-cos(gyro_theta)*sin(gyro_phi);-cos(gyro_theta)*cos(gyro_phi)];
    
    if (k < 1000)
        b_acc_n = b_acc_n*(k-1)/k + acc_b_t_b(:,k)/k;
        vel(:,k+1) = 0;
        pos(:,k+1) = 0;
        Cb2n = eye(3);
%         acc_b_m(:,k) = [0 0 0]';
    else
        if k == 1000
            b_acc = b_acc+b_acc_n;
        else
            b_acc = b_acc;
        end
        pos(:,k+1) = pos(:,k)+vel(:,k)*dt+1/2*dt^2*acc_n_m(:,k);
        vel(:,k+1) = vel(:,k)+acc_n_m(:,k)*dt;
        Cb2n = eulr2dcm([kf_phi;kf_theta;kf_psi])';
    end
    %     acc_b_euler(:,k) = [acc_b_phi;acc_b_theta];
    Conv = 1;
    kf_phi_t(k) = kf_phi*Conv;           kf_theta_t(k) = kf_theta*Conv;       kf_psi_t(k) = kf_psi*Conv;
    gyro_phi_t(k) = gyro_phi*Conv;      gyro_theta_t(k) = gyro_theta*Conv;    gyro_psi_t(k) = gyro_psi*Conv;
    acc_b_phi_t(k) = acc_b_phi*Conv;      acc_b_theta_t(k) = acc_b_theta*Conv;    acc_b_psi_t(k) = 0*Conv;
end

picnum = 0;
% figure(picnum);hold off;plot(kf_phi_m_t);hold on;plot(kf_phi_t,'m');plot(gyro_phi_t,'r');title('phi compare')
picnum=picnum+1;figure(picnum);hold off;plot(gyro_phi_t);hold on;plot(gyro_theta_t,'m');plot(gyro_psi_t,'r');title('gyro euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(kf_phi_t);hold on;plot(kf_theta_t,'m');plot(kf_psi_t,'r');title('kf euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_phi_t);hold on;plot(acc_b_theta_t,'m');plot(acc_b_psi_t,'r');title('acc euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(vel(1,:));hold on;plot(vel(2,:),'m');plot(vel(3,:),'r');title('velocity')
picnum=picnum+1;figure(picnum);hold off;plot(pos(1,:));hold on;plot(pos(2,:),'m');plot(pos(3,:),'r');title('position')

picnum=picnum+1;figure(picnum);hold off;plot(acc_n_m(1,:));hold on;plot(acc_n_m(2,:),'r');plot(acc_n_m(3,:),'g');title('acc n moving')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_m(1,:));hold on;plot(acc_b_m(2,:),'r');plot(acc_b_m(3,:),'g');title('acc b moving')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t_b(1,:));hold on;plot(acc_b_t_b(2,:),'r');plot(acc_b_t_b(3,:),'g');title('acc b wo moving')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t(1,:));hold on;plot(acc_b_t(2,:),'r');plot(acc_b_t(3,:),'g');title('acc b wo moving')

picnum=picnum+1;figure(picnum);hold off;plot(IMUgyro(:,1)-b_gyro(1));hold on;plot(IMUgyro(:,2)-b_gyro(2),'r');title('gyro original - bias')







