function [IMUposU, b_acc_o, kf_psi, gyro_psi, cent_pos_est, cent_vel_est] = fusion3(kl,k0, tag_pos_est, tag_center_vel_est, cent_pos_est, cent_vel_est, b_acc_o, acc_b_phi, Acc_Pos, Acc_Vel, kalman_on, init_flag, UWBErrSum, kf_psi, gyro_psi, heading_est, acc_b_theta, IMUSel, Nanchor, state_IMU)
    persistent k UWB_LS_Pos UWB_M_Pos UWB_M_Vel kf_theta kf_phi IMUacc
    Lv = 10;
    if IMUSel == 1
        dt = 0.01;
    else
        dt = 0.03;
    end

    zt_b = 1.53;
    if (isempty(k))
        k = 0;
        kf_theta = 0;
        kf_phi = 0;
        IMUacc = zeros(2*Lv, 3);
        UWB_LS_Pos = zeros(3,2*Lv);
        UWB_M_Pos = zeros(3,2*Lv);
        UWB_M_Vel = zeros(3,2*Lv);
    end
    IMUposU = ((9*tag_pos_est(3)+tag_pos_est(1))+(9*tag_pos_est(4)+tag_pos_est(2)))/20;
    init_pos.x = real(IMUposU);
    init_pos.y = imag(IMUposU);
    init_pos.z = zt_b;
    
    if ((k0(5)>50))  % INS/UWB
        
    end
    
end