function [x] = PosKalman_v1(x, A, z, R)

persistent H Q
persistent P
persistent firstRun

if isempty(firstRun)
    H = eye(3);
    Q = 10*eye(3);
    
    
    P = 1*eye(3,3);
    firstRun = 1;
    
end

xp = A*x;
Pp = A*P*A' + Q;

K = Pp*H'*pinv(H*Pp*H' + R);

x = xp + K*(z - H*xp);
P = Pp - K*H*Pp;
