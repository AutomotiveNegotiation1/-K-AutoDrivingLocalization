function [cent_pos_est, cent_vel_est, kf_psi, IMUposU, acc_b_phi, tag_center_vel_est] = fusion(tag_center_vel_est, state_IMU, Nanchor, b_acc_o, acc_b_theta, acc_b_phi, UWBErrSum, init_flag, kalman_on, k0, cent_pos_est, cent_vel_est, kf_psi, tag_pos_est, heading_est, zt_b)
Lv = 10;
dt = 0.01;
IMUSel = 2;

init_pos.x = 0;
init_pos.y = 0;
init_pos.z = 0;

persistent k Acc_Vel Acc_Pos IMUacc UWB_M_Pos UWB_M_Vel kf_theta kf_phi kl UWB_LS_Pos

if isempty (k)
    k = 0;
    Acc_Vel = zeros(3,2*Lv);
    Acc_Pos = zeros(3,2*Lv);
    IMUacc = zeros(2*Lv, 3);
    UWB_M_Pos = zeros(3,2*Lv);
    UWB_M_Vel = zeros(3,2*Lv);
    kf_theta = 0;
    kf_phi = 0;
    kl = 1;
    UWB_LS_Pos = zeros(3,2*Lv);
end

%% Initialization
if k < 1000
    k = k + 1;
end


IMUposU = ((9*tag_pos_est(3)+tag_pos_est(1))+(9*tag_pos_est(4)+tag_pos_est(2)))/20;
init_pos.x = real(IMUposU);
init_pos.y = imag(IMUposU);
init_pos.z = zt_b;

if ((k0(5)>50))  % INS/UWB
    UWB_LS_Pos(:,1:end-1) = UWB_LS_Pos(:,2:end);
    UWB_LS_Pos(:,end) = [real(IMUposU) ;imag(IMUposU); zt_b] ;
    %
    %                 if (RxIDTot(k,PP) >= 3) && (Reliability>2) && (kalman_on)
    if (kalman_on == 1) && (init_flag > 2)  && (UWBErrSum <0.5)%&& (Nanchor>2) && (Reliability >2)

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

    if (kalman_on == 1) && (init_flag > 2) && (UWBErrSum <1)
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
end