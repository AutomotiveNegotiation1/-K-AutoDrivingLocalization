clear all;
% close all;
% aa = rosbag('2023-07-26-18-35-13.bag');
% aa = rosbag('2023-08-07-11-03-21.bag');
% aa = rosbag('2023-08-07-14-34-54.bag');
% aa = rosbag('[zed_f9r]2023-08-31-17-58-37_fast.bag');
% aa = rosbag('[zed_f9r]2023-08-31-17-56-41_slow.bag');
% aa = rosbag('[zed_f9r]2023-08-31-18-06-05_parking.bag');
% aa = rosbag('[zed_f9r]2023-12-07-12-49-42.bag');
% aa = rosbag('[zed_f9r]2023-12-05-18-16-11.bag');
aa = rosbag('[zed_f9r]2023-08-31-18-00-04_long_stop.bag');

IMUSel = 2; % 1 : Xsens,  2 : F9R

LenTot = 0;
for dfe = 1 : 4
    % UWB{dfe} = select(aa,'Topic',['/dwm1001/anchor/tag',num2str(dfe-1,1)] );
    UWB{dfe} = select(aa,'Topic',['/dwm1001/anchor/ttyUWB',num2str(dfe-1,1)] );

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

xt_b = [-0.525 0.525 -0.525 0.525];
yt_b = [0.505 0.505 -0.505 -0.505];
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

while((length(UWBMsg{1})>=k0(1))&&(length(UWBMsg{2})>=k0(2))&&(length(UWBMsg{3})>=k0(3))&&(length(UWBMsg{4})>=k0(4))&&(length(IMUMsg)>=k0(5)))
    k = k + 1;
    for lk = 1 : 4
        s_time_a(lk) = double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Sec)+double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Nsec)/10^9;
    end
    s_time_a(lk+1) = double(IMUMsg{k0(lk+1)}.Header.Stamp.Sec)+double(IMUMsg{k0(lk+1)}.Header.Stamp.Nsec)/10^9;

    [min_stamp_v, min_stamp_i] = min(s_time_a);
    s_time = min_stamp_v;

    if min_stamp_i == 5
        if IMUSel == 1
            IMUacc_c  = [IMUAMsg{k0(5)}.Vector.X IMUAMsg{k0(5)}.Vector.Y IMUAMsg{k0(5)}.Vector.Z];
            IMUgyro_c = [IMUVMsg{k0(5)}.Vector.X IMUVMsg{k0(5)}.Vector.Y IMUVMsg{k0(5)}.Vector.Z];
        else
            IMUacc_c = [IMUMsg{k0(5)}.LinearAcceleration.X IMUMsg{k0(5)}.LinearAcceleration.Y IMUMsg{k0(5)}.LinearAcceleration.Z]*1;
            IMUgyro_c  = [IMUMsg{k0(5)}.AngularVelocity.X IMUMsg{k0(5)}.AngularVelocity.Y IMUMsg{k0(5)}.AngularVelocity.Z];
        end
        IMUacc_c_t(k0(5),:) = IMUacc_c;      %%% debuging 용
        IMUgyro_c_t(k0(5),:) = IMUgyro_c;    %%% debuging 용
        
        [cent_pos_est, cent_vel_est, kf_psi, gyro_psi, b_acc_o, b_gyro, state_IMU, acc_b_phi, acc_b_theta ] = IMUpos(IMUacc_c, IMUgyro_c, s_time, b_acc_o, b_gyro, IMUSel, kf_psi, gyro_psi, cent_pos_est, cent_vel_est);

        k0(5) = k0(5) + 1;

    elseif min_stamp_i<5

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
        RxIDUWB = zeros(Ln,1);
        RxIDUWB(1:Nanchor) = RxID;
        RxDistUWB = zeros(Ln,1);
        RxDistUWB(1:Nanchor) = RxDist;

        xain = zeros(1,Ln);
        xain(1:LnC) = xa;

        yain = zeros(1,Ln);
        yain(1:LnC) = ya;

        if (Nanchor > 1) && (isempty(find(RxDistUWB(1:Nanchor)==0, 1)))
            % UWBpos_Base(Ln, Lp, LnC, PP, Nanchor, RxIDUWB, RxDistUWB, s_time, tag_pos_b, xain, yain, TagPos, -kf_psi);
            [UWBout] = UWBpos_V3(Ln, Lp, LnC, PP, Nanchor, RxIDUWB, RxDistUWB, s_time, tag_pos_b, xain, yain, TagPos, -kf_psi);
            %[UWBout] = UWBpos6(Ln, Lp, LnC, PP, Nanchor, RxIDUWB, RxDistUWB, s_time, tag_pos_b, xain, yain, TagPos, -kf_psi);
            %             UWBoutTot(k,:) = UWBout;

            tag_pos_est = UWBout(1:4)+j*UWBout(5:8);
            heading_est = UWBout(9);
            tag_pos_est_aver = UWBout(10:13)+j*UWBout(14:17);
            headingest_a_aver_v = UWBout(18);
            tag_center_vel_est = UWBout(19)+j*UWBout(20);

            UWBErrSum = UWBout(21);
            Heading_New(k) = UWBout(22);
            
            if (heading_est ~=0) && (init_flag == 0)
                init_flag = 1;
            elseif (heading_est ~=0) && (init_flag == 1)
                init_flag = 1;
            elseif (init_flag == 2)
                init_flag = 3;
            elseif (init_flag == 3)
                init_flag = 3;
            else
                init_flag = 0;
            end

            TagPosBuff(:,1:end-1) = TagPosBuff(:,2:end);
            TagPosBuff(:,end) = tag_pos_est;
            TagRoute = mean(TagPosBuff(:,end))-mean(TagPosBuff(:,1));


            if Nanchor > 2
                Reliability = ReliCalc(tag_pos_est, xain+j*yain, RxDist, RxID, PP);
            else
                Reliability = 0;
            end

            k0(PP) = k0(PP) + 1;

            RxIDT{k} = RxID;
            RxTime(k,PP) = s_time;
            RxIDTot(k,PP) = length(RxID);

            tag_pos_est_total{k} = tag_pos_est;
            center_pos_est_total(k) = mean(tag_pos_est);
            headingest_total(k) = heading_est;
            center_pos_est_aver_total(k) = mean(tag_pos_est_aver);
            headingest_aver_total(k) = headingest_a_aver_v;
            tag_center_vel_est_total(k) = tag_center_vel_est;

            RxDistOrigTot(k,RxID,PP)=RxDistOrig;
            RxDistTot(k,RxID,PP)=RxDist;

            % IMUposU = ((9*tag_pos_est(3)+tag_pos_est(1))+(9*tag_pos_est(4)+tag_pos_est(2)))/20;
            % init_pos.x = real(IMUposU);
            % init_pos.y = imag(IMUposU);
            % init_pos.z = zt_b; 

            [IMUposU, b_acc_o, kf_psi, gyro_psi, cent_pos_est, cent_vel_est] = fusion3(kl, k0, tag_pos_est, tag_center_vel_est, cent_pos_est, cent_vel_est, b_acc_o, acc_b_phi, Acc_Pos, Acc_Vel, kalman_on, init_flag, UWBErrSum, kf_psi, gyro_psi, heading_est, acc_b_theta, IMUSel, Nanchor, state_IMU);
        else
            k0(PP) = k0(PP) + 1;

        end
        if (init_flag == 1)
            gyro_psi = -heading_est;
            kf_psi = gyro_psi;
            cent_pos_est = [real(IMUposU);imag(IMUposU);0];
            cent_vel_est = [real(tag_center_vel_est);imag(tag_center_vel_est);0];
        end
    end
    
    TagPos = cent_pos_est(1)+j*cent_pos_est(2) + exp(j*(-kf_psi))*(tag_pos_b+0.4j);
    headigTot(k) = -kf_psi;


    TagPosTot(k,:) = TagPos;
    TagUWBPosTot(k,:) = tag_pos_est;
    TagPosTime(k) = s_time;
    Reliability_t(k) = Reliability;
    cent_vel_est_total(k) = cent_vel_est(1)+cent_vel_est(2)*j;
    b_acc_o_t(k,:)= b_acc_o;

    % if mod(k,1) == 0
    %         %figure(1);hold off;plot(cent_pos_est(1),cent_pos_est(2),'ro');hold on;plot(xa,ya,'o');axis([-5 30 -5 20]);axis equal
    %         figure(1);hold off;plot(cent_pos_est(1),cent_pos_est(2),'.');hold on;plot(xa,ya,'o');plot(real(TagPos(1:2)),imag(TagPos(1:2)),'ro');plot(real(TagPos(3:4)),imag(TagPos(3:4)),'ko');axis equal
    % 
    % end
    %         if min_stamp_i < 5
    % 
    %             if (RxIDTot(end,PP) == 4 ) && (Reliability>2)
    %                 figure(1);plot(real(tag_pos_est),imag(tag_pos_est),'b*')
    %             elseif (RxIDTot(end,PP) == 3 ) && (Reliability>2)
    %                 figure(1);plot(real(tag_pos_est),imag(tag_pos_est),'m*')
    %             else
    %                 figure(1);plot(real(tag_pos_est),imag(tag_pos_est),'r*')
    %             end
    % 
    %         end
end


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