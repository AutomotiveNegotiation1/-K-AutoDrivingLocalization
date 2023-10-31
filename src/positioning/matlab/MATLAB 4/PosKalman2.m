function [x] = PosKalman2(x, A, z, R)

persistent H Q
persistent P
persistent firstRun

if isempty(firstRun)
    H = zeros(3,12);
%     H(2,2) = 1; H(3:6,3:6) = eye(4);
    H(1:3,1:3) = eye(3);
    Q = 1e-2*eye(12);
    Q(10:12,10:12)=1e-5*eye(3);
    
    
    P = 1*eye(12); P(1:3,4:6) = 1*eye(3); P(1:6,7:12) = 1*ones(6,6);
    P(10:12,10:12) = 1*eye(3);
    firstRun = 1;
end

xp = A*x;   % 12x1, 
Pp = A*P*A' + Q;

K = Pp*H'*inv(H*Pp*H' + R);

x = xp + K*(z - H*xp);
P = Pp - K*H*Pp;
