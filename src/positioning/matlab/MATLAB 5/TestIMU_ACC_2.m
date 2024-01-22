clear all;

% aa = rosbag('2023-07-26-18-35-13.bag');
% aa = rosbag('2023-08-07-11-03-21.bag');
% aa = rosbag('2023-08-07-14-34-54.bag');
aa = rosbag('202308_publisher_queue_size_20.bag');

IMURoSV = select(aa,'Topic','/imu/angular_velocity');
IMUVMsg = readMessages(IMURoSV,'DataFormat','struct');

IMURoSA = select(aa,'Topic','/imu/acceleration');
IMUAMsg = readMessages(IMURoSA,'DataFormat','struct');

LnC = 0;
Lp = 4;
Ln = 6;

% xt_b = [-0.075 0.075 -0.075 0.075];
% yt_b = [0.12 0.12 -0.12 -0.12];

% xt_b = [-0.07 0.07 -0.07 0.07];
% yt_b = [0.135 0.135 -0.135 -0.135];

xt_b = [-0.525 0.525 -0.525 0.525];
yt_b = [0.505 0.505 -0.505 -0.505];
zt_b = 1.53;

% Center = [0.700 3.024]
% tag1 = [0.694 3.166]
% tag2 = [0.786 3.165]
% tag3 = [0.674 2.893]
% tag4 = [0.814 2.892]

tag_pos_b = xt_b + j*yt_b;

AnchorIDmapH = [];
AnchorIDmap = [];
xa = [];
ya = [];
za = [];
k0 = ones(1,5);
RxDistTot = zeros(1,5);

gravity = 9.8;

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

% b_gyro = mean(IMUgyro(1:100,:));
b_gyro = [0 0 0];
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

% b_acc_o =[ 0.0315    0.1288   -0.0798];
% b_acc_o =[ -0.2921+0.5-0.04    0.1928+0.1+0.05   -0.019-0.048];
b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original
b_acc_o = [-1 -1 0];
% b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original

% b_acc = [ -0.2921    0.1928   -0.019];  %% original
b_head = zeros(3,1);

i = 1;
Acc_S = [0 0 0]';
Gyro_S = [0 0 0]';
dT = 0.1;
dt = 0.01;
P = 1;

Lv = 10;

IMUacc = zeros(2*Lv, 3);
IMUgyro = zeros(2*Lv, 3);
Acc_Acc = zeros(3,2*Lv );
Acc_Vel = zeros(3,2*Lv);
Acc_Pos = zeros(3,2*Lv);
Acc_PQR = zeros(3,2*Lv);
Acc_Steady = zeros(3,2*Lv);
UWB_LS_Pos = zeros(3,2*Lv);
UWB_M_Pos = zeros(3,2*Lv);
UWB_M_Vel = zeros(3,2*Lv);
init_pos.x = 0;
init_pos.y = 0;
init_pos.z = 0;
kl = 1;
s_time_prev = zeros(1,2);
%for k = 1 : LenTot*3-300
% for k = 1 : 3435
k = 0;
while (length(IMUVMsg)>=k0(5))
    k = k + 1;

    s_time_a(1) = double(IMUVMsg{k0(5)}.Header.Stamp.Sec)+double(IMUVMsg{k0(5)}.Header.Stamp.Nsec)/10^9;
    s_time_a(2) = double(IMUAMsg{k0(5)}.Header.Stamp.Sec)+double(IMUAMsg{k0(5)}.Header.Stamp.Nsec)/10^9;

    dt = s_time_a(1) - s_time_prev(1);
    s_time_prev(1) = s_time_a(1);
    dt_acc = s_time_a(2) - s_time_prev(2);
    s_time_prev(2) = s_time_a(2);
%     dt = 0.01;
%     dt_acc = 0.01;

    dt_test(k) = dt;
    dt_acc_test(k) = dt_acc;
    
    dt = dt*1;
    dt_acc = dt_acc*1;
    IMUgyro(1:end-1, :) = IMUgyro(2:end, :);
    IMUgyro(end,:)  = [IMUVMsg{k0(5)}.Vector.X IMUVMsg{k0(5)}.Vector.Y IMUVMsg{k0(5)}.Vector.Z];
    IMUacc(1:end-1, :) = IMUacc(2:end, :);
    IMUacc(end,:) = [IMUAMsg{k0(5)}.Vector.X IMUAMsg{k0(5)}.Vector.Y IMUAMsg{k0(5)}.Vector.Z]*1;

    if k0(5)>800
%                     p = (IMUgyro(end,1)-b_gyro(1))*pi/180;    q = (IMUgyro(end,2)-b_gyro(2))*pi/180;    r = (IMUgyro(end,3)-b_gyro(3))*pi/180;
        p = (IMUgyro(end,1)-b_gyro(1));    q = (IMUgyro(end,2)-b_gyro(2));    r = (IMUgyro(end,3)-b_gyro(3));
        acc_b = (IMUacc(end,[1 2 3])-b_acc_o)';

        used_phi = kf_phi;
        used_theta = kf_theta;
        used_psi = kf_psi;
%         used_phi = 0;
%         used_theta = 0;
%         %             used_psi = headingest_a_aver_v;
%         used_psi = 0;


        used_phi_next = kf_phi;
        used_theta_next = kf_theta;
        used_psi_next = kf_psi;
        %             used_psi_next = headingest_a_aver_v;

        Acc_Pos(:,1:end-1) = Acc_Pos(:,2:end);
        Acc_Vel(:,1:end-1) = Acc_Vel(:,2:end);
        Acc_PQR(:,1:end-1) = Acc_PQR(:,2:end);
        Acc_Steady(:,1:end-1) = Acc_Steady(:,2:end);
        Acc_Acc(:,1:end-1) = Acc_Acc(:,2:end);

%         vel_t = rotationVectorToMatrix([0;0;used_psi])*Acc_Vel(:,end-1);
%         vel_t = rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,end-1);
%         Acc_PQR(:,end) = 1*([0 vel_t(3) -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]);
%         Acc_Steady(:,end) = gravity*[sin(used_theta_next);-cos(used_theta_next)*sin(used_phi_next);-cos(used_theta_next)*cos(used_phi_next)];
        Acc_Acc(:,end) = [0 1 0;1 0 0;0 0 1]*rotationVectorToMatrix([0;0;used_psi])'*([acc_b(1);acc_b(2);0]);
%          Acc_Acc(:,end) = rotationVectorToMatrix([used_phi;used_theta;used_psi])'*([acc_b(1);acc_b(2);acc_b(3)]-Acc_Steady(:,end));
%         Acc_Acc(:,end) = rotationVectorToMatrix([used_phi;used_theta;used_psi])'*([acc_b(1);acc_b(2);acc_b(3)]-Acc_PQR(:,end)-Acc_Steady(:,end));

        %             [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,0,0,gyro_psi);
        [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);

        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));

        qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, gyro_psi);
        qua_kf = EulerToQuaternion(gyro_phi,gyro_theta,gyro_psi);
        AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
        % %
        [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
        kf_phi =real(kf_phi);
        kf_theta =real(kf_theta);
        kf_psi =real(kf_psi);
        %             kf_phi = gyro_phi;
        %             kf_theta = gyro_theta;
        %             kf_psi = gyro_psi;

        %Acc_Vel(:,end) = Acc_Vel(:,end-1) + Acc_Acc(:,end)*dt;
        %             Acc_Pos(:,end) = Acc_Pos(:,end-1) + Acc_Vel(:,end-1)*dt + Acc_Acc(:,end)*dt^2/2;
        Acc_Vel(:,end) = Acc_Vel(:,end-1) + Acc_Acc(:,end)*dt_acc;
        Acc_Pos(:,end) = Acc_Pos(:,end-1) + Acc_Vel(:,end-1)*dt_acc + Acc_Acc(:,end)*dt_acc^2/2;

    elseif k0(5)== 800

        b_gyro = mean(IMUgyro);
        b_acc_o = (mean(IMUacc)'+[0;0;gravity])';
        p = (IMUgyro(end,1)-b_gyro(1));    q = (IMUgyro(end,2)-b_gyro(2));    r = (IMUgyro(end,3)-b_gyro(3));
%         p = (IMUgyro(end,1)-b_gyro(1))*pi/180;    q = (IMUgyro(end,2)-b_gyro(2))*pi/180;    r = (IMUgyro(end,3)-b_gyro(3))*pi/180;
        acc_b = (IMUacc(end,:)-b_acc_o)';

        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));
        gyro_phi = acc_b_phi;
        kf_phi = gyro_phi;
        gyro_theta = acc_b_theta;
        kf_theta = gyro_theta;
        gyro_psi = pi;
    else
        p = (IMUgyro(end,1));    q = (IMUgyro(end,2));    r = (IMUgyro(end,3));
%         p = (IMUgyro(end,1))*pi/180;    q = (IMUgyro(end,2))*pi/180;    r = (IMUgyro(end,3))*pi/180;
        acc_b = (IMUacc(end,:))';

        Acc_Vel(:,1:end-1) = Acc_Vel(:,2:end);
        Acc_Acc(:,1:end-1) = Acc_Acc(:,2:end);
        Acc_Pos(:,1:end-1) = Acc_Pos(:,2:end);

        Acc_Acc(:,end) = [0 0 0]';
        Acc_Vel(:,end) = [0 0 0]';
        Acc_Pos(:,end) = [init_pos.x init_pos.y init_pos.z]';

        [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));
        gyro_phi = gyro_phi*(k0(5)-1)/k0(5)+acc_b_phi/k0(5);
        kf_phi = gyro_phi;
        gyro_theta = gyro_theta*(k0(5)-1)/k0(5)+acc_b_theta/k0(5);
        kf_theta = gyro_theta;
        gyro_psi = 0;
        %             b_gyro = b_gyro*(k0(5)-1)/k0(5)+IMUgyro(end,:)/k0(5);
    end
    k0(5) = k0(5) + 1;

    Pos(:,k) = Acc_Pos(:,end);
    Vel(:,k) = Acc_Vel(:,end);

    gyro_phi_a(k) = gyro_phi;
    gyro_theta_a(k) = gyro_theta;
    gyro_psi_a(k) = gyro_psi;
    kf_phi_a(k) = kf_phi;
    kf_theta_a(k) = kf_theta;
    kf_psi_a(k) = kf_psi;
    IMUgyrot(k,:) = IMUgyro(end,:);
    IMUacct(k,:) = IMUacc(end,:);
    pt(k)=p;
    qt(k)=q;
    rt(k)=r;
    ACC_t(:,k) = Acc_Acc(:,end);
end

figure(231);hold off;plot(Vel(1,:));hold on;plot(Vel(2,:),'r')
figure(232);hold off;plot(Pos(1,:),Pos(2,:),'.');hold on;plot(xa,ya,'k*')

figure(233);hold off;plot(mod(kf_psi_a,2*pi));hold on;plot(mod(gyro_psi_a,2*pi),'r')
figure(234);hold off;plot(mod(kf_theta_a,2*pi));hold on;plot(mod(gyro_theta_a,2*pi),'r')
figure(235);hold off;plot(mod(kf_phi_a,2*pi));hold on;plot(mod(gyro_phi_a,2*pi),'r')

figure(300);plot(IMUacct)
figure(301);plot(IMUgyrot)

figure(400);plot(pt)
figure(401);plot(qt)
figure(402);plot(rt)

tet = 0;
for kq = 1 : length(rt)
    if kq > 1
        tet(kq) = tet(kq-1)+dt*rt(kq);
    else
        tet(kq) = rt(kq);
    end
end

figure(500);plot(mod(tet,2*pi))

figure(600);plot(ACC_t(1,1:end))
figure(602);plot(ACC_t(2,1:end))

