function [kalman_on] = fusion2(kalman_on, imuNum, kf_psi, UWBErrSum, tag_pos_est, heading_est, zt_b)
Lv = 10;
init_pos.x = 0;
init_pos.y = 0;
init_pos.z = 0;

persistent k UWB_LS_Pos

if isempty (k)
    k = 0;
    UWB_LS_Pos = zeros(3,2*Lv);
end

IMUposU = ((9*tag_pos_est(3)+tag_pos_est(1))+(9*tag_pos_est(4)+tag_pos_est(2)))/20;
init_pos.x = real(IMUposU);
init_pos.y = imag(IMUposU);
init_pos.z = zt_b;

if ((k0(5)>50))  % INS/UWB
    UWB_LS_Pos(:,1:end-1) = UWB_LS_Pos(:,2:end);
    UWB_LS_Pos(:,end) = [real(IMUposU) ;imag(IMUposU); zt_b] ;

    if (kalman_on == 1) && (init_flag > 2)  && (UWBErrSum <0.5)
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
end
end