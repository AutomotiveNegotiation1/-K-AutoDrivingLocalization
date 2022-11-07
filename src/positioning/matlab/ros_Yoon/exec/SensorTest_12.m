clear all;
fname = 'rosbag\test_b1_line_20220803_v1.mat';
load(fname,'Sens');
SensNormal = Sens;
IMUacc = SensNormal.imu.acc;
% 
% fname = 'rosbag\test_o_line_20220729_v1.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccU = SensUpsideDown.imu.acc;
% 
% fname = 'rosbag\test_o_line_20220729_v2.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccR = SensUpsideDown.imu.acc;
% 
% fname = 'rosbag\test_o_line_20220729_v3.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccD = SensUpsideDown.imu.acc;
% 
% fname = 'rosbag\test_o_line_20220729_v4.mat';
% load(fname,'Sens');
% SensUpsideDown = Sens;
% IMUaccL = SensUpsideDown.imu.acc;

% figure(1);
% subplot(2,2,1);hold off;plot(IMUaccU(:,1));hold on;plot(IMUaccU(:,2),'r');plot(IMUaccU(:,3),'g');title('IMUacc Up')
% subplot(2,2,2);hold off;plot(IMUaccR(:,1));hold on;plot(IMUaccR(:,2),'r');plot(IMUaccR(:,3),'g');title('IMUacc Right')
% subplot(2,2,3);hold off;plot(IMUaccL(:,1));hold on;plot(IMUaccL(:,2),'r');plot(IMUaccL(:,3),'g');title('IMUacc Left')
% subplot(2,2,4);hold off;plot(IMUaccD(:,1));hold on;plot(IMUaccD(:,2),'r');plot(IMUaccD(:,3),'g');title('IMUacc Down')
gravity = 9.8;

Varia1_min = 100;
Varia2_min = 100;
Varia2_min = 100;

indx = 1;
indy = 1;
indz = 1;
for b_acc_x = -0.1:0.001:0.1
    indy = 1
    for b_acc_y = -0.1:0.001:0.1
        b_acc_z = -0.0784;
        
        D1 = (IMUacc(1:1000,:))'-[b_acc_x;b_acc_y;b_acc_z];
        D2 = (IMUacc(1001:2000,:))'-[b_acc_x;b_acc_y;b_acc_z];
        D4 = (IMUacc(11001:12000,:))'-[b_acc_x;b_acc_y;b_acc_z];
        D3 = (IMUacc(2001:3000,:))'-[b_acc_x;b_acc_y;b_acc_z];
        % D1_U = (IMUaccU(1701:2700,:))';
        % D1_R = (IMUaccR(3001:4000,:))';
        % D1_L = (IMUaccL(1:1000,:))';
        % D1_D = (IMUaccD(2501:3500,:))';
        
        M1 = mean(D1');
        M2 = mean(D2');
        M3 = mean(D3');
        M4 = mean(D4');
        % M1_U = mean(D1_U');
        % M1_R = mean(D1_R');
        % M1_L = mean(D1_L');
        % M1_D = mean(D1_D');
        
        G1 = norm(M1');
        G2 = norm(M2');
        G3 = norm(M3');
        G4 = norm(M3');
        % G1_U = norm(M1_U');
        % G1_R = norm(M1_R');
        % G1_L = norm(M1_L');
        % G1_D = norm(M1_D');
        
        % Az_bias = (G1 - (G1_U+G1_D+G1_L+G1_R)/4)/2;
        
        [P1_phi,P1_theta] = EulerAcc(M1(1),M1(2),M1(3));
        [P2_phi,P2_theta] = EulerAcc(M2(1),M2(2),M2(3));
        [P3_phi,P3_theta] = EulerAcc(M3(1),M3(2),M3(3));
        [P4_phi,P4_theta] = EulerAcc(M4(1),M4(2),M4(3));
        
        P1_theta_test = asin(M1(1)/gravity);
        P2_theta_test = asin(M2(1)/gravity);
        P3_theta_test = asin(M3(1)/gravity);
        P4_theta_test = asin(M4(1)/gravity);
        Varia1(indx,indy,indz) = sin(P1_theta_test)^2/sin(P1_theta)^2;
        Varia2(indx,indy,indz) = sin(P2_theta_test)^2/sin(P2_theta)^2;
        Varia3(indx,indy,indz) = sin(P3_theta_test)^2/sin(P3_theta)^2;
        Varia4(indx,indy,indz) = sin(P4_theta_test)^2/sin(P4_theta)^2;
        
        % if abs(Varia1(ind)-1) < Varia1_min
        %     b_acc_x_min = b_acc_x;
        %     b_acc_y_min = b_acc_y;
        %     b_acc_z_min = b_acc_z;
        %
        %     Varia1_min = abs(Varia1(ind)-1);
        % end
        
        
        % [P3_phi,P3_theta] = EulerAcc(P3(1),P3(2),P3(3));
        % gravity = norm(M1')-0.0274;
        % gravity_P2 = norm(P2');
        % gravity_P3 = norm(P3');
        indy = indy + 1;
    end
    indx = indx + 1;
    
end

% figure(10101);hold off;plot(FilterdG(1,:));hold on;plot(FilterdG(2,:),'r');plot(FilterdG(3,:),'g')
% figure(10102);hold off;plot(Filtered(1,:));hold on;plot(Filtered(2,:),'r');plot(Filtered(3,:),'g')
% figure(10103);hold off;plot(V(1,:));hold on;plot(V(2,:),'r');plot(V(3,:),'g')
% figure(10104);hold off;plot(P(1,:));hold on;plot(P(2,:),'r');plot(P(3,:),'g')

