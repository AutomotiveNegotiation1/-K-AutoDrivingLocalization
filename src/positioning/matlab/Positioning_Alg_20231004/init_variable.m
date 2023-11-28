function [ Nav, Grp ] = init_variable()

    Nav.mode = 0;

    Nav.output.pos = zeros(3,1);
    Nav.output.vel = zeros(3,1);

    %% sh_v1 (20220801)
    Nav.output.pos_new = zeros(3,1);
    Nav.output.vel_new = zeros(3,1);
    Nav.output.est_orientation = zeros(3,1);
    Nav.output.est_orientation_2 = zeros(3,1);

    %% INS_Alignment
    Nav.imu_align.time = 10; % [sec]    
    Nav.imu_align.g_body = zeros(3,1);
    Nav.imu_bias.acel = zeros(3,1);
    Nav.imu_bias.gyro = zeros(3,1);


    %% EKF Setting
    Nav.KF.P = zeros(15,15);
    Nav.KF.P(1:3,1:3) = 1 * eye(3,3); % (위치) original
    Nav.KF.P(4:6,4:6) = 0.005 * eye(3,3); % (속도) original
    Nav.KF.P(7:9,7:9) = 1.0e-03 * 0.4575 * eye(3,3); % (자세) original
    Nav.KF.P(10:12,10:12) = 1.0e-05 * eye(3,3); % (3축 가속도) original
    Nav.KF.P(13:15,13:15) = 1.0e-05 * eye(3,3); % (3축 각속도) original
   
    %%
    Nav.KF.Q = zeros(6,6);
    Nav.KF.Q(1:3,1:3) = diag(1.0e-01*ones(1,3)); % original
    Nav.KF.Q(4:6,4:6) = diag(1.0e-02*ones(1,3)); % original


    Nav.KF.R_GPS = zeros(6,6);
    Nav.KF.R_GPS(1:3,1:3) = diag(500000*[1,1,1]); % GPS 위치에 대한 가중치
    
    Nav.KF.R_GPS(4:6,4:6) = diag(1e-2*ones(1,3)); % GPS 속도에 대한 가중치

    Nav.KF.R_Map = zeros(6,6);
    Nav.KF.R_Map(1:3,1:3) = 0.1 * eye(3,3);
    Nav.KF.R_Map(4:6,4:6) = 0.1 * eye(3,3);

    Nav.KF.R_UWB = zeros(4,4);
    Nav.KF.R_UWB = 1 * eye(4,4);


    %% Results
    Grp.Nav.pos = zeros(1,3);
    Grp.Nav.vel = zeros(1,3);
    Grp.Nav.att = zeros(1,4);
    Grp.Nav.eulr = zeros(1,3);
    Grp.Nav.time = zeros(2,1);
    Grp.Nav.mode = zeros(2,1);
    Grp.Nav.cov = zeros(1,15);
    Grp.beta = zeros(1,3);  
    Grp.Nav.eulr_2 = zeros(1,3);
    Grp.Nav.eulr_3 = zeros(1,3);

end