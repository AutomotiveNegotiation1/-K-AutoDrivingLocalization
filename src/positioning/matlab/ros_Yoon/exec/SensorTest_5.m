clear all;
close all;

fname = 'rosbag\test_b1_line_20220803_v1.mat';
% fname = 'rosbag\test_o_line_20220729_v1.mat';

load(fname,'Sens');

IMUacc = Sens.imu.acc;
IMUgyro = Sens.imu.gyro;
IMUtime = Sens.imu.time;
% IMUacc(:,2) = -IMUacc(:,2);
SensorTest_init;
StartTime = IMUtime(1);
Nindex = 1;
acc_org_t = [0 0 0]';
gyro_org_t = [0 0 0]';
ll = 1;
for k = 1 : Sens.length
    if (IMUtime(k)-StartTime) > 1
        acc_org_av(:,Nindex) = acc_org_t;
        acc_org_t = IMUacc(k,:)';
        gyro_org_av(:,Nindex) = gyro_org_t;
        gyro_org_t = IMUgyro(k,:)';
        StartTime = IMUtime(k);
        ll = 1;
        Nindex = Nindex + 1;
    else
        acc_org_t = acc_org_t*(ll-1)/ll + IMUacc(k,:)'/ll;
        gyro_org_t = gyro_org_t*(ll-1)/ll + IMUgyro(k,:)'/ll;
        ll = ll + 1;
    end
    if k > 100
        acc_org_ma(:,k) = mean(IMUacc(k-99:k,:))';
    else
        acc_org_ma(:,k) = [0 0 0]';
    end
end
V = [1 0 0]; Adiff = GyroToAcc(gyro_org_av-mean(gyro_org_av(:,1:100)')',V);
figure(10);hold off;plot(Adiff(2,:)*1);hold on;plot(acc_org_av(2,:),'r');
figure(11);hold off;plot(gyro_org_av(1,:));hold on;plot(gyro_org_av(2,:),'r');plot(gyro_org_av(3,:),'g')
figure(12);hold off;plot(acc_org_av(1,:));hold on;plot(acc_org_av(2,:),'r');plot(acc_org_av(3,:),'g')
figure(13);hold off;plot(acc_org_ma(1,:));hold on;plot(acc_org_ma(2,:),'r');plot(acc_org_ma(3,:),'g')

picnum = 0;
% figure(picnum);hold off;plot(kf_phi_m_t);hold on;plot(kf_phi_t,'m');plot(gyro_phi_t,'r');title('phi compare')
% picnum=picnum+1;figure(picnum);hold off;plot(gyro_phi_t);hold on;plot(gyro_theta_t,'m');plot(gyro_psi_t,'r');title('gyro euler');legend('phi','theta','psi')
% picnum=picnum+1;figure(picnum);hold off;plot(kf_phi_t);hold on;plot(kf_theta_t,'m');plot(kf_psi_t,'r');title('kf euler');legend('phi','theta','psi')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_phi_t);hold on;plot(acc_theta_t,'m');title('acc euler');legend('phi','theta')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_org(1,:));hold on;plot(acc_org(2,:),'r');plot(acc_org(3,:),'g');title('acc b')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_wo_bias(1,:));hold on;plot(acc_wo_bias(2,:),'r');plot(acc_wo_bias(3,:),'g');title('acc c')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_corrected(1,:));hold on;plot(acc_corrected(2,:),'r');plot(acc_corrected(3,:),'g');title('acc d')
% picnum=picnum+1;figure(picnum);hold off;plot(vel(1,:));hold on;plot(vel(2,:),'m');plot(vel(3,:),'r');title('velocity')
% picnum=picnum+1;figure(picnum);hold off;plot(pos(1,:));hold on;plot(pos(2,:),'m');plot(pos(3,:),'r');title('position')
% 
% 
% 
% 
% picnum=picnum+1;figure(picnum);hold off;plot(acc_n(1,:));hold on;plot(acc_n(2,:),'r');plot(acc_n(3,:),'g');title('acc n')
% picnum=picnum+1;figure(picnum);hold off;plot(acc_b(1,:));hold on;plot(acc_b(2,:),'r');plot(acc_b(3,:),'g');title('acc b bias compansated')
% picnum=picnum+1;figure(picnum);hold off;plot(IMUgyro(:,1)-b_gyro(1));hold on;plot(IMUgyro(:,2)-b_gyro(2),'r');title('gyro original - bias')







