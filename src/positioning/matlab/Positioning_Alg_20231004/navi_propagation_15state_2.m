function [Nav] = navi_propagation_15state_2( Nav, IMU, i )

    dt = IMU.dt;
    
    P = Nav.KF.P(1:15,1:15);
    
    a_raw = IMU.acc(:,1);
    w_raw = IMU.gyro(:,1);
        
    p_ned = Nav.output.pos;
    v_ned = Nav.output.vel;
    x_att = Nav.output.att;
    
    C_b2n = Nav.output.C_b2n;
    C_n2b = Nav.output.C_n2b;    
    
    b_acel = Nav.imu_bias.acel;
    b_gyro = Nav.imu_bias.gyro;
    
   
    % Real State Propagation
    p_ned = p_ned + v_ned*dt + ((C_b2n*(a_raw - b_acel) - [0; 0; -Nav.gravity])*dt^2)/2;
    v_ned = v_ned + (C_b2n*(a_raw - b_acel) - [0; 0; -Nav.gravity])*dt;


    %%
    x_att = Qua_Update(x_att,ang2qua((w_raw - b_gyro)*dt));

    %% sh_v2
%     x_att = Qua_Update(x_att,ang2qua([phi; theta; psi]));

    % naviation frame
    A = zeros(15,15);
    
    A(1:3,1:3) = eye(3,3);
    A(1:3,4:6) = eye(3,3)*dt;
    A(1:3,7:9) = 0.5*left_cross(C_b2n*(a_raw - b_acel))*dt^2;
    A(1:3,10:12) = -0.5*C_b2n*dt^2;
    

    A(4:6,4:6) = eye(3,3);
    A(4:6,7:9) = left_cross(C_b2n*(a_raw - b_acel))*dt;
    A(4:6,10:12) = -C_b2n*dt; 
    
        
    % b-Frame Attitude
    A(7:9,7:9) = eye(3,3);
    A(7:9,13:15) = -C_b2n*dt; 
        
    % Bias & Gravity   
    A(10:12,10:12) = eye(3,3);
    A(13:15,13:15) = eye(3,3);
    

    B = zeros(15,6);
    B(1:3,1:3) = -0.5*C_b2n*dt^2;
    B(1:3,4:6) = zeros(3,3); 
    B(4:6,1:3) = -C_b2n*dt; 
    B(4:6,4:6) = zeros(3,3); 
    
    B(7:9,4:6) = -C_b2n*dt;  

    P = A*P*A' + B*Nav.KF.Q*B';
    
    
    Nav.output.pos = p_ned;
    Nav.output.vel = v_ned;    
    Nav.output.att = x_att;

    Nav.output.C_b2n = qua2dcm(x_att);
    Nav.output.C_n2b = C_b2n';

    Nav.KF.P(1:15,1:15) = P;    
%     Nav.KF.Pinv = inv(P);

    %% sh_v1 (20220801)
%     Nav.output.pos_new = p_ned_new;
%     Nav.output.vel_new = v_ned_new;
%     Nav.output.est_orientation = q;
%     Nav.output.est_orientation_2 = q_minus_accel;
end