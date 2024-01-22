function [Nav] = ins_position_3(Nav, IMU, NaviMode)

persistent init_first
persistent firstRun
persistent vx_e vy_e vz_e
persistent ax_e ay_e az_e
persistent pos flip gyro_bias
persistent k acc_e v_e att_hor
persistent acc_vec qua_gyro Cacc_bias
persistent theta_bias phi_bias psi_bias
use_kf = 1;

if isempty(firstRun)
    init_first = 0;
    flip = 0;
    firstRun = 1;
    vx_e = 0; vy_e = 0; vz_e = 0;
    ax_e = 0; ay_e = 0; az_e = 0;
    pos = [0 0 0]';
    v_e = [0 0 0]';
    k = 1;
    acc_e= [0 0 0]';
    att_hor = [1 0 0 0]';
    acc_vec = [0 0 0]';
    Nav.output.att = [1 0 0 0]';
    theta_bias = 0;
    phi_bias = 0;
    psi_bias = 0;
    qua_gyro = [1 0 0 0]';
    Cacc_bias = eye(3);
    gyro_bias = [0 0 0]';
end
alpha = (k-1) / k;
dt = IMU.dt;
% Nav.output.att = [1 0 0 0]';

if ((IMU.acc(3) > 9)&(init_first==0)) || (flip==1)
    p = (IMU.gyro(1)-gyro_bias(1));
    q = -(IMU.gyro(2)-gyro_bias(2));
    r = -(IMU.gyro(3)-gyro_bias(3));
    ax = IMU.acc(1);
    ay = -IMU.acc(2);
    az = -IMU.acc(3);
    flip = 1;
else
    p = IMU.gyro(1)-gyro_bias(1);
    q = IMU.gyro(2)-gyro_bias(2);
    r = IMU.gyro(3)-gyro_bias(3);
    ax = IMU.acc(1);
    ay = IMU.acc(2);
    az = IMU.acc(3);
end

if (IMU.SimulationTime >= Nav.imu_align.time)&(init_first==0)    % [sec]
    
    Nav.gravity = norm(Nav.imu_align.g_body);
    [phi_bias, theta_bias] = EulerAcc(Nav.imu_align.g_body(1), Nav.imu_align.g_body(2), Nav.imu_align.g_body(3));
    Cacc_bias = qua2dcm(EulerToQuaternion(phi_bias, theta_bias, 0));
    gyro_bias = Nav.imu_bias.gyro;
    
%     A = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
%     
%     qua_gyro = A*qua_gyro;
%     eurl_gyro= dcm2eulr(qua2dcm(qua_gyro));
%     
%     [phi, theta] = EulerAcc(ax, ay, az);
%     
    qua_gyro = [1 0 0 0]';
%     [euler_angle_p] = dcm2eulr(qua2dcm(qua_gyro));
%     
%     z = EulerToQuaternion(phi, theta, euler_angle_p(3));
%     [phi, theta, psi] = EulerKalman(A, z);
%     att_hor = EulerToQuaternion(phi-phi_bias, theta-theta_bias, psi);
%     C_b2n = Nav.output.C_b2n;
%     C_n2b = C_b2n';
    Nav.output.C_b2n = eye(3);
    Nav.output.C_n2b = eye(3)';
% %     att_hor = EulerToQuaternion(phi, theta, 0);
%     Nav.output.att = att_hor;
%     
%     qua_gyro = EulerToQuaternion(phi_bias, theta_bias, 0);
%     att_kf = [1 0 0 0]';
%     acc_e = ([Nav.imu_align.g_body(1) Nav.imu_align.g_body(2) Nav.imu_align.g_body(3)]'-Nav.gravity*[sin(theta);-cos(theta)*sin(phi);-cos(theta)*cos(phi)]);
%     acc_vec = acc_e;
    Nav.mode = NaviMode;

    init_first = 1;
elseif (IMU.SimulationTime < Nav.imu_align.time)

    Nav.imu_bias.gyro = alpha*Nav.imu_bias.gyro + (1-alpha)*[p q r]';
    Nav.imu_align.g_body = alpha*Nav.imu_align.g_body + (1-alpha)*[ax ay az]';
    Nav.output.C_b2n = eye(3);
    Nav.output.C_n2b = Nav.output.C_b2n';
    A = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    [phi, theta] = EulerAcc(ax, ay, az);
    qua_gyro = A* qua_gyro;
    [euler_angle_p] = dcm2eulr(qua2dcm(qua_gyro));

    z = EulerToQuaternion(phi, theta, euler_angle_p(3));
    [phi, theta, psi] = EulerKalman(A, z);

else
    A = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    a_b = Cacc_bias*[ax ay az]';
    ax = a_b(1); ay = a_b(2); az = a_b(3);
    
    qua_gyro = A*qua_gyro;
    eurl_gyro= dcm2eulr(qua2dcm(qua_gyro));

    [phi_temp, theta_temp] = EulerAcc(ax, ay, az);
    phi_org = phi_temp;
    theta_org = theta_temp;

    z = EulerToQuaternion(phi_org+phi_bias, theta_org+theta_bias, eurl_gyro(3));
    [phi, theta, psi] = EulerKalman(A, z);
    att_hor = EulerToQuaternion(phi-phi_bias, theta-theta_bias, psi);
    Nav.output.att = att_hor;
    C_b2n = Nav.output.C_b2n;
    C_n2b = Nav.output.C_b2n';

    Nav.output.C_b2n = qua2dcm(att_hor);
    Nav.output.C_n2b = Nav.output.C_b2n';
    
    v_b = C_n2b*[vx_e vy_e vz_e]';
    vx_b = v_b(1);vy_b=v_b(2);vz_b=v_b(3);
    a_e = C_b2n*[ax ay az]';
    ax_e = a_e(1);ay_e = a_e(2);az_e = a_e(3);
    
    [euler_angle] = dcm2eulr(qua2dcm(Nav.output.att));
%     phi = euler_angle(1);theta = euler_angle(2);psi = euler_angle(3);
%     acc_b = ([ax ay az]'-[0 vz_b -vy_b; -vz_b 0 vx_b;vy_b -vx_b 0]*[p;q;r]-Nav.gravity*[sin(theta);-cos(theta)*sin(phi);-cos(theta)*cos(phi)]);
    temp_b = Nav.gravity*[sin(theta);-cos(theta)*sin(phi);-cos(theta)*cos(phi)];
    temp_c = Cacc_bias*[0 vz_b -vy_b; -vz_b 0 vx_b;vy_b -vx_b 0]*[p;q;r];
%     temp_b = Nav.output.C_n2b*temp_e;
    acc_b = ([ax ay az]'+temp_c-temp_b);
    acc_e = Nav.output.C_b2n*acc_b;
    
    v_e = [vx_e;vy_e;vz_e]+acc_e*dt;
    pos = pos + [vx_e;vy_e;vz_e]*dt + 1/2*dt^2*acc_e;
    vx_e = v_e(1);vy_e = v_e(2);vz_e = v_e(3);
end
Nav.output.acc = acc_e;
Nav.output.pos = pos;
Nav.output.vel = v_e;    


k = k + 1;

