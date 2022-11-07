function [Nav] = navi_propagation_15state_4( Nav, IMU, i )

    dt = IMU.dt;
    
    P = Nav.KF.P(1:15,1:15);

    a_raw = IMU.gyro(:,1);
    p_ned = Nav.output.pos;
    v_ned = Nav.output.vel;
    x_att = Nav.output.att;
    
    Cacc_bias = Nav.imu_bias.Cacc_bias;
    phi_bias = Nav.imu_bias.phi_bias;
    theta_bias = Nav.imu_bias.theta_bias;
    
    p = (IMU.gyro(1)-Nav.imu_bias.gyro_bias(1));
    q = (IMU.gyro(2)-Nav.imu_bias.gyro_bias(2));
    r = (IMU.gyro(3)-Nav.imu_bias.gyro_bias(3));
    
    ax = IMU.acc(1,1);
    ay = IMU.acc(2,1);
    az = IMU.acc(3,1);
    
    gyro_bias = Nav.imu_bias.gyro;
    a_b = Cacc_bias*[ax ay az]';
    ax = a_b(1); ay = a_b(2); az = a_b(3);
    
    AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    Nav.output.qua_gyro = AA*Nav.output.qua_gyro;
    eurl_gyro= dcm2eulr(qua2dcm(Nav.output.qua_gyro));
    
    [phi_org, theta_org] = EulerAcc(ax, ay, az);

    z = EulerToQuaternion(phi_org+phi_bias, theta_org+theta_bias, eurl_gyro(3));
    [phi, theta, psi] = EulerKalman(AA, z);
    att_hor = EulerToQuaternion(phi-phi_bias, theta-theta_bias, psi);
    Nav.output.att = att_hor;
    C_b2n = Nav.output.C_b2n;
    C_n2b = Nav.output.C_b2n';
    
    Nav.output.C_b2n = qua2dcm(att_hor);
    Nav.output.C_n2b = Nav.output.C_b2n';
    
    v_b = C_n2b*v_ned;
    vx_b = v_b(1);vy_b=v_b(2);vz_b=v_b(3);
    a_e = C_b2n*[ax ay az]';
    ax_e = a_e(1);ay_e = a_e(2);az_e = a_e(3);
    
    temp_b = Nav.gravity*[sin(theta);-cos(theta)*sin(phi);-cos(theta)*cos(phi)];
    temp_c = Cacc_bias*[0 vz_b -vy_b; -vz_b 0 vx_b;vy_b -vx_b 0]*[p;q;r];
    acc_b = ([ax ay az]'-temp_c-temp_b);
    acc_e = Nav.output.C_b2n*acc_b;

    p_ned = p_ned + v_ned*dt + 1/2*dt^2*acc_e;
    v_ned = v_ned+acc_e*dt;

    A = zeros(15,15);
    
    A(1:3,1:3) = eye(3,3);
    A(1:3,4:6) = eye(3,3)*dt;
    A(1:3,7:9) = 0.5*left_cross(Cacc_bias*C_b2n*(a_raw))*dt^2;
    A(1:3,10:12) = -0.5*C_b2n*dt^2;
    

    A(4:6,4:6) = eye(3,3);
    A(4:6,7:9) = left_cross(Cacc_bias*C_b2n*(a_raw))*dt;
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
    Nav.output.acc = acc_e;
    Nav.output.C_b2n = qua2dcm(x_att);
    Nav.output.C_n2b = C_b2n';

    Nav.KF.P(1:15,1:15) = P;    


end