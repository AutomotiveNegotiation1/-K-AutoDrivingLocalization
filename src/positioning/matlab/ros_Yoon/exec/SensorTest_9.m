% clear all;
close all;

fname = 'rosbag\test_b1_line_20220803_v1.mat';
% fname = 'rosbag\test_o_line_20220729_v1.mat';

% load(fname,'Sens');

IMUacc = Sens.imu.acc;
IMUgyro = Sens.imu.gyro;
IMUtime = Sens.imu.time;
b_acc = zeros(3,1);
b_gyro = zeros(3,1);

N = 1000;   %%   inital accumulation
NA = 10;    %%   averaging window

for k = 1 : Sens.length
    if k>1
        dt = IMUtime(k)-IMUtime(k-1);
    else
        dt = 0.01;
    end
    
    p = (IMUgyro(k,1)-b_gyro(1));    q = (IMUgyro(k,2)-b_gyro(2));    r = (IMUgyro(k,3)-b_gyro(3));
    
    acc_b_t(:,k) = IMUacc(k,:)';
    
    if k <= N
        b_acc = b_acc*(k-1)/k+acc_b_t(:,k)/k;
        b_gyro = b_gyro*(k-1)/k+[p;q;r]/k;
        if k == N
            [acc_phi, acc_theta] = EulerAcc(b_acc(1),b_acc(2),b_acc(3));
            gravity = norm(b_acc);
            for l = 0 : NA-1
                acc_out(:,k-l) = acc_b_t(:,k-l)-norm(b_acc)*[sin(acc_theta);-cos(acc_theta)*sin(acc_phi);-cos(acc_theta)*cos(acc_phi)];
                %                 acc_gravity(:,k-l) = gravity*[sin(acc_theta);-cos(acc_theta)*sin(acc_phi);-cos(acc_theta)*cos(acc_phi)];
                acc_phi_t(:,k-l) = acc_phi;
                acc_theta_t(:,k-l) = acc_theta;
                acc_psi(:,k-1) = 0;
            end
            gyro_phi=acc_phi;gyro_theta=acc_theta;gyro_psi=0;
            kf_phi=acc_phi;kf_theta=acc_theta;kf_psi=0;
            
        end
        
    else
        
        if (k < N+N)
            [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
            
            A = [0 0 0]'; B=[0 0;0 0;0 0];
            for AvInd = 1 : NA
                PHI_L = acc_phi_t(k-AvInd);
                THETA_L = acc_theta_t(k-AvInd);
                
                A  = A + acc_b_t(:,k)-acc_b_t(:,k-AvInd) - gravity*[0 cos(THETA_L);-cos(THETA_L)*cos(PHI_L) sin(THETA_L)*sin(PHI_L);cos(THETA_L)*sin(PHI_L) sin(THETA_L)*cos(PHI_L)]*[acc_phi_t(k-1)-acc_phi_t(k-AvInd);acc_theta_t(k-1)-acc_theta_t(k-AvInd)];
                B = B +gravity*[0 cos(THETA_L);-cos(THETA_L)*cos(PHI_L) sin(THETA_L)*sin(PHI_L);cos(THETA_L)*sin(PHI_L) sin(THETA_L)*cos(PHI_L)];
            end
            dEuler = inv(B'*B)*B'*A;
            dPhi = dEuler(1);
            dTheta = dEuler(2);
            acc_b_phi = acc_phi_t(k-1)+dPhi;
            acc_b_theta = acc_theta_t(k-1)+dTheta;
            qua_acc_b = EulerToQuaternion(acc_b_phi, acc_b_theta, gyro_psi);
            qua_kf = EulerToQuaternion(acc_phi_t(k-1), acc_theta_t(k-1), acc_psi_t(k-1));
            
            AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
            
            [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc_b);
            acc_phi_t(k) = kf_phi;
            acc_theta_t(k) = kf_theta;
            acc_psi_t(k) = gyro_psi;
        else
            [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
            [acc_b_phi,acc_b_theta] = EulerAcc(acc_b_t(1,k), acc_b_t(2,k), acc_b_t(3,k));
            qua_acc_b = EulerToQuaternion(acc_b_phi, acc_b_theta, gyro_psi);
            qua_kf = EulerToQuaternion(acc_phi_t(k-1), acc_theta_t(k-1), acc_psi_t(k-1));
            
            AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
            
            [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc_b);
            acc_phi_t(k) = kf_phi;
            acc_theta_t(k) = kf_theta;
            acc_psi_t(k) = gyro_psi;
        end
%         else
%             for AvInd = 1 : NA
%                 A(AvInd,1)  = acc_b_t(3,k)-acc_b_t(3,k-AvInd) - gravity*[cos(acc_theta_t(k-AvInd))*sin(acc_phi_t(k-AvInd)) sin(acc_theta_t(k-AvInd))*cos(acc_phi_t(k-AvInd))]*[acc_phi_t(k-1)-acc_phi_t(k-AvInd);acc_theta_t(k-1)-acc_theta_t(k-AvInd)];
%                 B(AvInd,1:2) = gravity*[cos(acc_theta_t(k-AvInd))*sin(acc_phi_t(k-AvInd)) sin(acc_theta_t(k-AvInd))*cos(acc_phi_t(k-AvInd))];
%             end
%             dEuler = inv(B'*B)*B'*A;
%             dPhi = dEuler(1);
%             dTheta = dEuler(2);
% %             acc_phi_t(k) = acc_phi_t(k-1)+dPhi;
% %             acc_theta_t(k) = acc_theta_t(k-1)+dTheta;
%             acc_phi = acc_phi_t(k-1)+dPhi;
%             acc_theta = acc_theta_t(k-1)+dTheta;
%             [acc_b_phi,acc_b_theta] = EulerAcc(acc_b_t(1,k), acc_b_t(2,k), acc_b_t(3,k));
%             acc_phi_a = (acc_phi+acc_b_phi)/2;
%             acc_theta_a = (acc_theta+acc_b_theta)/2;
%             
%             [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
%             
%             qua_acc_b = EulerToQuaternion(acc_phi_a, acc_theta_a, gyro_psi);
%             qua_kf = EulerToQuaternion(acc_phi_t(k-1), acc_theta_t(k-1), acc_psi_t(k-1));
%             
%             AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
%             
%             [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc_b);
%             acc_phi_t(k) = kf_phi;
%             acc_theta_t(k) = kf_theta;
%             acc_psi_t(k) = gyro_psi;
%         end
        PrevAcc = [0 0 0]';
        acc_out(:,k) = acc_b_t(:,k)-gravity*[sin(acc_phi_t(k));-cos(acc_phi_t(k))*sin(acc_theta_t(k));-cos(acc_phi_t(k))*cos(acc_phi_t(k))];
        for AvInd = 1 : NA
            PrevAcc = PrevAcc -acc_b_t(:,k-AvInd)+acc_out(:,k-AvInd)+gravity*[sin(acc_phi_t(k-AvInd));-cos(acc_phi_t(k-AvInd))*sin(acc_theta_t(k-AvInd));-cos(acc_phi_t(k-AvInd))*cos(acc_phi_t(k-AvInd))];
        end
        acc_out(:,k) = acc_out(:,k)+PrevAcc/NA;
        
    end
    
    %     [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt,gyro_phi,gyro_theta,gyro_psi);
    %
    %     [acc_b_phi,acc_b_theta] = EulerAcc(acc_b(1), acc_b(2), acc_b(3));
    %     qua_acc = EulerToQuaternion(acc_b_phi, acc_b_theta, 0);
    %     qua_kf = EulerToQuaternion(kf_phi, kf_theta, kf_psi);
    %     AA = eye(4)+dt*1/2*[0 -p -q -r;    p 0 r -q;    q -r 0 p;    r q -p 0    ];
    %
    %     [kf_phi,kf_theta,kf_psi] = EulerKalman_2(qua_kf,AA,qua_acc);
    %
    %     gravity = norm(b_acc);
    %     acc_n(:,k) = acc_b(:,k)-gravity*[sin(kf_theta);-cos(kf_theta)*sin(kf_phi);-cos(kf_theta)*cos(kf_phi)];
    % %     acc_n(:,k) = acc_b(:,k)-gravity*[sin(gyro_theta);-cos(gyro_theta)*sin(gyro_phi);-cos(gyro_theta)*cos(gyro_phi)];
    %
    %
    %
    %
    %
    %     pos(:,k+1) = pos(:,k)+vel(:,k)*dt+1/2*dt^2*acc_n(:,k);
    %     vel(:,k+1) = vel(:,k)+acc_n(:,k)*dt;
    %
    % %     acc_b_euler(:,k) = [acc_b_phi;acc_b_theta];
    %     Conv = 180/pi;
    %     kf_phi_t(k) = kf_phi*Conv;           kf_theta_t(k) = kf_theta*Conv;       kf_psi_t(k) = kf_psi*Conv;
    %     gyro_phi_t(k) = gyro_phi*Conv;      gyro_theta_t(k) = gyro_theta*Conv;    gyro_psi_t(k) = gyro_psi*Conv;
    %     acc_b_phi_t(k) = acc_b_phi*Conv;      acc_b_theta_t(k) = acc_b_theta*Conv;    acc_b_psi_t(k) = 0*Conv;
end

picnum = 0;
% figure(picnum);hold off;plot(kf_phi_m_t);hold on;plot(kf_phi_t,'m');plot(gyro_phi_t,'r');title('phi compare')
% picnum=picnum+1;figure(picnum);hold off;plot(gyro_phi_t*pi/180);hold on;plot(gyro_theta_t*pi/180,'m');plot(gyro_psi_t*pi/180,'r');title('gyro euler');legend('phi','theta','phi')
% picnum=picnum+1;figure(picnum);hold off;plot(kf_phi_t);hold on;plot(kf_theta_t,'m');plot(kf_psi_t,'r');title('kf euler');legend('phi','theta','phi')
picnum=picnum+1;figure(picnum);hold off;plot(acc_phi_t);hold on;plot(acc_theta_t,'m');title('ACC euler');legend('phi','theta')
%
% picnum=picnum+1;figure(picnum);hold off;plot(acc_n(1,:));hold on;plot(acc_n(2,:),'r');plot(acc_n(3,:),'g');title('acc n')
picnum=picnum+1;figure(picnum);hold off;plot(acc_b_t(1,:));hold on;plot(acc_b_t(2,:),'r');plot(acc_b_t(3,:),'g');title('acc b')
picnum=picnum+1;figure(picnum);hold off;plot(acc_out(1,:));hold on;plot(acc_out(2,:),'r');plot(acc_out(3,:),'g');title('acc out')

% picnum=picnum+1;figure(picnum);hold off;plot(acc_b(1,:));hold on;plot(acc_b(2,:),'r');plot(acc_b(3,:),'g');title('acc b bias compansated')
% picnum=picnum+1;figure(picnum);hold off;plot(IMUgyro(:,1)-b_gyro(1));hold on;plot(IMUgyro(:,2)-b_gyro(2),'r');title('gyro original - bias')
% picnum=picnum+1;figure(picnum);hold off;plot(vel(1,:));hold on;plot(vel(2,:),'m');plot(vel(3,:),'r');title('velocity')
% picnum=picnum+1;figure(picnum);hold off;plot(pos(1,:));hold on;plot(pos(2,:),'m');plot(pos(3,:),'r');title('position')






