qqc = 1;
for ier = 1 : 4000
    if (IMUacc_t(ier,1)~=0) && (center_pos_est_total(ier)==0)
        IMUacc_c(qqc,:) = IMUacc_t(ier,:);
        IMUgyro_c(qqc,:) = IMUgyro_t(ier,:);
        qqc = qqc + 1;
    end
end

IMUacc_o = mean(IMUacc_c(1:300,:));
IMUgyro_o = mean(IMUgyro_c(1:300,:));

IMUacc_c = IMUacc_c - IMUacc_o;
IMUgyro_c = IMUgyro_c - IMUgyro_o;

dt_gyro = 0.03;
gyro_phi = 0;
gyro_theta = 0;
gyro_psi = 0;

for ier = 1 : size(IMUgyro_c,1)

    p = IMUgyro_c(ier,1)*pi/180;
    q = IMUgyro_c(ier,2)*pi/180;
    r = IMUgyro_c(ier,3)*pi/180;
    
    [gyro_phi,gyro_theta,gyro_psi] = EulerGyroUpdate(p,q,r,dt_gyro,gyro_phi,gyro_theta,gyro_psi);
    
    gyro_phi_t(ier) = mod(gyro_phi+pi,2*pi)-pi;
    gyro_theta_t(ier) = mod(gyro_theta+pi,2*pi)-pi;
    gyro_psi_t(ier) = mod(gyro_psi+pi,2*pi)-pi;
    
end

Po(1) = 0;
Vo(1) = 0;

for ier = 1 : size(IMUgyro_c,1)
    Po(ier+1) = Po(ier) + Vo(ier)*dt_gyro + (IMUacc_c(ier,1)+j*IMUacc_c(ier,2))/1000*exp(j*gyro_psi_t(ier));
    Vo(ier+1) = (Po(ier+1)-Po(ier))/dt_gyro;
end

figure(123);plot(Po,'.')
figure(345);plot(gyro_psi_t)

qqc = 1;
est_pos_prev = 0;
Pos_UWB_Pool = zeros(1,10);
Pos_IMU_Pool = zeros(1,10);
init = 0;

P = 0;
V = 0;
Pprev = 0;
for ier = 1 : 4000


    if ((IMUacc_t(ier,1)~=0) && (center_pos_est_total(ier)==0)) 
        %P(ier+1) = P(ier) + V(ier)*dt_gyro + (IMUacc_t(ier,1)-IMUacc_o(1)+j*(IMUacc_t(ier,2)-IMUacc_o(2)))/1000*exp(j*gyro_psi_t(qqc));
        %V(ier+1) = (P(ier+1)-Pprev)/dt_gyro;
        %Pprev = P(ier+1);
        P(ier+1) = Po(qqc);
        V(ier+1) = Vo(qqc);
        heading(ier+1) = heading(ier);
        qqc = qqc + 1;
        
    elseif (center_pos_est_total(ier)==est_pos_prev)
        P(ier+1) = P(ier);
        V(ier+1) = V(ier);
        heading(ier+1) = heading(ier);
    else
        Pos_UWB_Pool(1) = center_pos_est_total(ier);
        Pos_UWB_Pool(2:end) = Pos_UWB_Pool(1:end-1);
        Pos_IMU_Pool(1) = P(ier);
        Pos_IMU_Pool(2:end) = Pos_IMU_Pool(1:end-1);
        
        if (ier >= 10) && (init == 0)
            Pos_UWB_Pool = ones(1,10)*mean(Pos_UWB_Pool);
            heading(ier+1) = -pi/2;
            init = 1;
        elseif (ier >= 10)
            Pos_UWB_Pool_o = Pos_UWB_Pool - Pos_IMU_Pool(end);
            Pos_IMU_Pool_o = Pos_IMU_Pool - Pos_IMU_Pool(end);
            err_i = 100000;
            for dtheta = -pi:0.01:pi
                err_b = sqrt(mean(abs(Pos_UWB_Pool_o - Pos_IMU_Pool_o*exp(j*(dtheta)).^2)));
                if err_b < err_i
                    err_i = err_b;
                    sel_dtheta = dtheta;
                end
            end
            Pos_IMU_Pool_o = Pos_IMU_Pool_o*exp(j*sel_dtheta);
            P(ier+1) = P(ier);
            Pi(ier+1) = Pos_IMU_Pool_o(1);
            V(ier+1) = V(ier);
            heading(ier+1) = sel_dtheta;
        else
            P(ier+1) = P(ier);
            V(ier+1) = V(ier);
            
            heading(ier+1) = -pi/2;
        end
        est_pos_prev = center_pos_est_total(ier);
    end

    
end

figure(302);plot(P,'.')
figure(303);plot(V,'.')
