function Pos = TwoAnchPos(Xa, Ya, dist, EstCenter)
% Xa = [0 1];
% Ya = [0 0];
% dist = [1 1];

AA  = sqrt((Xa(1)-Xa(2))^2+(Ya(1)-Ya(2))^2);
B = dist(1);
C = dist(2);

d = sqrt(B^2-((B^2-C^2+AA^2)/(2*AA))^2);

A = [Ya(2)-Ya(1) -(Xa(2)-Xa(1));2*(Xa(2)-Xa(1)) 2*(Ya(2)-Ya(1))];
Y1 = [d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);-(C^2-B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];
Y2 = [-d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);-(C^2-B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];

X1 = inv(A'*A)*A'*Y1;
X2 = inv(A'*A)*A'*Y2;
if sum((X1-EstCenter').^2)>sum((X2-EstCenter').^2)
    Pos = [X2'];
else
    Pos = [X1'];
end