function [A] = makePredA_Simple(Xhat,acc,p, dT)

Pxh = Xhat(1); Pyh = Xhat(2); 
Vxh = Xhat(3); Vyh = Xhat(4); 
abxh = Xhat(5); abyh = Xhat(6); 
alph = Xhat(7); rbh = Xhat(8);

N = length(Xhat);

A = eye(N);

A(1,3) = dT;
A(1,5) = -0.5*sin(alph)*dT^2;
A(1,6) = -0.5*cos(alph)*dT^2;
A(1,7) = 0.5*((acc(1)-abxh)*cos(alph)-(acc(2)-abyh)*sin(alph))*dT^2;
A(2,4) = dT;
A(2,5) = -0.5*cos(alph)*dT^2;
A(2,6) = 0.5*sin(alph)*dT^2;
A(2,7) = 0.5*(-(acc(1)-abxh)*sin(alph)-(acc(2)-abyh)*cos(alph))*dT^2;
A(3,5) = -0.5*sin(alph)*dT;
A(3,6) = -0.5*cos(alph)*dT;
A(3,7) = 0.5*((acc(1)-abxh)*cos(alph)-(acc(2)-abyh)*sin(alph))*dT;
A(4,5) = -0.5*cos(alph)*dT;
A(4,6) = 0.5*sin(alph)*dT;
A(4,7) = 0.5*(-(acc(1)-abxh)*sin(alph)-(acc(2)-abyh)*cos(alph))*dT;
A(7,8) = -dT;









