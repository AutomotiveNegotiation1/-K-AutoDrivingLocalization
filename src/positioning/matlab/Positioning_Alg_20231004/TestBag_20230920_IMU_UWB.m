clear all;
% close all;
% aa = rosbag('2023-07-26-18-35-13.bag');
% aa = rosbag('2023-08-07-11-03-21.bag');
% aa = rosbag('2023-08-07-14-34-54.bag');
aa = rosbag('[xsens]2023-08-31-17-40-03_slow.bag');

IMUSel =1; % 1 : Xsens,  2 : F9R

LenTot = 0;
for dfe = 1 : 4
    UWB{dfe} = select(aa,'Topic',['/dwm1001/anchor/ttyUWB',num2str(dfe-1,1)] );
    LenTot = LenTot + size(UWB{dfe}.MessageList,1);
    UWBMsg{dfe} = readMessages(UWB{dfe},'DataFormat','struct');
end

for dfe = 1 : 4
    UWBTag{dfe} = select(aa,'Topic',['/dwm1001/ttyUWB',num2str(dfe-1,1)] );
    UWBTagMsg{dfe} = readMessages(UWBTag{dfe},'DataFormat','struct');
end

if IMUSel == 1
IMURoSV = select(aa,'Topic','/imu/angular_velocity');
IMUVMsg = readMessages(IMURoSV,'DataFormat','struct');

IMURoSA = select(aa,'Topic','/imu/acceleration');
IMUAMsg = readMessages(IMURoSA,'DataFormat','struct');

IMURoSM = select(aa,'Topic','/imu/mag');
IMUMMsg = readMessages(IMURoSM,'DataFormat','struct');

else

IMURoS = select(aa,'Topic','/zed_f9r/imu');
IMUMsg = readMessages(IMURoS,'DataFormat','struct');
end


LnC = 0;
Lp = 4;
Ln = 6;
kalman_on = 0;
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

gravity = 9.86;

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

IMUMode = 1;

%b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original
b_acc_o = [ 0    0   0]; 
b_acc = [ -0.2921    0.1928   -0.019];  %% original
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
k = 0;
s_time_prev = 0;
%for k = 1 : LenTot*3-300
% for k = 1 : 12754
state_acc = 0 ; % 0: init,  1 : Work
state_IMU = 0 ; % 0: init,  1 : Work
RxIDTot = zeros(1,4);
cent_vel_est =  zeros(3,1 );
cent_pos_est =  zeros(3,1 );
heading_est_t = 0;
tag_pos_est_t = zeros(1,4);

s_time_prev = 0;
anch_pos_o  = [-0.01-0.3i	7.85-0.3i	7.84+10.5i	0+15.1i	22.7-4i    24.6+16.3i];
% anch_pos_o  = [-0.01-0.3i	7.85-0.3i	7.84+10.5i	0+15.1i	22.5-3.9i    24.4+15.3i];

init_flag = 0;
TagPosBuff = zeros(4,20);
tag_pos_est = zeros(1,4);
heading_est = 0;
TagPos = [0 0 0 0];

if IMUSel == 1
    IMUMsg = IMUVMsg;
end
   
while((length(UWBMsg{1})>=k0(1))&&(length(UWBMsg{2})>=k0(2))&&(length(UWBMsg{3})>=k0(3))&&(length(UWBMsg{4})>=k0(4))&&(length(IMUMsg)>=k0(5)))
    k = k + 1;
    for lk = 1 : 4
        s_time_a(lk) = double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Sec)+double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Nsec)/10^9;
        %                 s_time_a(lk) = 100000000000000000000000000;
    end
    s_time_a(lk+1) = double(IMUMsg{k0(lk+1)}.Header.Stamp.Sec)+double(IMUMsg{k0(lk+1)}.Header.Stamp.Nsec)/10^9;
    %     s_time_a(lk+1) = 100000000000000000000000000;

    [min_stamp_v, min_stamp_i] = min(s_time_a);
    s_time = min_stamp_v;

    if min_stamp_v == 5
        if IMUSel == 1
            IMUacc_c  = [IMUAMsg{k0(5)}.Vector.X IMUAMsg{k0(5)}.Vector.Y IMUAMsg{k0(5)}.Vector.Z];
            IMUgyro_c = [IMUVMsg{k0(5)}.Vector.X IMUVMsg{k0(5)}.Vector.Y IMUVMsg{k0(5)}.Vector.Z];
        else
            IMUacc_c = [IMUMsg{k0(5)}.LinearAcceleration.X IMUMsg{k0(5)}.LinearAcceleration.Y IMUMsg{k0(5)}.LinearAcceleration.Z]*1;
            IMUgyro_c  = [IMUMsg{k0(5)}.AngularVelocity.X IMUMsg{k0(5)}.AngularVelocity.Y IMUMsg{k0(5)}.AngularVelocity.Z];
        end
        k0(5) = k0(5) + 1;

    else
        TT = UWBMsg{PP}{k0(PP)}.Id;
        RxDistOrig = UWBMsg{PP}{k0(PP)}.DistanceFromTag;
        
    end


    

    TagPosTot(k,:) = TagPos;
    TagUWBPosTot(k,:) = tag_pos_est;
    TagPosTime(k) = s_time;
    Reliability_t(k) = Reliability;
    cent_vel_est_total(k) = cent_vel_est(1)+cent_vel_est(2)*j;
    b_acc_o_t(k,:)= b_acc_o;
end

figure(2001);hold off;plot(TagPosTime,real(mean(TagPosTot,2)));hold on;plot(TagPosTime,real(mean(TagUWBPosTot,2)),'r')
figure(2002);hold off;plot(TagPosTime,imag(mean(TagPosTot,2)));hold on;plot(TagPosTime,imag(mean(TagUWBPosTot,2)),'r')
figure(2011);hold off;plot(real(mean(TagPosTot,2)));hold on;plot(real(mean(TagUWBPosTot,2)),'.r')
figure(2012);hold off;plot(imag(mean(TagPosTot,2)));hold on;plot(imag(mean(TagUWBPosTot,2)),'.r')

figure(2003);hold off;plot(TagPosTot,'.')
figure(2004);hold off;plot(TagUWBPosTot,'.');axis equal

figure(33);hold off;plot((center_pos_est_total(40+1:end)),'.')
% figure(33);hold on;plot(center_pos_est_aver_total(40+1:end),'r.')
figure(33);hold on;plot(real(anch_pos_o),imag(anch_pos_o),'b*');axis equal
figure(33);plot(mean(TagPosTot(40+1:end,:),2),'r.')

figure(34);hold off;plot(TagPosTot(40+1:end),'.')
figure(34);hold on;plot(real(anch_pos_o),imag(anch_pos_o),'b*');axis equal

figure(35);hold off;plot(mod(headingest_total(40+1:end),2*pi)*180/pi,'bo')
% figure(35);hold on;plot(mod(headingest_aver_total(40+1:end),2*pi)*180/pi,'ro')

An = xain + j*yain;
% tag_pos = 0.21+2.96j;
% tag_pos = 0.700 + j*3.024;
% for kk = 1 : 4
%     dist_exp(:,kk) = abs(An-(tag_pos+tag_pos_b(kk)));
% end
%
% for g = 1 : LnC
%     figure(100+g)
%     for m = 1 : Lp
%         figure(100+g);hold off;subplot(2,2,m);plot(RxDistTot(:,g,m),'.');hold on;plot(dist_exp(g,m)*ones(1,size(RxDistTot,1)),'r')
%     end
% end
%
% for g = 1 : LnC
%     figure(200+g);
%     for m = 1 : Lp
%         figure(200+g);hold off;subplot(2,2,m);plot(RxDistOrigTot(:,g,m),'.');hold on;plot(dist_exp(g,m)*ones(1,size(RxDistOrigTot,1)),'r')
%     end
% end
%
% for g = 1 : LnC
%     for m = 1 : 4
%
%         RxDistOrigTotNonzeroInd = find(RxDistOrigTot(:,g,m)~=0);
%         RxDistOrigTotNonzero = RxDistOrigTot(RxDistOrigTotNonzeroInd,g,m);
%         aver_dist_org(g,m)=mean(RxDistOrigTotNonzero);
%
%         RxDistTotNonzeroInd = find(RxDistTot(:,g,m)~=0);
%         RxDistTotNonzero = RxDistTot(RxDistTotNonzeroInd,g,m);
%         aver_dist(g,m)=mean(RxDistTotNonzero);
%
%     end
% end
%
RxIDTot = zeros(length(RxIDT),5);
for df = 1 : length(RxIDT)
    RxIDL(df) = length(RxIDT{df});
    RxIDTot(df,1:RxIDL(df)) = RxIDT{df};
end
figure(99);plot(RxIDTot,'.')

figure(88);hold off;plot(RxIDL/10,'r');hold on;plot(center_pos_est_total)

figure(99);plot(headigTot)
figure(99996);hold off;plot(TagPosTime,RxDistOrigTot(:,:,1),'o');hold on;plot(TagPosTime,imag(mean(TagUWBPosTot,2)),'r')
figure(99997);hold off;plot(TagPosTime,RxDistOrigTot(:,:,2),'o');hold on;plot(TagPosTime,imag(mean(TagUWBPosTot,2)),'r')
figure(99998);hold off;plot(TagPosTime,RxDistOrigTot(:,:,3),'o');hold on;plot(TagPosTime,imag(mean(TagUWBPosTot,2)),'r')
figure(99999);hold off;plot(TagPosTime,RxDistOrigTot(:,:,4),'o');hold on;plot(TagPosTime,imag(mean(TagUWBPosTot,2)),'r')
% figure(99999);hold off;plot(TagPosTime,RxDistOrigTot(:,:,1),'o');hold on;plot(TagPosTime,RxDistOrigTot(:,2,1),'go');plot(TagPosTime,RxDistOrigTot(:,3,1),'ro');plot(TagPosTime,RxDistOrigTot(:,4,1),'ko');plot(TagPosTime,imag(mean(TagUWBPosTot,2)),'r')

figure(10000);hold off;plot(RxDistOrigTot(:,:,1),'o');hold on;plot(imag(mean(TagUWBPosTot,2)),'r')
figure(10001);hold off;plot(RxDistOrigTot(:,:,2),'o');hold on;plot(imag(mean(TagUWBPosTot,2)),'r')
figure(10002);hold off;plot(RxDistOrigTot(:,:,3),'o');hold on;plot(imag(mean(TagUWBPosTot,2)),'r')
figure(10003);hold off;plot(RxDistOrigTot(:,:,4),'o');hold on;plot(imag(mean(TagUWBPosTot,2)),'r')

figure(20103);plot(UWBErrSum,'.')
figure(9112);hold off;plot(b_acc_o_t(:,1),'r.');hold on;plot(b_acc_o_t(:,2),'b.');plot(b_acc_o_t(:,3),'g.')