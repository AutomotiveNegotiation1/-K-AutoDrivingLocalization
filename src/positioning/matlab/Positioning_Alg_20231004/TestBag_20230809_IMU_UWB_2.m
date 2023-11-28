clear all;
close all;
% aa = rosbag('2023-07-26-18-35-13.bag');
% aa = rosbag('2023-08-07-11-03-21.bag');
% aa = rosbag('2023-08-07-14-34-54.bag');
aa = rosbag('20230809_driving_record_1.bag');

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

IMURoSV = select(aa,'Topic','/imu/angular_velocity');
IMUVMsg = readMessages(IMURoSV,'DataFormat','struct');

IMURoSA = select(aa,'Topic','/imu/acceleration');
IMUAMsg = readMessages(IMURoSA,'DataFormat','struct');

IMURoSM = select(aa,'Topic','/imu/mag');
IMUMMsg = readMessages(IMURoSM,'DataFormat','struct');

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

% b_acc_o =[ 0.0315    0.1288   -0.0798];
% b_acc_o =[ -0.2921+0.5-0.04    0.1928+0.1+0.05   -0.019-0.048];
b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original
% b_acc_o = [ -0.2927    0.1928   -0.0194];  %% original

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

while((length(UWBMsg{1})>=k0(1))&&(length(UWBMsg{2})>=k0(2))&&(length(UWBMsg{3})>=k0(3))&&(length(UWBMsg{4})>=k0(4))&&(length(IMUVMsg)>=k0(5)))
    k = k + 1;
    for lk = 1 : 4
        s_time_a(lk) = double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Sec)+double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Nsec)/10^9;
%         s_time_a(lk) = 10000000000000000000000;
    end
    s_time_a(lk+1) = double(IMUVMsg{k0(lk+1)}.Header.Stamp.Sec)+double(IMUVMsg{k0(lk+1)}.Header.Stamp.Nsec)/10^9;
    
    [min_stamp_v, min_stamp_i] = min(s_time_a);
    s_time = min_stamp_v;

    if min_stamp_i == 5
        IMUacc(1:end-1, :) = IMUacc(2:end, :);
        IMUacc(end,:) = [IMUAMsg{k0(5)}.Vector.X IMUAMsg{k0(5)}.Vector.Y IMUAMsg{k0(5)}.Vector.Z]*1;
        IMUgyro(1:end-1, :) = IMUgyro(2:end, :);
        IMUgyro(end,:)  = [IMUVMsg{k0(5)}.Vector.X IMUVMsg{k0(5)}.Vector.Y IMUVMsg{k0(5)}.Vector.Z];

%         dt = s_time - s_time_prev;
%         s_time_prev = s_time;
        dt = 0.01;
        

        if k0(5)>50
            p = (IMUgyro(end,1)-b_gyro(1));    q = (IMUgyro(end,2)-b_gyro(2));    r = (IMUgyro(end,3)-b_gyro(3));
        %         p = (IMUgyro(end,1)-b_gyro(1))*pi/180;    q = (IMUgyro(end,2)-b_gyro(2))*pi/180;    r = (IMUgyro(end,3)-b_gyro(3))*pi/180;
            acc_b = (IMUacc(end,:)-b_acc_o)';
            used_phi = kf_phi;
            used_theta = kf_theta;
            used_psi = kf_psi;

            used_phi_next = kf_phi;
            used_theta_next = kf_theta;
            used_psi_next = kf_psi;

            Acc_Pos(:,1:end-1) = Acc_Pos(:,2:end);
            Acc_Vel(:,1:end-1) = Acc_Vel(:,2:end);
            Acc_PQR(:,1:end-1) = Acc_PQR(:,2:end);
            Acc_Steady(:,1:end-1) = Acc_Steady(:,2:end);
            Acc_Acc(:,1:end-1) = Acc_Acc(:,2:end);

            %             vel_t = rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,end-1);
            %             Acc_PQR(:,end) = 1*([0 vel_t(3) -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]);
            %             Acc_Steady(:,end) = gravity*[sin(used_theta_next);-cos(used_theta_next)*sin(used_phi_next);-cos(used_theta_next)*cos(used_phi_next)];
            %Acc_Acc(:,end) = rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b-Acc_PQR(:,end)-Acc_Steady(:,end));
            Acc_Acc(:,end) = [0 1 0;1 0 0;0 0 1]*rotationVectorToMatrix([0;0;used_psi])'*([acc_b(1);acc_b(2);0]);
            %             Acc_Acc(:,end) = rotationVectorToMatrix([used_phi;used_theta;used_psi])'*([acc_b(1);-acc_b(2);acc_b(3)]-Acc_PQR(:,end)-Acc_Steady(:,end));

            [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);

            [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));

            qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, gyro_psi);
            qua_kf = EulerToQuaternion(gyro_phi,gyro_theta,gyro_psi);
            AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
            %
            [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
            kf_phi =real(kf_phi);
            kf_theta =real(kf_theta);
            kf_psi =real(kf_psi);

            Acc_Vel(:,end) = Acc_Vel(:,end-1) + Acc_Acc(:,end)*dt;
            Acc_Pos(:,end) = Acc_Pos(:,end-1) + Acc_Vel(:,end-1)*dt + Acc_Acc(:,end)*dt^2/2;

        elseif k0(5)== 50

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

            Tasg = double(int32(abs(mod(-headingest_a_aver_v,2*pi) - kf_psi)/(2*pi)));
            if mod(-headingest_a_aver_v,2*pi) - kf_psi > pi
                headingest_a_aver_vm = mod(-headingest_a_aver_v,2*pi)-2*pi*Tasg;
            else
                headingest_a_aver_vm = mod(-headingest_a_aver_v,2*pi)+2*pi*Tasg;
            end

            gyro_psi = headingest_a_aver_vm;
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
            gyro_psi = pi;
            %             b_gyro = b_gyro*(k0(5)-1)/k0(5)+IMUgyro(end,:)/k0(5);
        end

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
        %RxDist = RxDistOrig;

        Nanchor = length(RxID);
        if Nanchor > 1
            [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v] = UWBpos(Ln, Lp, PP, Nanchor, RxID, RxDist, s_time, tag_pos_b, xa, ya);
            Reliability = ReliCalc(tag_pos_est, xa+j*ya, RxDist, RxID, PP)

            RxIDT{k} = RxID;
            RxTime(k,PP) = s_time;
            RxIDTot(k,PP) = length(RxID);

            tag_pos_est_total{k} = tag_pos_est;
            center_pos_est_total(k) = mean(tag_pos_est);
            headingest_total(k) = heading_est;
            center_pos_est_aver_total(k) = mean(tag_pos_est_aver);
            headingest_aver_total(k) = headingest_a_aver_v;
            k0(PP) = k0(PP) + 1;
            RxDistOrigTot(k,RxID,PP)=RxDistOrig;
            RxDistTot(k,RxID,PP)=real(sqrt(RxDist.^2-zt_b^2));

            IMUposU = ((9*tag_pos_est(3)+tag_pos_est(1))+(9*tag_pos_est(4)+tag_pos_est(2)))/20;
            init_pos.x = real(IMUposU);
            init_pos.y = imag(IMUposU);
            init_pos.z = zt_b;

            if ((k0(5)>50))  % INS/UWB
                UWB_LS_Pos(:,1:end-1) = UWB_LS_Pos(:,2:end);
                UWB_LS_Pos(:,end) = [real(IMUposU) ;imag(IMUposU); zt_b] ;
                %                  UWB_LS_Pos(:,1:end-1) = UWB_LS_Pos(:,2:end);
                %                 UWB_LS_Pos(:,end) = [real(mean(tag_pos_est)) ;imag(mean(tag_pos_est)); zt_b] ;

                Tasg = double(int32(abs(mod(-headingest_a_aver_v,2*pi) - kf_psi)/(2*pi)));
                if mod(-headingest_a_aver_v,2*pi) - kf_psi > pi
                    headingest_a_aver_vm = mod(-headingest_a_aver_v,2*pi)-2*pi*Tasg;
                else
                    headingest_a_aver_vm = mod(-headingest_a_aver_v,2*pi)+2*pi*Tasg;
                end
                %             gyro_psi = -headingest_a_aver_vm;
                qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, headingest_a_aver_vm);
                % %             qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, gyro_psi);
                qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);

                %             AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
                AA = eye(4);
                %               AA = eye(6);
                %               AA(1:3,4:6) = -1*eye(1);
                %               R_h = 1e-1*eye(6);
                
                if (RxIDTot(k,PP) == 4) && (Reliability>2)
                    [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
                
                %              [xout] = HeadingKalman([kf_phi;kf_theta;kf_psi], AA, [acc_b_phi;acc_b_theta;headingest_a_aver_vm;b_head(1);b_head(2);b_head(3)], R_h);
                %              kf_phi = xout(1);
                %              kf_theta = xout(2);
                %              kf_psi = xout(3);
                %              b_head(1) = xout(4);
                %              b_head(2) = xout(5);
                %              b_head(3) = xout(6);

                kf_phi =real(kf_phi);
                kf_theta =real(kf_theta);
                kf_psi =real(kf_psi);
                gyro_psi = kf_psi;
                end
                %             kf_psi = headingest_a_aver_v;

                if (kl > Lv*2)
                    Ta = mean(UWB_LS_Pos(:,end-Lv+1:end)')';
                    Tb = mean(UWB_LS_Pos(:,end-2*Lv+1:end-Lv)')';
                    Tm = (Ta+Tb)/2;
                    Td = Ta-Tm;
                    UWB_M_Pos(:,1:end-1) = UWB_M_Pos(:,2:end);
                    UWB_M_Vel(:,1:end-1) = UWB_M_Vel(:,2:end);
                    UWB_M_Pos(:,end) = (Tm+Td);
                    UWB_M_Vel(:,end) = (UWB_M_Pos(:,end)-UWB_M_Pos(:,end-9))/1;
                else
                    UWB_M_Pos(:,1:end-1) = UWB_M_Pos(:,2:end);
                    UWB_M_Vel(:,1:end-1) = UWB_M_Vel(:,2:end);

                    UWB_M_Pos(:,end) = [init_pos.x init_pos.y init_pos.z]';
                    UWB_M_Vel(:,end) = [0 0 0]';
                end

%                 R = 1e0*eye(6);
                if (RxIDTot(k,PP) == 4) && (Reliability>2)
                    R = 1e-2*eye(6);
                elseif (RxIDTot(k,PP) == 3) && (Reliability>2)
                    R = 0.5e1*eye(6);
                else
                    R = 1e1*eye(6);
                end

                %TEMP = [[IMUacc(end,1);IMUacc(end,2);IMUacc(end,3)]-Acc_Steady(:,end)];
                TEMP = [[IMUacc(end,1);IMUacc(end,2);0]];
                %             TEMP = [IMUacc(end,:)'-Acc_Steady(:,end)];
                TEMP_bias = [b_acc_o(1);b_acc_o(2);b_acc_o(3)];
                x = [Acc_Pos(:,end-1);Acc_Vel(:,end-1);TEMP;TEMP_bias];

                Cb2n = [0 1 0;1 0 0;0 0 1]*rotationVectorToMatrix([0;0;used_psi])';
                %Cb2n = rotationVectorToMatrix([used_phi;used_theta;used_psi])';
                
                A = eye(12);
                A(1:3,4:6) = dt*eye(3)-1/2*dt^2*[0 -r q;r 0 -p;-q p 0];
                A(1:3,7:9) = Cb2n*dt^2/2;
                A(1:3,10:12) = -Cb2n*dt^2/2;
                A(4:6,4:6) = A(4:6,4:6) - dt*[0 -r q;r 0 -p;-q p 0];
                A(4:6,7:9) = Cb2n*dt;
                A(4:6,10:12) = -Cb2n*dt;


                z = [UWB_M_Pos(:,end);UWB_M_Vel(:,end)];

                x_n = PosKalman(x,A,z,R);

                Acc_Pos(:,end) = x_n(1:3,1);

                Acc_Vel(:,end) = x_n(4:6,1);
%                 acc_g = x_n(7:9,1);
%                 b_acc_o = x_n(10:12,1)';
                kl = kl + 1;

                Acc_S = [0 0 0]';
            end
        else
            k0(PP) = k0(PP) + 1;
        end
    end
    TagPos = Acc_Pos(1,end)+j*Acc_Pos(2,end) + exp(j*(-kf_psi))*(tag_pos_b+0.4j);
    %     TagPos = Acc_Pos(1,end)+j*Acc_Pos(2,end) + exp(j*heading_est)*(tag_pos_b+0.4j);
    %     TagPos = Acc_Pos(1,end)+j*Acc_Pos(2,end) + exp(j*headingest_a_aver_v)*tag_pos_b;

        figure(1);hold off;plot(Acc_Pos(1,end),Acc_Pos(2,end),'.');hold on;plot(xa,ya,'o');plot(real(TagPos(1:2)),imag(TagPos(1:2)),'ro');plot(real(TagPos(3:4)),imag(TagPos(3:4)),'ko');axis([-5 30 -5 20])
        if min_stamp_i < 5
            
            if (RxIDTot(end,PP) == 4 ) && (Reliability>2)
                figure(1);plot(real(tag_pos_est),imag(tag_pos_est),'b*')
            elseif (RxIDTot(end,PP) == 3 ) && (Reliability>2)
                figure(1);plot(real(tag_pos_est),imag(tag_pos_est),'m*')
            else
                figure(1);plot(real(tag_pos_est),imag(tag_pos_est),'r*')
            end

        end

    TagPosTot(k) = mean(TagPos);
end
for qik = 1 : 4
    [val,indx] = find(RxTime(:,qik)~=0);
    RxTimeTag{qik} = RxTime(val,qik);
    RxIDTotTag{qik} = RxIDTot(val,qik);
end

figure(33);hold off;plot(center_pos_est_total(40+1:end),'.')
figure(33);hold on;plot(center_pos_est_aver_total(40+1:end),'r.')
figure(33);hold on;plot(xa,ya,'b*');axis equal

figure(34);hold off;plot(TagPosTot(40+1:end),'.')
figure(34);hold on;plot(xa,ya,'b*');axis equal

figure(35);hold off;plot(mod(headingest_total(40+1:end),2*pi)*180/pi)
figure(35);hold on;plot(mod(headingest_aver_total(40+1:end),2*pi)*180/pi,'r')

An = xa + j*ya;
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

