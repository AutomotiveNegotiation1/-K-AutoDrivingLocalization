clear all;
fname = 'rosbag\test_0818_v1.mat';
load(fname,'Sens');
SensNormal = Sens;
IMUacc = SensNormal.imu.acc;
IMUgyro = SensNormal.imu.gyro;
IMUtime = SensNormal.imu.time;

% fname = 'rosbag\test_b1_line_20220803_v1.mat';
% load(fname,'Sens');
% SensNormal = Sens;
% IMUacc = SensNormal.imu.acc;
% IMUgyro = SensNormal.imu.gyro;
% IMUtime = SensNormal.imu.time;

% fname = 'rosbag\test_o_line_20220729_v1.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccU = SensUpsideDown.imu.acc;
% IMUgyroU = SensUpsideDown.imu.acc;
% FUSEU = imufilter('SampleRate',100,'ReferenceFrame','NED');
% QQU = FUSEU(IMUaccU,IMUgyroU);
% figure(9999);plot(eulerd(QQU,'ZYX','frame'));title('Up')
% %
% fname = 'rosbag\test_o_line_20220729_v2.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccR = SensUpsideDown.imu.acc;
% IMUgyroR = SensUpsideDown.imu.acc;
% FUSER = imufilter('SampleRate',100,'ReferenceFrame','NED');
% QQR = FUSER(IMUaccR,IMUgyroR);
% figure(9998);plot(eulerd(QQR,'ZYX','frame'));title('Right')
% 
% %
% fname = 'rosbag\test_o_line_20220729_v3.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccD = SensUpsideDown.imu.acc;
% IMUgyroD = SensUpsideDown.imu.acc;
% FUSED = imufilter('SampleRate',100,'ReferenceFrame','NED');
% QQD = FUSED(IMUaccD,IMUgyroD);
% figure(9997);plot(eulerd(QQD,'ZYX','frame'));title('Down')
% %
% fname = 'rosbag\test_o_line_20220729_v4.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccL = SensUpsideDown.imu.acc;
% IMUgyroL = SensUpsideDown.imu.acc;
% FUSEL = imufilter('SampleRate',100,'ReferenceFrame','NED');
% QQL = FUSEL(IMUaccL,IMUgyroL);
% figure(9996);plot(eulerd(QQL,'ZYX','frame'));title('Left')


gravity = 9.85;

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

FUSE = imufilter('SampleRate',100,'ReferenceFrame','ENU');
% Orientation = FUSE(IMUacc,IMUgyro);
Ori = zeros(1,1,'quaternion');
% b_acc_o =[ 0.0315    0.1288   -0.0798];
% b_acc_o =[ -0.2921    0.1928   -0.019];
b_acc_o =[ -0.2921-0.0531-0.0284-0.0147-0.0075    0.1928+0.0144+0.0019+9e-4+4.7e-4   -0.019+0.0017+5.87e-5-4.5e-5-2.35e-5];
% b_acc_o =[ -0.3470    0.1028   -0.07];

for k = 1 : Sens.length
    if k>1
        dt = IMUtime(k)-IMUtime(k-1);
    else
        dt = 0.01;
    end
    
    p = (IMUgyro(k,1)-b_gyro(1));    q = (IMUgyro(k,2)-b_gyro(2));    r = (IMUgyro(k,3)-b_gyro(3));
    acc_b(:,k) = IMUacc(k,:)'-b_acc_o';
    Ori(k) = FUSE(acc_b(:,k)',[p q r]);
    
    if k > Lav
        dTheta = kf_theta_t(k-1)-kf_theta_t(k-Lav);
        dPhi = kf_phi_t(k-1)-kf_phi_t(k-Lav);
        vel_t = [1 0 0]';
        PQRm_1(:,k) = [0 vel_t(3) -vel_t(2);-vel_t(3) 0 vel_t(1);vel_t(2) -vel_t(1) 0]*[p;q;r];
        vel_t = [0 1 0]';
        PQRm_2(:,k) = [0 vel_t(3) -vel_t(2);-vel_t(3) 0 vel_t(1);vel_t(2) -vel_t(1) 0]*[p;q;r];
        vel_t = [0 0 1]';
        PQRm_3(:,k) = [0 vel_t(3) -vel_t(2);-vel_t(3) 0 vel_t(1);vel_t(2) -vel_t(1) 0]*[p;q;r];
        
        acc_b_t(:,k) = acc_b(:,k)-[0 vel(3,k) -vel(2,k);-vel(3,k) 0 vel(1,k);vel(2,k) -vel(1,k) 0]*[p;q;r]*0;
%         acc_b_m(:,k) = acc_b_t(:,k)-mean(acc_b_t(:,k-Lav:k-1),2)+mean(acc_b_m(:,k-Lav:k-1),2)-gravity*[dTheta*cos(kf_theta_t(k-Lav));-dPhi*cos(kf_theta_t(k-Lav))*cos(kf_phi_t(k-Lav))+dTheta*sin(kf_theta_t(k-Lav))*sin(kf_phi_t(k-Lav));dPhi*cos(kf_theta_t(k-Lav))*sin(kf_phi_t(k-Lav))+dTheta*sin(kf_theta_t(k-Lav))*cos(kf_phi_t(k-Lav))];
        acc_b_m(:,k) = acc_b_t(:,k)-gravity*[sin(gyro_theta);-cos(gyro_theta)*sin(gyro_phi);-cos(gyro_theta)*cos(gyro_phi)]; 
        acc_b_s(:,k) = acc_b_t(:,k)-acc_b_m(:,k);
        acc_b_t_b(:,k) = acc_b_s(:,k)-gravity*[sin(gyro_theta);-cos(gyro_theta)*sin(gyro_phi);-cos(gyro_theta)*cos(gyro_phi)];
        acc_n_m(:,k) = Cb2n*(acc_b_m(:,k));
    else
        acc_b_t(:,k) = acc_b(:,k);
        acc_b_m(:,k) = [0 0 0]';
        acc_b_s(:,k) = acc_b_t(:,k)-acc_b_m(:,k);
        acc_n_m(:,k) = Cb2n*(acc_b_m(:,k));
        acc_b_t_b(:,k) = acc_b_s(:,k)-gravity*[sin(gyro_theta);-cos(gyro_theta)*sin(gyro_phi);-cos(gyro_theta)*cos(gyro_phi)];
    end
    [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
%     [acc_b_phi,acc_b_theta] = EulerAcc_2(acc_b_t(1,k), acc_b_t(2,k), acc_b_t(3,k), gravity);
        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1,k), acc_b(2,k), acc_b(3,k));
    
    qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, 0);
    qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);
    AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    
    [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
%     TEmp = eulerd(Ori,'ZYX','frame');
%     kf_phi = TEmp(1);
%     kf_theta = TEmp(2);
%     kf_psi = TEmp(3);
    
    %     acc_n(:,k) = acc_b(:,k)-gravity*[sin(gyro_theta);-cos(gyro_theta)*sin(gyro_phi);-cos(gyro_theta)*cos(gyro_phi)];
    
    if (k < 1)
        b_acc_n = b_acc_n*(k-1)/k + acc_b_m(:,k)/k;
        vel(:,k+1) = 0;
        pos(:,k+1) = 0;
        Cb2n = eye(3);
%         acc_b_m(:,k) = [0 0 0]';
    else
        b_acc = b_acc_n;
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
picnum=picnum+1;figure(picnum);hold off;plot(gyro_phi_t);hold on;plot(gyro_theta_t,'r');plot(gyro_psi_t,'g');title('gyro euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(kf_phi_t);hold on;plot(kf_theta_t,'r');plot(kf_psi_t,'g');title('kf euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_phi_t);hold on;plot(acc_b_theta_t,'r');plot(acc_b_psi_t,'r');title('acc euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(vel(1,:));hold on;plot(vel(2,:),'r');plot(vel(3,:),'g');title('velocity')
picnum=picnum+1;figure(picnum);hold off;plot(pos(1,:));hold on;plot(pos(2,:),'r');plot(pos(3,:),'g');title('position')

picnum=picnum+1;figure(picnum);hold off;plot(acc_n_m(1,:));hold on;plot(acc_n_m(2,:),'r');plot(acc_n_m(3,:),'g');title('acc n moving')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_m(1,:));hold on;plot(acc_b_m(2,:),'r');plot(acc_b_m(3,:),'g');title('acc b moving')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_s(1,:));hold on;plot(acc_b_s(2,:),'r');plot(acc_b_s(3,:),'g');title('acc b wo pqr moving')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t(1,:));hold on;plot(acc_b_t(2,:),'r');plot(acc_b_t(3,:),'g');title('acc b wo moving')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b(1,:));hold on;plot(acc_b(2,:),'r');plot(acc_b(3,:),'g');title('acc b')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t_b(1,:));hold on;plot(acc_b_t_b(2,:),'r');plot(acc_b_t_b(3,:),'g');title('acc b wo moving')

picnum=picnum+1;figure(picnum);hold off;plot(IMUgyro(:,1)-b_gyro(1));hold on;plot(IMUgyro(:,2)-b_gyro(2),'r');title('gyro original - bias')
figure(9876);hold off;plot(PQRm_1(1,:));hold on;plot(PQRm_1(2,:),'r');plot(PQRm_1(3,:),'g')
figure(9877);hold off;plot(PQRm_2(1,:));hold on;plot(PQRm_2(2,:),'r');plot(PQRm_2(3,:),'g')
figure(9878);hold off;plot(PQRm_3(1,:));hold on;plot(PQRm_3(2,:),'r');plot(PQRm_3(3,:),'g')


A=mean(IMUacc(1:2000,:))'
B=mean(IMUacc(3200:4600,:))'
C=mean(IMUacc(5300:6100,:))'
D=mean(IMUacc(7200:8400,:))'
E=mean(IMUacc(9200:10800,:))'
F=mean(IMUacc(12000:13600,:))'

axo_cand = (C(1)+D(1))/2
ayo_cand = (E(2)+F(2))/2
azo_cand = (A(3)+B(3))/2

A_wo_b=A-[axo_cand;ayo_cand;azo_cand];
B_wo_b=B-[axo_cand;ayo_cand;azo_cand];
C_wo_b=C-[axo_cand;ayo_cand;azo_cand];
D_wo_b=D-[axo_cand;ayo_cand;azo_cand];
E_wo_b=E-[axo_cand;ayo_cand;azo_cand];
F_wo_b=F-[axo_cand;ayo_cand;azo_cand];

abs_A = norm(A)^2
abs_B = norm(B)^2
abs_C = norm(C)^2
abs_D = norm(D)^2
abs_E = norm(E)^2
abs_F = norm(F)^2

abs_A_wo_b = norm(A_wo_b)^2
abs_B_wo_b = norm(B_wo_b)^2
abs_C_wo_b = norm(C_wo_b)^2
abs_D_wo_b = norm(D_wo_b)^2
abs_E_wo_b = norm(E_wo_b)^2
abs_F_wo_b = norm(F_wo_b)^2

(abs_D-abs_C)/4/axo_cand  % 2gAxo
(abs_E-abs_F)/4/ayo_cand  % 2gAyo
(abs_B-abs_A)/4/azo_cand  % 2gAzo


figure(236);plot(sqrt(IMUacc(:,1).^2+IMUacc(:,2).^2+IMUacc(:,3).^2))
figure(237);plot(sqrt((IMUacc(:,1)-axo_cand).^2+(IMUacc(:,2)-ayo_cand).^2+(IMUacc(:,3)-azo_cand).^2))


