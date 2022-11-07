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

gravity = 9.7964;
M1 = mean(IMUacc(1:1000,:))';
M2 = mean(IMUacc(1001:2000,:))';
M3 = mean(IMUacc(11001:12000,:))';

Norm1 = norm(M1');
Norm2 = norm(M2');
Norm3 = norm(M3');
dG = 0;

e1 = (gravity+dG)^2 - Norm1^2;
e2 = (gravity+dG)^2 - Norm2^2;
e3 = (gravity+dG)^2 - Norm3^2;
Amat = zeros(1,3);
Emat1 = 0;
Emat2 = 0;
Emat3 = 0;

k = 1;
for b_acc_x = -1:0.2:1
    for b_acc_y = -1:0.2:1
        for b_acc_z = -1:0.2:1
            Amat(k,:) = -2*[b_acc_x b_acc_y b_acc_z];
            Emat1(k,1) = e1+b_acc_x^2 + b_acc_y^2 + b_acc_z^2;
            Emat2(k,1) = e2+b_acc_x^2 + b_acc_y^2 + b_acc_z^2;
            Emat3(k,1) = e3+b_acc_x^2 + b_acc_y^2 + b_acc_z^2;
            k = k + 1;
        end
    end
end

Ymat1 = inv(Amat'*Amat)*Amat'*Emat1;
Ymat2 = inv(Amat'*Amat)*Amat'*Emat3;
Ymat3 = inv(Amat'*Amat)*Amat'*Emat2;

            
