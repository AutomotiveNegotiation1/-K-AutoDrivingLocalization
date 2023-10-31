function [Nav] = navi_gps_update(Nav, GPS)


    P = Nav.KF.P(1:15,1:15);

    p_ned = Nav.output.pos;
    v_ned = Nav.output.vel;
    x_att = Nav.output.att;
    
    C_b2n = Nav.output.C_b2n;
    C_n2b = Nav.output.C_n2b;    
    
    b_acel = Nav.imu_bias.acel;
    b_gyro = Nav.imu_bias.gyro;
       
    
    H = zeros(6,15);
    H(1:3,1:3) = eye(3,3);
    H(4:6,4:6) = eye(3,3);
    
    R = Nav.KF.R_GPS;
    Z = [GPS.pos_ned; GPS.vel_ned] - [p_ned; v_ned];
    
    
%     %%% Altitude
%     H(end+1,3) = 1;
%     R(end+1,end+1) = 0.01;
%     Z(end+1) = 0 - p_ned(3);
    
    
    K = P*H'/(H*P*H'+R);
    P = P - K*H*P;                
    err_X = zeros(15,1) + K*Z;
    
    
   
    p_ned = p_ned + err_X(1:3);
    v_ned = v_ned + err_X(4:6);
    x_att = Qua_Update(ang2qua(err_X(7:9)),x_att);
    
    
    Nav.output.pos = p_ned;
    Nav.output.vel = v_ned;    
    Nav.output.att = x_att;
    
    Nav.output.C_b2n = qua2dcm(Nav.output.att);
    Nav.output.C_n2b = Nav.output.C_b2n';
    
    b_acel = b_acel + err_X(10:12);
    Nav.imu_bias.acel = b_acel;
    
    b_gyro = b_gyro + err_X(13:15);
    Nav.imu_bias.gyro = b_gyro;
                
    
    Nav.KF.P(1:15,1:15) = P;
    Nav.output.x = err_X;
 
end