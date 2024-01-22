Nav.mode = 0;

Nav.output.pos = True.pos(1,:)';
Nav.output.vel = zeros(3,1);

%% INS_Alignment
Nav.imu_align.time = 10; % [sec]    
Nav.imu_align.g_body = zeros(3,1);
Nav.imu_bias.acel = zeros(3,1);
Nav.imu_bias.gyro = zeros(3,1);


%% EKF Setting
Nav.KF.P = zeros(15,15);
Nav.KF.P(1:3,1:3) = 1 * eye(3,3);
Nav.KF.P(4:6,4:6) = 0.005 * eye(3,3);
Nav.KF.P(7:9,7:9) = 1.0e-03 * 0.4575 * eye(3,3);
Nav.KF.P(10:12,10:12) = 1.0e-05 * eye(3,3);
Nav.KF.P(13:15,13:15) = 1.0e-05 * eye(3,3);

Nav.KF.Q = zeros(6,6);
Nav.KF.Q(1:3,1:3) = diag(1.0e-01*ones(1,3));
Nav.KF.Q(4:6,4:6) = diag(1.0e-02*ones(1,3));

Nav.KF.R_GPS = zeros(6,6);
Nav.KF.R_GPS(1:3,1:3) = diag(500000*[1,1,1]);
Nav.KF.R_GPS(4:6,4:6) = diag(1e-2*ones(1,3));

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