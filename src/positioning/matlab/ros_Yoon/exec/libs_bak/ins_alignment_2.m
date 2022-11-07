function [ Nav ] = ins_alignment_2(Nav, IMU, k, mode)

p = IMU.gyro(1);
q = IMU.gyro(2);
r = IMU.gyro(3);
dt = IMU.dt;
A = eye(4)+dt*1/2*[0 -p -q -r;
                   p 0 r -q;
                   q -r 0 p;
                   r q -p 0
                   ];
ax = IMU.acc(1);
ay = IMU.acc(2);
az = IMU.acc(3);
[phi, theta] = EulerAcc(ax, ay, az);
z = EulerToQuaternion(phi, theta, 0);

[phi, theta, psi] = EulerKalman(A, z);


    alpha = (k-1) / k; % k=1~1001
    Nav.imu_bias.gyro = alpha*Nav.imu_bias.gyro + (1-alpha)*IMU.gyro;
%     Nav.imu_bias.acel = alpha*Nav.imu_bias.acel + (1-alpha)*IMU.acc;

    Nav.imu_align.g_body = alpha*Nav.imu_align.g_body + (1-alpha)*IMU.acc;
   
    if IMU.SimulationTime >= Nav.imu_align.time    % [sec]

        Nav.gravity = norm(Nav.imu_align.g_body);
%         att_hor = inner_angle(Nav.imu_align.g_body/Nav.gravity, [0;0;1]);
        att_hor = EulerToQuaternion(phi, theta, psi);
        Nav.output.att = Qua_Update([1 0 0 0]',att_hor);
  
        Nav.output.C_b2n = qua2dcm(Nav.output.att);    
        Nav.output.C_n2b = Nav.output.C_b2n';
        
        Nav.mode = mode; 
        % Mode 1 = INS/GPS
        % Mode 2 = INS/LiDAR

        disp('10sec Alignment End');

    end
end