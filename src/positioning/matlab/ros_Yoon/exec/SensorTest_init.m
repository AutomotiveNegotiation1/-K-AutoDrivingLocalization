gyro_phi = 0;
gyro_theta = 0;
gyro_psi = 0;
gyro_delta_phi = 0;
gyro_delta_theta = 0;
gyro_delta_psi = 0;
kf_phi = 0;
kf_theta = 0;
kf_psi = 0;
% b_acc = mean(IMUacc);

% P = 0.1*[10 0 1 0;0 10 0 1;0 0 1 0;0 0 0 1];
% H = [eye(2) -eye(2);zeros(2,4)];
% R = [0.1 0 1 1;0 0.1 1 1;0 0 1 0;0 0 0 1];
% P = zeros(6,6);
% P(1:3,1:3) = eye(3);
% P(4:6,4:6) = eye(3);
% R = zeros(6,6);
% R(1:3,1:3) = eye(3);
% R(4:6,4:6) = eye(3);
% H = [eye(3) -eye(3);zeros(3,3) eye(3)];

    P = zeros(15,15);
%     P(1:3,1:3) = 1 * eye(3,3); % (위치) original
%     P(4:6,4:6) = 0.005 * eye(3,3); % (속도) original
%     P(7:9,7:9) = 1.0e-03 * 0.4575 * eye(3,3); % (가속도) original
%     P(10:12,10:12) = 1.0e-05 * eye(3,3); % (센서바이어스) original
%     P(13:15,13:15) = 1.0e-05 * eye(3,3); % (중력바이어스) original
    P = eye(15);
    R = 1*eye(15);
    R(10:15,10:15) = eye(6)*0.001;
    
    %%
    Q = zeros(9,9);
    Q(1:3,1:3) = diag(1.0e-01*ones(1,3)); % original
    Q(4:6,4:6) = diag(1.0e-02*ones(1,3)); % original
    Q(7:9,7:9) = diag(1.0e-02*ones(1,3)); % original
    
% b_acc = mean(IMUacc(1:100,:));
% b_acc = [0 0 0]';
% if(b_acc(3) > 9)
%     m_phi_bias = pi;
% else
%     m_phi_bias = 0;
% end
b_acc = mean(IMUacc(1:1000,:));

[acc_phi_bias,acc_theta_bias] = EulerAcc(b_acc(1), b_acc(2), b_acc(3));
% acc_phi_bias = acc_phi_bias + m_phi_bias;
Cbas = eulr2dcm([acc_phi_bias,acc_theta_bias,0]);

% UpSideDown = qua2dcm(EulerToQuaternion(m_phi_bias,0,0));

Cacc_bias = qua2dcm(EulerToQuaternion(acc_phi_bias, acc_theta_bias, 0));
b_gyro = mean(IMUgyro(1:1000,:));
pos = zeros(3,1); vel = zeros(3,1);

b_gyro_his = zeros(3,1);
l = 100;
m = 100;
acc_g_bias = [0 0 0]';
b_acc_b = [0 0 0]';
acc_b_kf_wob = [0 0 0]';
Cb2n_kf = eye(3);
kf_phi_m = 0;
kf_theta_m = 0;
kf_phi = 0;
kf_theta = 0;
acc_b_bias = [0 0 0]';
gv = [0 0 -9.8]';