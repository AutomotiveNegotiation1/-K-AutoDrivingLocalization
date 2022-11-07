clear all;
fname = 'rosbag\test_0818_v3.mat';
% fname = 'rosbag\test_b1_line_20220803_v1.mat';

load(fname,'Sens');
SensNormal = Sens;
IMUacc = SensNormal.imu.acc;
IMUgyro = SensNormal.imu.gyro;
IMUtime = SensNormal.imu.time;
%
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


gravity = 9.8080;

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

b_gyro = mean(IMUgyro(1:100,:));
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
% b_acc_o =[ -0.2921+0.5-0.04    0.1928+0.1+0.05   -0.019-0.048];
b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original
% b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original

b_acc = [ -0.2921    0.1928   -0.019];  %% original
k = 1;
Acc_S = [0 0 0]';
Gyro_S = [0 0 0]';
dT = 0.1;
dt = 0.01;
P = 1;
% Ori = FUSE(IMUacc-b_acc_o,IMUgyro-b_gyro);
LPF = LPfilter;
AF = filter(LPF,IMUacc-b_acc_o);
LPFG1 = LPfilter;
LPFG2 = LPfilter;

% GF = filter(LPFG,IMUgyro-b_gyro);
FUSE = imufilter('SampleRate',100,'ReferenceFrame','ENU');
% Orientation = FUSE(IMUacc,IMUgyro);
Ori = zeros(1,1,'quaternion');

% Ori = FUSE(IMUacc-b_acc_o,IMUgyro-b_gyro);
% BBB = 
for i = 1 : Sens.length
    if i>1
        dt = IMUtime(i)-IMUtime(i-1);
    else
        dt = 0.01;
    end
    if Sens.flag(i,4) > 0
        [UWB] = func_extract_uwb(Sens, i, fname);
        %         fprintf("\t\t\t\t\tUWB : %f\n", UWB.time);
    else
        UWB.flag = false;
    end
    p = (IMUgyro(i,1)-b_gyro(1));    q = (IMUgyro(i,2)-b_gyro(2));    r = (IMUgyro(i,3)-b_gyro(3));
    acc_b(:,i) = (IMUacc(i,:)-b_acc_o)';
    %     Acc_S = Acc_S*(P-1)/P + IMUacc(i,:)'/P;
    Acc_S = Acc_S*(P-1)/P + acc_b(:,i)/P;
    Gyro_S = Gyro_S*(P-1)/P + IMUgyro(i,:)'/P;
    
    %%%% Phi, Theta, Psi %%%%%%%%%%%%%%%%%%%%%
    gyro_phi_prev = gyro_phi;
    gyro_theta_prev = gyro_theta;
    gyro_psi_prev = gyro_psi;
    kf_phi_prev = kf_phi;
    kf_theta_prev = kf_theta;
    kf_psi_prev = kf_psi;
    Ori(i) = FUSE(acc_b(:,i)',[p q r]);
    ML = eulerd(Ori(i),'ZYX','frame');
    ML_phi = ML(1)*pi/180;
    ML_theta = ML(2)*pi/180;
    ML_psi = ML(3)*pi/180;
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if i>50
        
        
        used_phi = gyro_phi;
        used_theta = gyro_theta;
        used_psi = gyro_psi;
        used_phi_next = gyro_phi;
        used_theta_next = gyro_theta;
        used_psi_next = gyro_psi;
        
        vel_t = rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,i-1);
%         vel_t = Acc_Vel(:,i-1);
        Acc_PQR(:,i) = 1*([0 vel_t(3) -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]);
%         Acc_PQR(:,i) = [0 0 0]';
        TransMat = GetGyroUpdateMatrix(used_phi, used_theta);
        Acc_delEuler(:,i) = gravity*[0 cos(used_theta);-cos(used_theta)*cos(used_phi) sin(used_theta)*sin(used_phi);cos(used_theta)*sin(used_phi) sin(used_theta)*cos(used_phi)]*TransMat(1:2,:)*[p;q;r]*dt;
        Acc_delEuler(:,i) = [0 0 0]';
%         Acc_PQR(:,i) = rotationVectorToMatrix([used_phi;used_theta;used_psi])*([0 vel_t(3) -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]);
        Acc_Steady(:,i) = gravity*[sin(used_theta_next);-cos(used_theta_next)*sin(used_phi_next);-cos(used_theta_next)*cos(used_phi_next)];
%         Acc_Acc(:,i) = rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b(:,i)-Acc_Steady(:,i));
        Acc_Acc(:,i) = rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b(:,i)-Acc_PQR(:,i)-Acc_Steady(:,i)-Acc_delEuler(:,i));
        
        acc_b_new(:,i) = acc_b(:,i) - rotationVectorToMatrix([used_phi;used_theta;used_psi])*(Acc_Acc(:,i)+Acc_PQR(:,i));
%         gravity = gravity*0.99+norm(acc_b_new(:,i)')*0.01;
%         Acc_Acc(:,i) = Acc_Acc(:,i)-rotationVectorToMatrix([used_phi;used_theta;used_psi])'*[2.5e-4;1e-4;6.5e-5]/dt;
        [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
%         [acc_b_phi,acc_b_theta] = EulerAcc(acc_b_new(1,i), acc_b_new(2,i), acc_b_new(3,i));
        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1,i), acc_b(2,i), acc_b(3,i));
        acc_b_phi_filtered = filter(LPFG1,acc_b_phi);
        acc_b_theta_filtered = filter(LPFG2,acc_b_theta);
        
        qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, 0);
        qua_acc_filtered = EulerToQuaternion(acc_b_phi_filtered, acc_b_theta_filtered, 0);
        qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);
        AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
        
%         [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc_filtered);
        [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
        
        Acc_Vel(:,i) = Acc_Vel(:,i-1) + Acc_Acc(:,i)*dt;
        Acc_Pos(:,i) = Acc_Pos(:,i-1) + Acc_Vel(:,i-1)*dt + Acc_Acc(:,i)*dt^2/2;
    else
        Acc_Acc(:,i) = [0 0 0]';
        Acc_Vel(:,i) = [0 0 0]';
        Acc_Pos(:,i) = [0 0 0]';
        Acc_Vel(:,i) = [0 0 0]';
        Acc_Pos(:,i) = [0 0 0]';
        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1,i), acc_b(2,i), acc_b(3,i));
%         acc_b_phi_filtered = filter(LPFG1,acc_b_phi);
%         acc_b_theta_filtered = filter(LPFG2,acc_b_theta);
        gyro_phi = gyro_phi*(i-1)/i+acc_b_phi/i;
        kf_phi = gyro_phi;
        gyro_theta = gyro_theta*(i-1)/i+acc_b_theta/i;
        kf_theta = gyro_theta;
%         gyro_phi = gyro_phi*(i-1)/i+acc_b_phi_filtered/i;
%         gyro_theta = gyro_theta*(i-1)/i+acc_b_theta_filtered/i;
        gyro_psi = 0;
    end
    
    
    
    
    if (UWB.flag)  % INS/UWB
        if UWB.num >= 3
            [UWB] = uwb_LS_positioning_2(UWB);
            UWB_LS_Pos(:,k) = UWB.pos;
            
        else
            UWB_LS_Pos(:,k) = UWB_LS_Pos(:,k-1);
            
        end
        Lv = 10;
        
        if (k > Lv*2)
            Ta = mean(UWB_LS_Pos(:,k-Lv+1:k)')';
            Tb = mean(UWB_LS_Pos(:,k-2*Lv+1:k-Lv)')';
            Tm = (Ta+Tb)/2;
            Td = Ta-Tm;
            UWB_M_Pos(:,k) = (Tm+Td);
        else
            UWB_M_Pos(:,k) = [0 0 0]';
        end
        if k > 100
            %             UWB_M_Vel(:,k) = (UWB_LS_Pos(:,k)-UWB_LS_Pos(:,k-1))/dT;
            UWB_M_Vel(:,k) = (UWB_M_Pos(:,k)-UWB_M_Pos(:,k-1))/dT;
            TEMP_UWB_Acc(:,k) = (UWB_M_Vel(:,k)-UWB_M_Vel(:,k-1))/dT;
            %             vel_t = UWB_M_Vel(:,k);
            vel_t = TEMP_Acc_Vel(:,k-1);
            TEMP_Acc_A(:,k) = rotationVectorToMatrix([gyro_phi;gyro_theta;gyro_psi])'*(Acc_S- rotationVectorToMatrix([gyro_phi;gyro_theta;gyro_psi])*([0 vel_t(3) -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]));
            %             TEMP_Acc_A(:,k) = rotationVectorToMatrix([gyro_phi;gyro_theta;gyro_psi])'*Acc_S;
            %             TEMP_Acc_A(:,k) = rotationVectorToMatrix([kf_phi;kf_theta;kf_psi])'*Acc_S;
            %             [acc_b_phi,acc_b_theta] = EulerAcc(TEMP_Acc(1), TEMP_Acc(2), TEMP_Acc(3));
            
            TEMP_Acc(:,k) = TEMP_Acc_A(:,k) - gravity*[0;0;-1];
            TEMP_Acc_Vel(:,k) = TEMP_Acc_Vel(:,k-1) + TEMP_Acc(:,k)*dT;
            TEMP_Acc_Pos(:,k) = TEMP_Acc_Pos(:,k-1) + TEMP_Acc_Vel(:,k-1)*dT + TEMP_Acc(:,k)*dT^2/2;
            %             TEMP_Acc(:,k) = TEMP_Acc_A(:,k) - gravity*[0;0;-1]-rotationVectorToMatrix([0;0;gyro_psi])'*([0 vel_t(3) -vel_t(2);-vel_t(3) 0 vel_t(1);vel_t(2) -vel_t(1) 0]*[p;q;r]);
        else
            
            UWB_M_Vel(:,k) = [0 0 0]';
            TEMP_UWB_Acc(:,k) = [0 0 0]';
            TEMP_Acc_A(:,k) = [0 0 0]';
            TEMP_Acc(:,k) = [0 0 0]';
            TEMP_Acc_Vel(:,k) = [0 0 0]';
            TEMP_Acc_Pos(:,k) = [0 0 0]';
            %
        end
        P = 1;
        k = k + 1;
        
        
        %         [Nav, Ani] = navi_uwb_update(Nav, UWB, Ani);
        UWB.flag = false;
        Acc_S = [0 0 0]';
    end
    
    Conv = 1;
    p_t(i) = p;           q_t(i) = q;       r_t(i) = r;
    kf_phi_t(i) = kf_phi*Conv;           kf_theta_t(i) = kf_theta*Conv;       kf_psi_t(i) = kf_psi*Conv;
    gyro_phi_t(i) = gyro_phi*Conv;      gyro_theta_t(i) = gyro_theta*Conv;    gyro_psi_t(i) = gyro_psi*Conv;
    acc_b_phi_t(i) = acc_b_phi*Conv;      acc_b_theta_t(i) = acc_b_theta*Conv;    acc_b_psi_t(i) = 0*Conv;
    ML_phi_t(i) = ML_phi*Conv;      ML_theta_t(i) = ML_theta*Conv;    ML_psi_t(i) = 0*Conv;
%     acc_b_phi_filtered_t(i) = acc_b_phi_filtered*Conv;      acc_b_theta_filtered_t(i) = acc_b_theta_filtered*Conv;    acc_b_psi_filtered_t(i) = 0*Conv;

end

picnum = 0;
%  picnum=picnum+1;figure(picnum);hold off;plot(kf_phi_m_t);hold on;plot(kf_phi_t,'m');plot(gyro_phi_t,'r');title('phi compare')
picnum=picnum+1;figure(picnum);hold off;plot(gyro_phi_t);hold on;plot(gyro_theta_t,'r');plot(gyro_psi_t,'g');title('gyro euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(kf_phi_t);hold on;plot(kf_theta_t,'r');plot(kf_psi_t,'g');title('kf euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_phi_t);hold on;plot(acc_b_theta_t,'r');plot(acc_b_psi_t,'r');title('acc euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(ML_phi_t);hold on;plot(ML_theta_t,'r');plot(ML_psi_t,'r');title('ML euler');legend('phi','theta','psi')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_b_phi_filtered_t);hold on;plot(acc_b_theta_filtered_t,'r');title('acc euler');legend('phi','theta')

% picnum=picnum+1;figure(picnum);hold off;plot(vel(1,:));hold on;plot(vel(2,:),'r');plot(vel(3,:),'g');title('velocity')
% picnum=picnum+1;figure(picnum);hold off;plot(pos(1,:));hold on;plot(pos(2,:),'r');plot(pos(3,:),'g');title('position')
%
% picnum=picnum+1;figure(picnum);hold off;plot(acc_n_m(1,:));hold on;plot(acc_n_m(2,:),'r');plot(acc_n_m(3,:),'g');title('acc n moving')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_b_m(1,:));hold on;plot(acc_b_m(2,:),'r');plot(acc_b_m(3,:),'g');title('acc b moving')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_b_s(1,:));hold on;plot(acc_b_s(2,:),'r');plot(acc_b_s(3,:),'g');title('acc b wo pqr moving')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t(1,:));hold on;plot(acc_b_t(2,:),'r');plot(acc_b_t(3,:),'g');title('acc b wo moving')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b(1,:));hold on;plot(acc_b(2,:),'r');plot(acc_b(3,:),'g');title('acc b')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_new(1,:));hold on;plot(acc_b_new(2,:),'r');plot(acc_b_new(3,:),'g');title('acc b new')

%
% picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t_b(1,:));hold on;plot(acc_b_t_b(2,:),'r');plot(acc_b_t_b(3,:),'g');title('acc b wo moving')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_b_filtered(1,:));hold on;plot(acc_b_filtered(2,:),'r');plot(acc_b_filtered(3,:),'g');title('acc b filtered')
% picnum=picnum+1;figure(picnum);hold off;plot(IMUgyro(:,1)-b_gyro(1));hold on;plot(IMUgyro(:,2)-b_gyro(2),'r');plot(IMUgyro(:,3)-b_gyro(3),'g');title('gyro original - bias')
% picnum=picnum+1;figure(picnum);hold off;plot(GF(:,1));hold on;plot(GF(:,2),'r');plot(GF(:,3),'g');title('gyro original - bias')
% figure(9876);hold off;plot(PQRm_1(1,:));hold on;plot(PQRm_1(2,:),'r');plot(PQRm_1(3,:),'g')
% figure(9877);hold off;plot(PQRm_2(1,:));hold on;plot(PQRm_2(2,:),'r');plot(PQRm_2(3,:),'g')
% figure(9878);hold off;plot(PQRm_3(1,:));hold on;plot(PQRm_3(2,:),'r');plot(PQRm_3(3,:),'g')

figure(1239);hold off;plot(p_t,'b');hold on;plot(q_t,'r');plot(r_t,'g');title('p q r')

% figure(1232);hold off;plot((UWB_M_Vel(2,:)),'r')
% figure(2238);hold off;plot(cumsum(TEMP_Acc(1,:)));
% figure(1234);hold off;plot((TEMP_UWB_Acc(2,:)),'r')
% 
% figure(123);hold off;plot((TEMP_Acc_Pos(1,:)),'b');hold on;plot((TEMP_Acc_Pos(2,:)),'r');plot((TEMP_Acc_Pos(3,:)),'g');title('Pos')
% figure(124);hold off;plot((TEMP_Acc_Vel(1,:)),'b');hold on;plot((TEMP_Acc_Vel(2,:)),'r');plot((TEMP_Acc_Vel(3,:)),'g');title('Vel')
% figure(125);hold off;plot((TEMP_Acc(1,:)));hold on;plot((TEMP_Acc(2,:)),'r');plot((TEMP_Acc(3,:)),'g');title('Acc')

figure(126);hold off;plot((Acc_Pos(1,:)),'b');hold on;plot((Acc_Pos(2,:)),'r');plot((Acc_Pos(3,:)),'g');title('Pos 0.01');grid on
figure(127);hold off;plot((Acc_Vel(1,:)),'b');hold on;plot((Acc_Vel(2,:)),'r');plot((Acc_Vel(3,:)),'g');title('Vel 0.01');grid on
figure(128);hold off;plot((Acc_Acc(1,:)));hold on;plot((Acc_Acc(2,:)),'r');plot((Acc_Acc(3,:)),'g');title('Acc 0.01');grid on
figure(1128);hold off;plot((Acc_Acc(1,:)));

figure(129);hold off;plot((Acc_PQR(1,:)));hold on;plot((Acc_PQR(2,:)),'r');plot((Acc_PQR(3,:)),'g');title('Acc p q r velocity 0.01');grid on
figure(130);hold off;plot((Acc_Steady(1,:)));hold on;plot((Acc_Steady(2,:)),'r');plot((Acc_Steady(3,:)),'g');title('Acc Steady 0.01');grid on
figure(140);plot(sqrt(acc_b(1,:).^2+acc_b(2,:).^2+acc_b(3,:).^2))
figure(141);plot(sqrt(Acc_Acc(1,:).^2+Acc_Acc(2,:).^2+Acc_Acc(3,:).^2))

% figure(1235);hold off;plot(cumsum(TEMP_UWB_Acc(2,:)),'r');
% figure(2239);hold off;plot(cumsum(TEMP_Acc(1,:)));hold on;plot(cumsum(TEMP_Acc(2,:)),'r');plot(cumsum(TEMP_Acc(3,:)),'g');


% figure(2240);hold off;plot(cumsum(PQRm_1(1,:)));hold on;plot(cumsum(PQRm_1(2,:)),'r');plot(cumsum(PQRm_1(3,:)),'g');
% figure(2241);hold off;plot(cumsum(PQRm_2(1,:)));hold on;plot(cumsum(PQRm_2(2,:)),'r');plot(cumsum(PQRm_2(3,:)),'g');
% figure(2242);hold off;plot(cumsum(PQRm_3(1,:)));hold on;plot(cumsum(PQRm_3(2,:)),'r');plot(cumsum(PQRm_3(3,:)),'g');

% 
% A=mean(IMUacc(1:2000,:))'
% B=mean(IMUacc(3200:4600,:))'
% C=mean(IMUacc(5300:6100,:))'
% D=mean(IMUacc(7200:8400,:))'
% E=mean(IMUacc(9200:10800,:))'
% F=mean(IMUacc(12000:13600,:))'
% 
% axo_cand = (C(1)+D(1))/2
% ayo_cand = (E(2)+F(2))/2
% azo_cand = (A(3)+B(3))/2
% 
% A_wo_b=A-[axo_cand;ayo_cand;azo_cand];
% B_wo_b=B-[axo_cand;ayo_cand;azo_cand];
% C_wo_b=C-[axo_cand;ayo_cand;azo_cand];
% D_wo_b=D-[axo_cand;ayo_cand;azo_cand];
% E_wo_b=E-[axo_cand;ayo_cand;azo_cand];
% F_wo_b=F-[axo_cand;ayo_cand;azo_cand];
% 
% abs_A = norm(A)^2
% abs_B = norm(B)^2
% abs_C = norm(C)^2
% abs_D = norm(D)^2
% abs_E = norm(E)^2
% abs_F = norm(F)^2
% 
% abs_A_wo_b = norm(A_wo_b)^2
% abs_B_wo_b = norm(B_wo_b)^2
% abs_C_wo_b = norm(C_wo_b)^2
% abs_D_wo_b = norm(D_wo_b)^2
% abs_E_wo_b = norm(E_wo_b)^2
% abs_F_wo_b = norm(F_wo_b)^2
% 
% (abs_D-abs_C)/4/axo_cand  % 2gAxo
% (abs_E-abs_F)/4/ayo_cand  % 2gAyo
% (abs_B-abs_A)/4/azo_cand  % 2gAzo
% 
% 
% figure(236);plot(sqrt(IMUacc(:,1).^2+IMUacc(:,2).^2+IMUacc(:,3).^2))
% figure(237);plot(sqrt((IMUacc(:,1)-axo_cand).^2+(IMUacc(:,2)-ayo_cand).^2+(IMUacc(:,3)-azo_cand).^2))
