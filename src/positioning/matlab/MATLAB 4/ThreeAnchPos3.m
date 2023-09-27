function [Pos,Prob]=ThreeAnchPos3(xa, ya, dist)
M = length(xa);
A = zeros(M-1,2);
y = zeros(M-1,1);


% if (ya(2)-y(1))*(xa(3)-xa(1)) ~= (ya(3)-y(1))*(xa(2)-xa(1))
    for k = 1 : M-1 
        A(k,:) = [-2*(xa(k)-xa(M)) -2*(ya(k)-ya(M))];
        y(k,1) = dist(k)^2-dist(M)^2-(xa(k)^2-xa(M)^2)-(ya(k)^2-ya(M)^2);
    end
% else
%     
% end


% Pos = (A'*A)\(A'*y);
Pos = inv(A'*A)*A'*y;

Prob = sqrt(mean(abs((xa - Pos(1)).^2 + (ya - Pos(2)).^2 - dist.^2)));

                      