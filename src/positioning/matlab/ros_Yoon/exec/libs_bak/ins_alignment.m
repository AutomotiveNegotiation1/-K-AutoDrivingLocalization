function [ Nav ] = ins_alignment(Nav, IMU, k, mode)

    alpha = (k-1) / k; % k=1~1001
    Nav.imu_bias.gyro = alpha*Nav.imu_bias.gyro + (1-alpha)*IMU.gyro;
%     Nav.imu_bias.acel = alpha*Nav.imu_bias.acel + (1-alpha)*IMU.acc;

    Nav.imu_align.g_body = alpha*Nav.imu_align.g_body + (1-alpha)*IMU.acc;
   
    if IMU.SimulationTime >= Nav.imu_align.time    % [sec]

        Nav.gravity = norm(Nav.imu_align.g_body);
        att_hor = inner_angle(Nav.imu_align.g_body/Nav.gravity, [0;0;1]);
        Nav.output.att = Qua_Update([1 0 0 0]',att_hor);
  
        Nav.output.C_b2n = qua2dcm(Nav.output.att);    
        Nav.output.C_n2b = Nav.output.C_b2n';
        
        Nav.mode = mode; 
        % Mode 1 = INS/GPS
        % Mode 2 = INS/LiDAR

        disp('10sec Alignment End');

    end
end