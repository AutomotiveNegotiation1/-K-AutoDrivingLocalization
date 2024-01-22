clear all;
% close all;
% aa = rosbag('2023-07-26-18-35-13.bag');
% aa = rosbag('2023-08-07-11-03-21.bag');
% aa = rosbag('2023-08-07-14-34-54.bag');
% aa = rosbag('[zed_f9r]2023-08-31-17-58-37_fast.bag');
% aa = rosbag('[zed_f9r]2023-08-31-18-00-04_long_stop.bag');

% aa = rosbag('[zed_f9r]2023-08-31-17-56-41_slow.bag');
% aa = rosbag('[zed_f9r]2023-08-31-18-06-05_parking.bag');
% aa = rosbag('[zed_f9r]2023-12-07-13-09-48.bag');
aa = rosbag('[zed_f9r]2023-12-05-18-16-11.bag');
IMUSel = 2; % 1 : Xsens,  2 : F9R

LenTot = 0;
for dfe = 1 : 4
    % UWB{dfe} = select(aa,'Topic',['/dwm1001/anchor/ttyUWB',num2str(dfe-1,1)] );
    UWB{dfe} = select(aa,'Topic',['/dwm1001/anchor/tag',num2str(dfe-1,1)] );

    LenTot = LenTot + size(UWB{dfe}.MessageList,1);
    UWBMsg{dfe} = readMessages(UWB{dfe},'DataFormat','struct');
end

% for dfe = 1 : 4
%     UWBTag{dfe} = select(aa,'AvailableTopics',['/dwm1001/tag',num2str(dfe-1,1)] );
%     UWBTagMsg{dfe} = readMessages(UWBTag{dfe},'DataFormat','struct');
% end

if IMUSel == 1
    IMURoSV = select(aa,'Topic','zed_f9r/imu/angular_velocity');
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
kalman_on = 1; % seonghyun (2023.09.08)

% For 1205 version
xt_b = [-0.525 0.525 0.525 -0.525];
yt_b = [0.505 0.505 -0.505 -0.505];

%% Original
% xt_b = [-0.525 0.525 -0.525 0.525];
% yt_b = [0.505 0.505 -0.505 -0.505];

zt_b = 1.53;

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
gyro_delta_phi = 0;
gyro_delta_theta = 0;
gyro_delta_psi = 0;
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
acc_b_phi = 0;

acc_g_bias = [0 0 0]';
b_acc_b = [0 0 0]';
acc_b_kf_wob = [0 0 0]';
Cb2n_kf = eye(3);
kf_phi_m = 0;
kf_theta_m = 0;

Cb2n = eye(3);

Lav = 1;

IMUMode = 1;

%b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original
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

% 2023.11.09_ljw0904
kf_psi =0;
cent_vel_est =  zeros(3,1 );
cent_pos_est =  zeros(3,1 );
b_acc_o = [ 0    0   0];


acc_b_theta = 0;


UWB_LS_Pos = zeros(3,2*Lv);
UWB_M_Pos = zeros(3,2*Lv);
UWB_M_Vel = zeros(3,2*Lv);
kf_phi = 0;
kf_theta = 0;
Reliability = 0;

gyro_psi = 0;

if IMUSel == 1
    IMUMsg = IMUVMsg;
end

IMUacc_c_fifo = zeros(4,3);
IMUgyro_c_fifo = zeros(4,3);

Xhat = [0 0 0 0 0 0 0 0 ]';
P = eye(8);
qv = 1;
Heading_Prev = 0;
alpe_t1 = 0;
xain = [];
yain = [];


while((length(UWBMsg{1})>=k0(1))&&(length(UWBMsg{2})>=k0(2))&&(length(UWBMsg{3})>=k0(3))&&(length(UWBMsg{4})>=k0(4))&&(length(IMUMsg)>=k0(5)))
    k = k + 1;
    for lk = 1 : 4
        s_time_a(lk) = double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Sec)+double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Nsec)/10^9;
    end
    s_time_a(lk+1) = double(IMUMsg{k0(lk+1)}.Header.Stamp.Sec)+double(IMUMsg{k0(lk+1)}.Header.Stamp.Nsec)/10^9;

    [min_stamp_v, min_stamp_i] = min(s_time_a);
    s_time = min_stamp_v;

    PositionVector = [];
    PositionVector(1) = s_time;
    PositionVector(2) = min_stamp_i;

    if min_stamp_i == 5
        if IMUSel == 1
            IMUacc_c  = [IMUAMsg{k0(5)}.Vector.X IMUAMsg{k0(5)}.Vector.Y IMUAMsg{k0(5)}.Vector.Z];
            IMUgyro_c = [IMUVMsg{k0(5)}.Vector.X IMUVMsg{k0(5)}.Vector.Y IMUVMsg{k0(5)}.Vector.Z];
        else
            IMUacc_c = [IMUMsg{k0(5)}.LinearAcceleration.X IMUMsg{k0(5)}.LinearAcceleration.Y IMUMsg{k0(5)}.LinearAcceleration.Z]*1;
            IMUgyro_c  = [IMUMsg{k0(5)}.AngularVelocity.X IMUMsg{k0(5)}.AngularVelocity.Y IMUMsg{k0(5)}.AngularVelocity.Z];
        end
        IMUacc_c_t(k0(5),:) = IMUacc_c;
        IMUgyro_c_t(k0(5),:) = IMUgyro_c;

        IMUacc_c_fifo = [IMUacc_c_fifo(2:4,:) ;IMUacc_c];
        IMUgyro_c_fifo = [IMUgyro_c_fifo(2:4,:) ;IMUgyro_c];

        PositionVector(3:5) = IMUacc_c;
        PositionVector(6:8) = IMUgyro_c;

        k0(5) = k0(5) + 1;

    elseif min_stamp_i<5

        PP = min_stamp_i;
        RxID = [];
        TT = UWBMsg{PP}{k0(PP)}.Id;

        for df = 1 : length(TT)
            [val,ids] = find(AnchorIDmap == hex2dec(TT{df}));
            if length(ids)>0
                RxID(df) = ids;
            else
                LnC = LnC + 1;
                AnchorIDmapH{LnC} = TT{df};
                AnchorIDmap(LnC) = hex2dec(AnchorIDmapH{LnC});
                RxID(df) = LnC;
                xa(LnC) = UWBMsg{PP}{k0(PP)}.X(df);
                ya(LnC) = UWBMsg{PP}{k0(PP)}.Y(df);
                za(LnC) = UWBMsg{PP}{k0(PP)}.Z(df);

            end
        end
        RxDistOrig = UWBMsg{PP}{k0(PP)}.DistanceFromTag;
        RxDist = real(sqrt(RxDistOrig.^2-(za(1)-zt_b)^2));

        Nanchor = length(RxID);
        RxIDUWB = zeros(Ln,1);
        RxIDUWB(1:Nanchor) = RxID;
        RxDistUWB = zeros(Ln,1);
        RxDistUWB(1:Nanchor) = RxDist;

        xain = zeros(1,Ln);
        xain(1:LnC) = xa;

        yain = zeros(1,Ln);
        yain(1:LnC) = ya;

        PositionVector = [PositionVector LnC Nanchor RxID RxDistOrig' xain(1:LnC) yain(1:LnC) za xt_b yt_b zt_b];

        k0(PP) = k0(PP) + 1;
    end

    [PosH(k), HeadingH(k), P] = PositioningSystem_V1(PositionVector);
    Uncertainty(:,k) = P(1:2);
    
    % if (PosH(k) ~= 0) & ~isempty(xain)
    %     % figure(23);hold on;plot(PosH(k),'.')
    %     figure(23);hold off;plot(xain,yain,'square',MarkerSize=10);axis([-5 50 -7 30]);axis equal;
    %     figure(23);hold on;plot(PosH(max(1,k-100):k),'b.')
    %     tagPos = PosH(k)+(xt_b*cos(-HeadingH(k))-yt_b*sin(-HeadingH(k)))+j*(xt_b*sin(-HeadingH(k))+yt_b*cos(-HeadingH(k)));
    %     figure(23);hold on;plot(tagPos(1),'r.',MarkerSize=10);
    %     figure(23);hold on;plot(tagPos(2:4),'b.',MarkerSize=10);
    %     figure(23);plot(xa(RxIDUWB(1:Nanchor)),ya(RxIDUWB(1:Nanchor)),'rsquare',MarkerSize=6)
    %     dd = 1;
    % end
end
