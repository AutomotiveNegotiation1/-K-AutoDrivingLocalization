function [x] = PosKalman(x, A, z, R)

persistent H Q
persistent P
persistent firstRun

if isempty(firstRun)
    H = zeros(6,12);
%     H(2,2) = 1; H(3:6,3:6) = eye(4);
    H(1:6,1:6) = eye(6);
    Q = 1e-4*eye(12);
    
    
    P = 1*eye(12); P(1:3,4:6) = 1*eye(3); P(1:6,7:12) = 10*ones(6,6);
    P(10:12,10:12) = 100*eye(3);
    firstRun = 1;
end

xp = A*x;
Pp = A*P*A' + Q;

K = Pp*H'*inv(H*Pp*H' + R);

x = xp + K*(z - H*xp);
P = Pp - K*H*Pp;
