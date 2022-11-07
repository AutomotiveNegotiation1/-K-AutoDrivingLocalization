function [Nav] = navi_propagation_15state_2( Nav, IMU, i )

    dt = IMU.dt;
    
    P = Nav.KF.P(1:15,1:15);
    
    a_raw = IMU.acc(:,1);
    w_raw = IMU.gyro(:,1);
        
    p_ned = Nav.output.pos;
%     p_ned = [p_ned_t(2) -p_ned_t(1) p_ned_t(3)]';
    v_ned = Nav.output.vel;
    x_att = Nav.output.att;
%     x_att_eul = quat2eul(x_att','XYZ');
    
    C_b2n = Nav.output.C_b2n;
    C_n2b = Nav.output.C_n2b;    
    
    b_acel = Nav.imu_bias.acel;
    b_gyro = Nav.imu_bias.gyro;
    
   
    % Real State Propagation
    p_ned = p_ned + v_ned*dt + ((C_b2n*(a_raw - b_acel) - [0; 0; -Nav.gravity])*dt^2)/2;
    v_ned = v_ned + (C_b2n*(a_raw - b_acel) - [0; 0; -Nav.gravity])*dt;
%     temp_dist = ((C_b2n*(a_raw - b_acel)));
%     temp = [-temp_dist(2);temp_dist(1);temp_dist(3)];
%     p_ned = p_ned + v_ned*dt + temp*dt^2/2;
%     v_ned = v_ned + temp*dt;

    %% sh_v1 (gravity test)
%     Nav.gravity = norm(Nav.imu_align.g_body);
%     att_hor = inner_angle(Nav.imu_align.g_body/Nav.gravity, [0;0;-1]);
%     Nav.output.att = Qua_Update([1 0 0 0]',att_hor);

%     figure(22); hold on; plot(i, Nav.gravity, 'r.');

    %% sh_v2
%     w_raw_tmp = w_raw - b_gyro;
%     a_raw_tmp = a_raw - b_acel;
%     [phi, theta, psi] = CompFilterWithPI(w_raw_tmp(1), w_raw_tmp(2), w_raw_tmp(3), a_raw_tmp(1), a_raw_tmp(2), a_raw_tmp(3), dt);

    %% sh_v1 (20220801)
%     p_ned_new = Nav.output.pos_new;
%     v_ned_new = Nav.output.vel_new;
% 
%     % IMU filter
%     decim = 1;
%     Fs = 1/dt;
%     fuse = imufilter('SampleRate', Fs, 'DecimationFactor', decim);
% 
%     q = fuse(a_raw', w_raw');
%     q_minus_accel = fuse((a_raw - b_acel)', (w_raw - b_gyro)');
%     [qa,qb,qc,qd] = parts(q_minus_accel);
%     dcm_sh_v1 = qua2dcm([qa,qb,qc,qd]);
% 
%     p_ned_new = p_ned_new + v_ned_new*dt + ((dcm_sh_v1*(a_raw - b_acel) - [0; 0; -Nav.gravity])*dt^2)/2;
%     v_ned_new = v_ned_new + (dcm_sh_v1*(a_raw - b_acel) - [0; 0; -Nav.gravity])*dt;

%     sin_theta = sin(x_att_eul(1));
%     cos_theta = cos(x_att_eul(1));
%     sin_pi    = sin(x_att_eul(2));
%     cos_pi    = cos(x_att_eul(2));

%     g_att = Nav.gravity * [sin_theta; -cos_theta*sin_pi; -cos_theta*cos_pi];
% 
%     prev_mov_accel_max = [0 a_raw(3) -a_raw(2); -a_raw(3) 0 a_raw(1); a_raw(2) -a_raw(1) 0];
%     mov_accel = b_acel - prev_mov_accel_max*w_raw + g_att;
%     v_ned_new = v_ned_new + mov_accel * dt;
%     p_ned_new = p_ned_new + v_ned_new*dt;

%     figure(11); hold on; subplot(4,1,mov_accel)

    
    %%
%     x_att = Qua_Update(x_att,ang2qua((w_raw - b_gyro)*dt));
    p = w_raw(1)-b_gyro(1);
    q = w_raw(2)-b_gyro(2);
    r = w_raw(3)-b_gyro(3);
    [GyroEuler] = dcm2eulr(qua2dcm(x_att));
    [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,GyroEuler(1),GyroEuler(2),GyroEuler(3));
    x_att = EulerToQuaternion(gyro_phi,gyro_theta,gyro_psi);
    
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
    
    
%     Nav.output.pos = [p_ned(2);-p_ned(1);p_ned(3)];
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