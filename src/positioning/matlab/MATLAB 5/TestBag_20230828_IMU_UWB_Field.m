clear all;
% close all;
% aa = rosbag('2023-07-26-18-35-13.bag');
% aa = rosbag('2023-08-07-11-03-21.bag');
% aa = rosbag('2023-08-07-14-34-54.bag');

from_File = 1;
IMUSel =2; % 1 : Xsens,  2 : F9R

if from_File == 1;
    aa = rosbag('[zed_f9r]2023-08-31-17-56-41_slow.bag');
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

else
    [subImu, subUwb0, subUwb1, subUwb2, subUwb3] = conf_ros_env;
end
% aa = rosbag('[xsens]2023-08-31-17-40-03_slow.bag');
%
%

% LenTot = 0;
% for dfe = 1 : 4
%     UWB{dfe} = select(aa,'Topic',['/dwm1001/anchor/ttyUWB',num2str(dfe-1,1)] );
%     LenTot = LenTot + size(UWB{dfe}.MessageList,1);
%     UWBMsg{dfe} = readMessages(UWB{dfe},'DataFormat','struct');
% end
%
% for dfe = 1 : 4
%     UWBTag{dfe} = select(aa,'Topic',['/dwm1001/ttyUWB',num2str(dfe-1,1)] );
%     UWBTagMsg{dfe} = readMessages(UWBTag{dfe},'DataFormat','struct');
% end
%
% if IMUSel == 1
% IMURoSV = select(aa,'Topic','/imu/angular_velocity');
% IMUVMsg = readMessages(IMURoSV,'DataFormat','struct');
%
% IMURoSA = select(aa,'Topic','/imu/acceleration');
% IMUAMsg = readMessages(IMURoSA,'DataFormat','struct');
%
% IMURoSM = select(aa,'Topic','/imu/mag');
% IMUMMsg = readMessages(IMURoSM,'DataFormat','struct');
%
% else
%
% IMURoS = select(aa,'Topic','/zed_f9r/imu');
% IMUMsg = readMessages(IMURoS,'DataFormat','struct');
% end
%

LnC = 0;
Lp = 4;
Ln = 6;
kalman_on = 1;

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


while(1)
    k = k + 1;
    
    if from_File == 1
        for lk = 1 : 4
            s_time_a(lk) = double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Sec)+double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Nsec)/10^9;
        end
        s_time_a(lk+1) = double(IMUMsg{k0(lk+1)}.Header.Stamp.Sec)+double(IMUMsg{k0(lk+1)}.Header.Stamp.Nsec)/10^9;
    
        [min_stamp_v, min_stamp_i] = min(s_time_a);
        
        for lk = 1 : 4
            statusUwb{lk} = 0;
        end
        statusImu = 0;

        if min_stamp_i == 5
            msgImu = IMUMsg{k0(5)};
            statusImu = 1;
        else
            statusUwb{min_stamp_i} = 1;
            msgUwb{min_stamp_i} = UWBMsg{min_stamp_i}{k0(min_stamp_i)};
        end
        k0(min_stamp_i) = k0(min_stamp_i) + 1;
    else
        [msgImu,statusImu,statustextImu] = receive(subImu);

        [msgUwb{1},statusUwb{1},statustextUwb0] = receive(subUwb0);
        [msgUwb{2},statusUwb{2},statustextUwb1] = receive(subUwb1);
        [msgUwb{3},statusUwb{3},statustextUwb2] = receive(subUwb2);
        [msgUwb{4},statusUwb{4},statustextUwb3] = receive(subUwb3);
    end

    
    for lk = 1 : 4
        if statusUwb{lk} == 1
            s_time_a(lk) = double(msgUwb{lk}.Header.Stamp.Sec)+double(msgUwb{lk}.Header.Stamp.Nsec)/10^9;
        else
            s_time_a(lk) = 100000000000000000;
        end
    end

    if statusImu == 1
        s_time_a(lk+1) = double(msgImu.Header.Stamp.Sec)+double(msgImu.Header.Stamp.Nsec)/10^9;
    else
        s_time_a(lk+1) = 100000000000000000;
    end


    [sort_s_time,sort_s_time_index] = sort(s_time_a);
    for kk = 1 : 5

        if (sort_s_time(kk) < 100000000000000000)
            s_time = sort_s_time(kk);
            min_stamp_i = sort_s_time_index(kk);


            if min_stamp_i == 5
                IMUacc_c   = [msgImu.LinearAcceleration.X msgImu.LinearAcceleration.Y msgImu.LinearAcceleration.Z]*1;
                IMUgyro_c  = [msgImu.AngularVelocity.X msgImu.AngularVelocity.Y msgImu.AngularVelocity.Z];

                IMUacc_t(k,:) = IMUacc_c;
                IMUgyro_t(k,:) = IMUgyro_c;
                
                [cent_pos_est, cent_vel_est, kf_psi, gyro_psi, b_acc_o, b_gyro, state_IMU, acc_b_phi, acc_b_theta ] = IMUpos(IMUacc_c, IMUgyro_c, s_time, b_acc_o, b_gyro, IMUSel, kf_psi, gyro_psi, cent_pos_est, cent_vel_est);

            elseif min_stamp_i<5

                dt_uwb = s_time - s_time_prev;
                s_time_prev = s_time;
                PP = min_stamp_i;
                RxID = [];

                TT = msgUwb{PP}.Id;
                for df = 1 : length(TT)
                    [val,ids] = find(AnchorIDmap == hex2dec(TT{df}));
                    if length(ids)>0
                        RxID(df) = ids;

                    else
                        LnC = LnC + 1;
                        AnchorIDmapH{LnC} = TT{df};
                        AnchorIDmap(LnC) = hex2dec(AnchorIDmapH{LnC});
                        RxID(df) = LnC;
                        xa(LnC) = msgUwb{PP}.X(df);
                        ya(LnC) = msgUwb{PP}.Y(df);
                        za(LnC) = msgUwb{PP}.Z(df);

                    end
                end

                RxDistOrig = msgUwb{PP}.DistanceFromTag;



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

                %% Anchor Position 조정 필요시..
                %         xain(1:LnC) = real(anch_pos_o(1:LnC));
                %         yain(1:LnC) = imag(anch_pos_o(1:LnC));

                %         xa = xain;
                %         ya = yain;

                if (Nanchor > 1) && (isempty(find(RxDistUWB(1:Nanchor)==0, 1)))
                    [UWBout] = UWBpos6(Ln, Lp, LnC, PP, Nanchor, RxIDUWB, RxDistUWB, s_time, tag_pos_b, xain, yain, TagPos, -kf_psi);
                    %             UWBoutTot(k,:) = UWBout;

                    tag_pos_est = UWBout(1:4)+j*UWBout(5:8);
                    heading_est = UWBout(9);
                    tag_pos_est_aver = UWBout(10:13)+j*UWBout(14:17);
                    headingest_a_aver_v = UWBout(18);
                    tag_center_vel_est = UWBout(19)+j*UWBout(20);

                    UWBErrSum(k) = UWBout(21);

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
                    DistErr(k,RxID,PP) = abs(abs(anch_pos_o(RxID)-tag_pos_est(PP))-RxDist');

                    IMUposU = ((9*tag_pos_est(3)+tag_pos_est(1))+(9*tag_pos_est(4)+tag_pos_est(2)))/20;
                    init_pos.x = real(IMUposU);
                    init_pos.y = imag(IMUposU);
                    init_pos.z = zt_b;

                    if ((k0(5)>50))  % INS/UWB
                        UWB_LS_Pos(:,1:end-1) = UWB_LS_Pos(:,2:end);
                        UWB_LS_Pos(:,end) = [real(IMUposU) ;imag(IMUposU); zt_b] ;
                        %
                        %                 if (RxIDTot(k,PP) >= 3) && (Reliability>2) && (kalman_on)
                        if (kalman_on == 1) && (init_flag > 2)  && (UWBErrSum(k) <0.5)%&& (Nanchor>2) && (Reliability >2)

                            Tasg = double(int32(abs(mod(-heading_est,2*pi) - kf_psi)/(2*pi)));
                            if mod(-heading_est,2*pi) - kf_psi > pi
                                headingest_a_aver_vm = mod(-heading_est,2*pi)-2*pi*Tasg;
                            else
                                headingest_a_aver_vm = mod(-heading_est,2*pi)+2*pi*Tasg;
                            end

                            qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, headingest_a_aver_vm);
                            qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);

                            AA = 1*eye(4);

                            [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
                            kf_phi =real(kf_phi);
                            kf_theta =real(kf_theta);
                            kf_psi =real(kf_psi);
                            gyro_psi = kf_psi;

                        end

                        if (kl > Lv*2)
                            Ta = mean(UWB_LS_Pos(:,end-Lv+1:end)')';
                            Tb = mean(UWB_LS_Pos(:,end-2*Lv+1:end-Lv)')';
                            Tm = (Ta+Tb)/2;
                            Td = Ta-Tm;
                            UWB_M_Pos(:,1:end-1) = UWB_M_Pos(:,2:end);
                            UWB_M_Vel(:,1:end-1) = UWB_M_Vel(:,2:end);
                            %                     UWB_M_Pos(:,end) = (Tm+Td);
                            %                     UWB_M_Pos(:,end) = (Ta);
                            UWB_M_Pos(:,end) = [real(IMUposU);imag(IMUposU);0];
                            UWB_M_Vel(:,end) = [real(tag_center_vel_est);imag(tag_center_vel_est);0];
                            %                     UWB_M_Vel(:,end) = (UWB_M_Pos(:,end)-UWB_M_Pos(:,end-9))/10;
                            %                     tag_center_vel_est
                        else
                            UWB_M_Pos(:,1:end-1) = UWB_M_Pos(:,2:end);
                            UWB_M_Vel(:,1:end-1) = UWB_M_Vel(:,2:end);

                            UWB_M_Pos(:,end) = [init_pos.x init_pos.y init_pos.z]';
                            UWB_M_Vel(:,end) = [0 0 0]';
                        end

                        if (kalman_on == 1) && (init_flag > 2) && (UWBErrSum(k) <1)
                            if IMUSel == 1
                                R = 1e0*eye(6);
                            else
                                R = 1e0*eye(6);
                            end
                            TEMP = [[IMUacc(end,1);IMUacc(end,2);0]];
                            TEMP_bias = [b_acc_o(1);b_acc_o(2);b_acc_o(3)];

                            %x = [cent_pos_est;cent_vel_est(1:2);0;TEMP;TEMP_bias];
                            x = [cent_pos_est;cent_vel_est;TEMP;TEMP_bias];


                            %Cb2n = [0 1 0;1 0 0;0 0 1]*rotationVectorToMatrix([0;0;kf_psi])';
                            Cb2n = [0 1 0;1 0 0;0 0 1]*rotationVectorToMatrix([kf_phi;kf_theta;kf_psi])';
                            A = eye(12);
                            p = 0;            q = 0;             r = 0;
                            A(1:3,4:6) = dt*eye(3)-1/2*dt^2*[0 -r q;r 0 -p;-q p 0];
                            A(1:3,7:9) = Cb2n*dt^2/2;
                            A(1:3,10:12) = -Cb2n*dt^2/2;
                            A(4:6,4:6) = A(4:6,4:6) - dt*[0 -r q;r 0 -p;-q p 0];
                            A(4:6,7:9) = Cb2n*dt;
                            A(4:6,10:12) = -Cb2n*dt;


                            if (abs((UWB_M_Pos(1,end)+j*UWB_M_Pos(2,end))-(cent_pos_est(1)+j*cent_pos_est(2)))<2)
                                if IMUSel == 1
                                    R = 1e0*eye(6);
                                else
                                    R = 1e-1*eye(6);
                                end
                                z = [UWB_M_Pos(:,end)];
                                x_n = PosKalman2(x,A,z,R(1:3,1:3));
                            else
                                if IMUSel == 1
                                    R = 1e-1*eye(6);
                                else
                                    R = 1e-2*eye(6);
                                end
                                z = [UWB_M_Pos(:,end)];
                                x_n = PosKalman2(x,A,z,R(1:3,1:3));
                                %                         z = [UWB_M_Pos(:,end);UWB_M_Vel(:,end)];
                                %                         x_n = PosKalman(x,A,z,R);
                                x_n(1:3) = UWB_M_Pos(:,end);
                                x_n(4:6) = UWB_M_Vel(:,end);
                            end

                            Acc_Pos(:,end) = x_n(1:3,1);

                            Acc_Vel(:,end) = x_n(4:6,1);

                            if (Nanchor>1)
                                if state_IMU == 2
                                    b_acc_o = x_n(10:12,1)';
                                    %b_acc_o = [-0.1459    0.2830   -0.0016];
                                end
                                cent_pos_est = Acc_Pos(:,end);
                                cent_vel_est = Acc_Vel(:,end);
                            end


                        end


                        kl = kl + 1;


                        Acc_S = [0 0 0]';
                    end
                else

                end
                if (init_flag == 1)
                    gyro_psi = -heading_est;
                    kf_psi = gyro_psi;
                    cent_pos_est = [real(IMUposU);imag(IMUposU);0];
                    cent_vel_est = [real(tag_center_vel_est);imag(tag_center_vel_est);0];
                end
            end
            if (min_stamp_i == 5) && (init_flag == 1)
                init_flag = 2;
            end

            TagPos = cent_pos_est(1)+j*cent_pos_est(2) + exp(j*(-kf_psi))*(tag_pos_b+0.4j);
            headig = -kf_psi;

        end
    end

    if mod(k,10) == 0
        figure(1);hold off;plot(cent_pos_est(1),cent_pos_est(2),'.');hold on;plot(xa,ya,'o');plot(real(TagPos(1:2)),imag(TagPos(1:2)),'ro');plot(real(TagPos(3:4)),imag(TagPos(3:4)),'ko');axis([-5 30 -5 20]);axis equal
    end

    headingTot(k) = -kf_psi;
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

figure(99);plot(headingTot)
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