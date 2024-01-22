function [x] = HeadingKalman(x, A, z, R)

persistent H Q
persistent P
persistent firstRun

if isempty(firstRun)
    H = zeros(6,6);
%     H(2,2) = 1; H(3:6,3:6) = eye(4);
    H(1:6,1:6) = eye(6);
    Q = 1e-4*eye(6);
    
    
    P = 1*eye(6); P(1:3,4:6) = 1*eye(3); 
    firstRun = 1;
end

xp = A(:,1:3)*x;
Pp = A*P*A' + Q;

K = Pp*H'*inv(H*Pp*H' + R);

x = xp + K*(z - H*xp);
P = Pp - K*H*Pp;