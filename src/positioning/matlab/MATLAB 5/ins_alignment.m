function [ Nav ] = ins_alignment(Nav, IMU, k, mode)

    alpha = (k-1) / k; % k=1~1001
    Nav.imu_bias.gyro = alpha*Nav.imu_bias.gyro + (1-alpha)*IMU.gyro;
    Nav.imu_align.g_body = alpha*Nav.imu_align.g_body + (1-alpha)*IMU.acc;
    Nav.imu_bias.acel = Nav.imu_align.g_body;
%     Nav.imu_bias.acel = [0 0 0]';
   
    if IMU.SimulationTime >= Nav.imu_align.time    % [sec]
        Nav.gravity = norm(Nav.imu_align.g_body);
        [phi_bias, theta_bias] = EulerAcc(Nav.imu_align.g_body(1), Nav.imu_align.g_body(2), Nav.imu_align.g_body(3));
        Nav.imu_bias.Cacc_bias = qua2dcm(EulerToQuaternion(phi_bias, theta_bias, 0));
        Nav.imu_bias.phi_bias = phi_bias;
        Nav.imu_bias.theta_bias = theta_bias;
        Nav.imu_bias.gyro_bias = Nav.imu_bias.gyro;
        
        Nav.gravity = norm(Nav.imu_align.g_body);
        att_hor = inner_angle(Nav.imu_align.g_body/Nav.gravity, [0;0;-1]);
        Nav.output.att = Qua_Update([1 0 0 0]',att_hor);
%         temp = EulerToQuaternion(0,0,-pi/2);
%         Nav.output.att = Qua_Update(temp,att_hor);
%             
%         Nav.output.C_b2n = qua2dcm(Nav.output.att);    
%         Nav.output.C_n2b = Nav.output.C_b2n';
         Nav.output.C_b2n = qua2dcm(Nav.output.att)';    
        Nav.output.C_n2b = Nav.output.C_b2n';       
        Nav.mode = mode; 
        % Mode 1 = INS/GPS
        % Mode 2 = INS/LiDA
%         Nav.output.qua_gyro = temp;
        Nav.output.qua_gyro = [1 0 0 0]';
        
        disp('10sec Alignment End');

    end
end