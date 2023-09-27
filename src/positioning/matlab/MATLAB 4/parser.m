clear all;
% close all;
% aa = rosbag('2023-07-26-18-35-13.bag');
% aa = rosbag('2023-08-07-11-03-21.bag');
% aa = rosbag('2023-08-07-14-34-54.bag');
aa = rosbag('[xsens]2023-08-31-17-42-15_fast.bag');

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
kalman_on = 0; % seonghyun (2023.09.08)
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
T = table();
while((length(UWBMsg{1})>=k0(1))&&(length(UWBMsg{2})>=k0(2))&&(length(UWBMsg{3})>=k0(3))&&(length(UWBMsg{4})>=k0(4)))
    k = k + 1;
    for lk = 1 : 4
        s_time_a(lk) = double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Sec)+double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Nsec)/10^9;
        %                 s_time_a(lk) = 100000000000000000000000000;
    end
    %     s_time_a(lk+1) = 100000000000000000000000000;

    [min_stamp_v, min_stamp_i] = min(s_time_a);
    s_time = min_stamp_v;

    % 선택된 버퍼에서 다음 메시지로 넘어간다.
    k0(min_stamp_i) = k0(min_stamp_i) + 1;


    dt_uwb = s_time - s_time_prev;
    s_time_prev = s_time;
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
    RxIDUWB = zeros(Ln, 1);
    RxIDUWB(1:Nanchor) = RxID;
    RxDistUWB = zeros(Ln,1);
    RxDistUWB(1:Nanchor) = RxDist;

    xain = zeros(1,Ln);
    xain(1:LnC) = xa;

    yain = zeros(1,Ln);
    yain(1:LnC) = ya;

    %% Anchor Position 조정 필요시..
    %         xain(1:LnC) = real(anch_pos_o(1:LnC));
    %         yain(1:LnC) = imag(anch_pos_o(1:LnC));

    %         xa = xain;
    %         ya = yain;
    
    if (Nanchor > 1) && (isempty(find(RxDistUWB(1:Nanchor)==0, 1)))

        % 복소수 데이터 처리: 실수와 허수로 분해
        tag_pos_b_real = reshape(real(tag_pos_b), [], 1);
        tag_pos_b_imag = reshape(imag(tag_pos_b), [], 1);
        tag_pos_b_processed = [tag_pos_b_real; tag_pos_b_imag];




        lengths = [length(Ln), length(Lp), length(LnC), length(PP), length(Nanchor), ...
            length(RxIDUWB), length(RxDistUWB), length(s_time), length(tag_pos_b_processed), ...
            length(xain), length(yain), length(tag_pos_b_processed), length(kf_psi) ];

        disp(lengths);

        % 가장 긴 데이터의 길이 계산
        maxLen = max(lengths);

        % 각 변수를 maxLen 길이로 만듭니다.
        Ln1 = [Ln; NaN(maxLen - length(Ln), 1)];
        Lp1 = [Lp; NaN(maxLen - length(Lp), 1)];
        LnC1 = [LnC; NaN(maxLen - length(LnC), 1)];
        PP1 = [PP; NaN(maxLen - length(PP), 1)];
        Nanchor1 = [Nanchor; NaN(maxLen - length(Nanchor), 1)];
        RxIDUWB1 = [RxIDUWB; NaN(maxLen - length(RxIDUWB), 1)];
        RxDistUWB1 = [RxDistUWB; NaN(maxLen - length(RxDistUWB), 1)];
        s_time1 = [s_time; NaN(maxLen - length(s_time), 1)];
        kf_psi1 = [kf_psi; NaN(maxLen - length(kf_psi), 1)];
        prevTagPos_processed1 = [reshape(TagPos, [], 1); NaN(maxLen - length(TagPos), 1)];
        tag_pos_b_processed1 = [tag_pos_b_processed; NaN(maxLen - length(tag_pos_b_processed), 1)];
        xain_processed1 = [reshape(xain, [], 1); NaN(maxLen - length(xain), 1)];
        yain_processed1 = [reshape(yain, [], 1); NaN(maxLen - length(yain), 1)];

        % CSV 파일로 저장
        T_temp = table(Ln1, Lp1, LnC1, PP1, Nanchor1, RxIDUWB1, RxDistUWB1, s_time1, ...
            tag_pos_b_processed1, xain_processed1, yain_processed1, ...
            prevTagPos_processed1, kf_psi1);
        T = [T; T_temp];

        
    else
        k0(PP) = k0(PP) + 1;
    end
end
writetable(T, 'processed_data3.csv');