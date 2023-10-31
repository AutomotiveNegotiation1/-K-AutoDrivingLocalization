function [Pos, Prob] = TwoAnchPos3(Xa, Ya, dist, tag_pos, EstCenter,anch_pos, dist_a)

AA_Temp1 = (Xa(1)-Xa(2))^2;
AA_Temp2 = (Ya(1)-Ya(2))^2;
AA  = sqrt(AA_Temp1+AA_Temp2);
B = dist(1);
C = dist(2);

s = (AA+B+C)/2;
temp_Temp1 = s*(s-AA);
temp_Temp2 = (s-B)*(s-C);
temp = temp_Temp1*temp_Temp2;

if temp>0
    S = (sqrt(temp));
else
    S = (sqrt(-temp));
end

d = 2*S/AA;

A = [Ya(2)-Ya(1) -(Xa(2)-Xa(1));2*(Xa(2)-Xa(1)) 2*(Ya(2)-Ya(1))];


Y1 = [d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);(-C^2+B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];
Y2 = [-d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);(-C^2+B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];

X1 = inv(A'*A)*A'*Y1;
X2 = inv(A'*A)*A'*Y2;



X1S = sum(abs(abs(anch_pos-(X1(1)+j*X1(2)))-dist_a'));
X2S = sum(abs(abs(anch_pos-(X2(1)+j*X2(2)))-dist_a'));
Y1S = sum((X1-tag_pos').^2);
Y2S = sum((X2-tag_pos').^2);
Z1S = abs(sum((X1-EstCenter').^2)-0.5);
Z2S = abs(sum((X2-EstCenter').^2)-0.5);


if (X1S+Y1S)>((X2S+Y2S))
    Pos = [X2';X1'];
    Prob = [X2S+Y2S;X1S+Y1S];
else
    Pos = [X1';X2'];
    Prob = [X1S+Y1S;X2S+Y2S];
end
