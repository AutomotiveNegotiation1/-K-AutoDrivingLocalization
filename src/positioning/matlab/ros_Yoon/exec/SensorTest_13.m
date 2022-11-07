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
gravity = 9.85;

Varia1_min = 100;
Varia2_min = 100;
Varia2_min = 100;


b_acc_x_v = [-0.1:0.01:0.5];
b_acc_y_v = [-0.1:0.01:1];
b_acc_z_v = [-0.1:0.01:1];
b_gravity_v = [9.75:0.01:9.9];

for b_gravity_i = 1 : length(b_gravity_v)
    gravity = b_gravity_v(b_gravity_i)
for b_acc_x_i = 1: length(b_acc_x_v)
    b_acc_x = b_acc_x_v(b_acc_x_i)
    for b_acc_y_i = 1: length(b_acc_y_v)
        b_acc_y = b_acc_y_v(b_acc_y_i)

%         b_acc_z = -0.0784
        for b_acc_z_i = 1 : length(b_acc_z_v);
            b_acc_z = b_acc_z_v(b_acc_z_i);
%         b_acc_y = 0.19;
%         b_acc_x = 0.49;
        temp_sum = 0;
        for i = 1 : Sens.length
            D1 = IMUacc(i,:)'-[b_acc_x;b_acc_y;b_acc_z];
           [P1_phi,P1_theta] = EulerAcc(D1(1),D1(2),D1(3));
            P1_theta_test = asin(D1(1)/gravity);
            P1_phi_test_a = asin(-D1(2)/gravity);
            P1_phi_test_b = acos(-D1(3)/gravity);
            
            temp_sum = temp_sum + abs(1-sin(P1_theta_test)/sin(P1_theta))^2+abs(1-cos(P1_theta_test)/cos(P1_theta))^2;
            temp_sum = temp_sum + abs(1-sin(P1_phi_test_a)/sin(P1_phi))^2+abs(1-cos(P1_phi_test_b)/cos(P1_phi))^2;
            temp_sum = temp_sum + abs(1-sin(P1_phi_test_a)/sin(P1_phi_test_b))^2+abs(1-cos(P1_phi_test_a)/cos(P1_phi_test_b))^2;
            
        end
        Varia1(b_gravity_i,b_acc_x_i,b_acc_y_i,b_acc_z_i) = temp_sum/Sens.length;
        if (Varia1(b_gravity_i,b_acc_x_i,b_acc_y_i,b_acc_z_i)< Varia1_min)
            b_gravity_min = gravity
            b_acc_x_min = b_acc_x
            b_acc_y_min = b_acc_y
            b_acc_z_min = b_acc_z
            Varia1_min = Varia1(b_gravity_i,b_acc_x_i,b_acc_y_i,b_acc_z_i);
        end
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
    end
    
    end
end
end

figure(10);surf(b_acc_y_v,b_acc_x_v,-Varia1);
% figure(10101);hold off;plot(FilterdG(1,:));hold on;plot(FilterdG(2,:),'r');plot(FilterdG(3,:),'g')
% figure(10102);hold off;plot(Filtered(1,:));hold on;plot(Filtered(2,:),'r');plot(Filtered(3,:),'g')
% figure(10103);hold off;plot(V(1,:));hold on;plot(V(2,:),'r');plot(V(3,:),'g')
% figure(10104);hold off;plot(P(1,:));hold on;plot(P(2,:),'r');plot(P(3,:),'g')

