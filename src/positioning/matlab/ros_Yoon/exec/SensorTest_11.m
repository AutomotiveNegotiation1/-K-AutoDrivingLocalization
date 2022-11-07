clear all;
fname = 'rosbag\test_b1_line_20220803_v1.mat';
load(fname,'Sens');
SensNormal = Sens;
IMUacc = SensNormal.imu.acc;

fname = 'rosbag\test_o_line_20220729_v1.mat';
load(fname,'Sens');
SensUpsideDown = Sens;
IMUaccU = SensUpsideDown.imu.acc;

fname = 'rosbag\test_o_line_20220729_v2.mat';
load(fname,'Sens');
SensUpsideDown = Sens;
IMUaccR = SensUpsideDown.imu.acc;

fname = 'rosbag\test_o_line_20220729_v3.mat';
load(fname,'Sens');
SensUpsideDown = Sens;
IMUaccD = SensUpsideDown.imu.acc;

fname = 'rosbag\test_o_line_20220729_v4.mat';
load(fname,'Sens');
SensUpsideDown = Sens;
IMUaccL = SensUpsideDown.imu.acc;

figure(1);
subplot(2,2,1);hold off;plot(IMUaccU(:,1));hold on;plot(IMUaccU(:,2),'r');plot(IMUaccU(:,3),'g');title('IMUacc Up')
subplot(2,2,2);hold off;plot(IMUaccR(:,1));hold on;plot(IMUaccR(:,2),'r');plot(IMUaccR(:,3),'g');title('IMUacc Right')
subplot(2,2,3);hold off;plot(IMUaccL(:,1));hold on;plot(IMUaccL(:,2),'r');plot(IMUaccL(:,3),'g');title('IMUacc Left')
subplot(2,2,4);hold off;plot(IMUaccD(:,1));hold on;plot(IMUaccD(:,2),'r');plot(IMUaccD(:,3),'g');title('IMUacc Down')


        b_acc_z = -0.0784;
        b_acc_y = 0.19;
        b_acc_x = 0.49;

D1 = (IMUacc(1:1000,:))'-[b_acc_x;b_acc_y;b_acc_z];
D2 = (IMUacc(1001:2000,:))'-[b_acc_x;b_acc_y;b_acc_z];
D3 = (IMUacc(11001:12000,:))'-[b_acc_x;b_acc_y;b_acc_z];

D1_U = (IMUaccU(1701:2700,:))';
D1_R = (IMUaccR(3001:4000,:))';
D1_L = (IMUaccL(1:1000,:))';
D1_D = (IMUaccD(2501:3500,:))';

M1 = mean(D1');
M2 = mean(D2');
M3 = mean(D3');
M1_U = mean(D1_U');
M1_R = mean(D1_R');
M1_L = mean(D1_L');
M1_D = mean(D1_D');

G1 = norm(M1');
G2 = norm(M2');
G3 = norm(M3');
G1_U = norm(M1_U');
G1_R = norm(M1_R');
G1_L = norm(M1_L');
G1_D = norm(M1_D');

Az_bias = (G1 - (G1_U+G1_D+G1_L+G1_R)/4)/2;

[P1_phi,P1_theta] = EulerAcc(M1(1),M1(2),M1(3));
[P2_phi,P2_theta] = EulerAcc(M2(1),M2(2),M2(3));
[P3_phi,P3_theta] = EulerAcc(M3(1),M3(2),M3(3));
% [P3_phi,P3_theta] = EulerAcc(P3(1),P3(2),P3(3));
% gravity = norm(M1')-0.0274;
gravity = 9.8;
% gravity_P2 = norm(P2');
% gravity_P3 = norm(P3');

GP1 = [sin(P1_theta);-cos(P1_theta)*sin(P1_phi);-cos(P1_theta)*cos(P1_phi)];
GP2 = [sin(P2_theta);-cos(P2_theta)*sin(P2_phi);-cos(P2_theta)*cos(P2_phi)];
GP3 = [sin(P3_theta);-cos(P3_theta)*sin(P3_phi);-cos(P3_theta)*cos(P3_phi)];
% GP3 = [sin(P3_theta);-cos(P3_theta)*sin(P3_phi);-cos(P3_theta)*cos(P3_phi)];

Dif1 = ((D1-gravity*GP1))  ;
Dif2 = ((D2-gravity*GP1))  ;
Dif3 = ((D3-gravity*GP1))  ;
Dif1_m = mean((D1-gravity*GP1),2)  ;
Dif2_m = mean((D2-gravity*GP1),2)  ;
Dif3_m = mean((D3-gravity*GP1),2)  ;

VectorPlot(Dif1,2,'G off')
VectorPlot(Dif2,3,'G off')
VectorPlot(Dif3,4,'G off')

Second = (P2'-gravity*GP1)/gravity;
dPhi = Second(1);
dTheta = -Second(2);
dGravity = -(Second(3)-(dTheta*P1_theta+dPhi*P1_phi));
P1_phi_a = P1_phi+dPhi;
P1_theta_a = P1_theta+dTheta;
gravity = gravity + dGravity;

Third = ((P1/2+P2/2)'-gravity*[sin(P1_theta_a);-cos(P1_theta_a)*sin(P1_phi_a);-cos(P1_theta_a)*cos(P1_phi_a)])/gravity
P1_phi_a = P1_phi_a + Third(1);
P1_theta_a = P1_theta_a - Third(2);
gravity = gravity - Third(3);

Fourth = (P1/2+P2/2)'-gravity*[sin(P1_theta_a);-cos(P1_theta_a)*sin(P1_phi_a);-cos(P1_theta_a)*cos(P1_phi_a)]

% P1'-gravity*[sin(P1_theta_a);-cos(P1_theta_a)*sin(P1_phi_a);-cos(P1_theta_a)*cos(P1_phi_a)]

figure(10101);hold off;plot(Dif(1,:));hold on;plot(Dif(2,:),'r');plot(Dif(3,:),'g')

DF = mean(Dif');

ALDif = IMUacc';
Filter = LPfilter;


Filtered(1,:) = filter(Filter,ALDif(1,:));
Filtered(2,:) = filter(Filter,ALDif(2,:));
Filtered(3,:) = filter(Filter,ALDif(3,:));

V = [0 0 0]';
P = [0 0 0]';
acc_bias = [0 0 0]';
for dfd = 1 : size(FilteredDC,2)
    Filtered(:,dfd) = Filtered(:,dfd) - acc_bias;
    [acc_phi,acc_theta] = EulerAcc(Filtered(1,dfd),Filtered(2,dfd),Filtered(3,dfd));
    Cb2n = eulr2dcm([acc_phi;acc_theta;0]);
    FilterdG(:,dfd) = ALDif(:,dfd)-gravity*[sin(acc_theta);-cos(acc_theta)*sin(acc_phi);-cos(acc_theta)*cos(acc_phi)];
%      FilterdG(:,dfd) = Cb2n'*ALDif(:,dfd)-gravity*[0;0;-1];
   
    V(:,dfd+1) = V(:,dfd)+FilterdG(:,dfd)*0.01;
    P(:,dfd+1) = P(:,dfd)+V(:,dfd)*0.01+1/2*0.01^2*FilterdG(:,dfd);
    if dfd == 1000
        acc_bias=V(:,1000)/10;
        V(:,dfd+1)=[0 0 0]';
        P(:,dfd+1)=[0 0 0]';
    end
end

figure(10101);hold off;plot(FilterdG(1,:));hold on;plot(FilterdG(2,:),'r');plot(FilterdG(3,:),'g')
figure(10102);hold off;plot(Filtered(1,:));hold on;plot(Filtered(2,:),'r');plot(Filtered(3,:),'g')
figure(10103);hold off;plot(V(1,:));hold on;plot(V(2,:),'r');plot(V(3,:),'g')
figure(10104);hold off;plot(P(1,:));hold on;plot(P(2,:),'r');plot(P(3,:),'g')

