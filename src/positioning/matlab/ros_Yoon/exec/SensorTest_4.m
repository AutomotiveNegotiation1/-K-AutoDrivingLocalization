% clear all;
close all;

fname = 'rosbag\test_b1_line_20220803_v1.mat';
% fname = 'rosbag\test_o_line_20220729_v1.mat';

% load(fname,'Sens');

IMUacc = Sens.imu.acc;
IMUgyro = Sens.imu.gyro;
IMUtime = Sens.imu.time;
% IMUacc(:,2) = -IMUacc(:,2);
SensorTest_init;

for k = 1 : Sens.length
    if k>1
        dt = IMUtime(k)-IMUtime(k-1);
    else
        dt = 0.01;
    end
    acc_org(:,k) = IMUacc(k,:)';
    p = (IMUgyro(k,1)-b_gyro(1));    q = (IMUgyro(k,2)-b_gyro(2));    r = (IMUgyro(k,3)-b_gyro(3));
    acc_wo_bias(:,k) = acc_org(:,k)-acc_b_bias;
    acc_corrected(:,k) = acc_wo_bias(:,k)-norm(gv)*[sin(kf_phi);-cos(kf_phi)*sin(kf_theta);-cos(kf_phi)*cos(kf_theta)];
    
    [acc_phi,acc_theta] = EulerAcc(acc_wo_bias(1,k),acc_wo_bias(2,k),acc_wo_bias(3,k));
    
    %% KF for phi,theta    
    AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    qua_kf = EulerToQuaternion(kf_phi,kf_theta,kf_psi);
    qua_acc = EulerToQuaternion(acc_phi,acc_theta,0);
    [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
    
    %% velocity, position

    
    if k < 1000
        %% KF for acc bias
        x = [pos(:,k);vel(:,k);acc_org(:,k);acc_b_bias;gv];
        gv_n = norm(gv)*[sin(kf_phi);-cos(kf_phi)*sin(kf_theta);-cos(kf_phi)*cos(kf_theta)];
        zhat = [0;0;0;0;0;0];  %%% [pos;vel;acc_corrected];

        H = zeros(15,15);
%         H(1:9,:) = [1 0 0 dt 0 0 1/2*dt^2 0 0 -1/2*dt^2 0 0 -1/2*dt^2 0 0; %% pos+vel*dt+1/2dt^2*(acc_org - acc_b_bias - gv)
%                     0 1 0 0 dt 0 0 1/2*dt^2 0 0 -1/2*dt^2 0 0 -1/2*dt^2 0;
%                     0 0 1 0 0 dt 0 0 1/2*dt^2 0 0 -1/2*dt^2 0 0 -1/2*dt^2;
%                     0 0 0 1 0 0 dt 0 0 -dt 0 0 -dt 0 0;
%                     0 0 0 0 1 0 0 dt 0 0 -dt 0 0 -dt 0;
%                     0 0 0 0 0 1 0 0 dt 0 0 -dt 0 0 -dt;
%                     0 0 0 0 0 0 1 0 0 -1 0 0 -1 0 0;
%                     0 0 0 0 0 0 0 1 0 0 -1 0 0 -1 0;
%                     0 0 0 0 0 0 0 0 1 0 0 -1 0 0 -1];
        H(4:9,:) = [0 0 0 1 0 0 dt 0 0 -dt 0 0 -dt 0 0;
                    0 0 0 0 1 0 0 dt 0 0 -dt 0 0 -dt 0;
                    0 0 0 0 0 1 0 0 dt 0 0 -dt 0 0 -dt;
            0 0 0 0 0 0 1 0 0 -1 0 0 -1 0 0;
                    0 0 0 0 0 0 0 1 0 0 -1 0 0 -1 0;
                    0 0 0 0 0 0 0 0 1 0 0 -1 0 0 -1];
        Z = zhat - H(4:9,:)*x;  
        K = P*H'/(H*P*H'+R);
        P = P - K*H*P;
        err_X = zeros(15,1) + K(:,4:9)*Z;
        %         x = x + err_X;
        acc_b_bias = acc_b_bias+err_X(10:12,1);
        gv = gv+err_X(13:15,1);
        pos(:,k+1) = 1/2*dt^2*acc_corrected(:,k);
        vel(:,k+1) = acc_corrected(:,k)*dt;
    else
        
        %     kf_phi_m_t(k) = kf_phi_m;       kf_theta_m_t(k) = kf_theta_m;
        pos(:,k+1) = pos(:,k)+vel(:,k)*dt+1/2*dt^2*acc_corrected(:,k);
        vel(:,k+1) = vel(:,k)+acc_corrected(:,k)*dt;
    end
    
    kf_phi_t(k) = kf_phi;           kf_theta_t(k) = kf_theta;   kf_psi_t(k) = kf_psi;
    acc_phi_t(k) = acc_phi;         acc_theta_t(k) = acc_theta;
end

picnum = 0;
% figure(picnum);hold off;plot(kf_phi_m_t);hold on;plot(kf_phi_t,'m');plot(gyro_phi_t,'r');title('phi compare')
% picnum=picnum+1;figure(picnum);hold off;plot(gyro_phi_t);hold on;plot(gyro_theta_t,'m');plot(gyro_psi_t,'r');title('gyro euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(kf_phi_t);hold on;plot(kf_theta_t,'m');plot(kf_psi_t,'r');title('kf euler');legend('phi','theta','psi')
picnum=picnum+1;figure(picnum);hold off;plot(acc_phi_t);hold on;plot(acc_theta_t,'m');title('acc euler');legend('phi','theta')
picnum=picnum+1;figure(picnum);hold off;plot(acc_org(1,:));hold on;plot(acc_org(2,:),'r');plot(acc_org(3,:),'g');title('acc b')
picnum=picnum+1;figure(picnum);hold off;plot(acc_wo_bias(1,:));hold on;plot(acc_wo_bias(2,:),'r');plot(acc_wo_bias(3,:),'g');title('acc c')
picnum=picnum+1;figure(picnum);hold off;plot(acc_corrected(1,:));hold on;plot(acc_corrected(2,:),'r');plot(acc_corrected(3,:),'g');title('acc d')
picnum=picnum+1;figure(picnum);hold off;plot(vel(1,:));hold on;plot(vel(2,:),'m');plot(vel(3,:),'r');title('velocity')
picnum=picnum+1;figure(picnum);hold off;plot(pos(1,:));hold on;plot(pos(2,:),'m');plot(pos(3,:),'r');title('position')




picnum=picnum+1;figure(picnum);hold off;plot(acc_n(1,:));hold on;plot(acc_n(2,:),'r');plot(acc_n(3,:),'g');title('acc n')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b(1,:));hold on;plot(acc_b(2,:),'r');plot(acc_b(3,:),'g');title('acc b bias compansated')
picnum=picnum+1;figure(picnum);hold off;plot(IMUgyro(:,1)-b_gyro(1));hold on;plot(IMUgyro(:,2)-b_gyro(2),'r');title('gyro original - bias')







