function [A] = makePredA(Xhat,acc,p, dT)

Pxh = Xhat(1); Pyh = Xhat(2); Pzh = Xhat(3);
Vxh = Xhat(4); Vyh = Xhat(5); Vzh = Xhat(6);
abxh = Xhat(7); abyh = Xhat(8); abzh = Xhat(9);
alph = Xhat(10); betah = Xhat(11); gammah = Xhat(12);
pbh = Xhat(13); qbh = Xhat(14); rbh = Xhat(15);

N = length(Xhat);

A = eye(N);

A(1,4) = dT;
A(2,5) = dT;

A(2,7) = -0.5*cos(alph)*cos(betah)*dT^2;
A(2,8) = -0.5*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))*dT^2;
A(2,9) = -0.5*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah))*dT^2;

A(2,10) = 0.5*(-(acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(-sin(alph)*sin(betah)*sin(gammah)-cos(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT^2;
A(2,11) = 0.5*(-(acc(1)-abxh)*cos(alph)*sin(betah)+(acc(2)-abyh)*(cos(alph)*cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(alph)*cos(betah)*cos(gammah)))*dT^2;
A(2,12) = 0.5*((acc(2)-abyh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah))+(acc(3)-abzh)*(-cos(alph)*sin(betah)*sin(gammah)+sin(alph)*cos(gammah)))*dT^2;

A(1,7) = -0.5*sin(alph)*cos(betah)*dT^2;
A(1,8) = -0.5*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))*dT^2;
A(1,9) = -0.5*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah))*dT^2;

A(1,10) = 0.5*((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT^2;
A(1,11) = 0.5*(-(acc(1)-abxh)*sin(alph)*sin(betah)+(acc(2)-abyh)*(sin(alph)*cos(betah)*sin(gammah))+(acc(3)-abzh)*(sin(alph)*cos(betah)*cos(gammah)))*dT^2;
A(1,12) = 0.5*((acc(2)-abyh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah))+(acc(3)-abzh)*(-sin(alph)*sin(betah)*sin(gammah)-cos(alph)*cos(gammah)))*dT^2;


% A(1,7) = -0.5*cos(alph)*cos(betah)*dT^2;
% A(1,8) = -0.5*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))*dT^2;
% A(1,9) = -0.5*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah))*dT^2;
% 
% A(1,10) = 0.5*(-(acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(-sin(alph)*sin(betah)*sin(gammah)-cos(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT^2;
% A(1,11) = 0.5*(-(acc(1)-abxh)*cos(alph)*sin(betah)+(acc(2)-abyh)*(cos(alph)*cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(alph)*cos(betah)*cos(gammah)))*dT^2;
% A(1,12) = 0.5*((acc(2)-abyh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah))+(acc(3)-abzh)*(-cos(alph)*sin(betah)*sin(gammah)+sin(alph)*cos(gammah)))*dT^2;

% A(2,7) = -0.5*sin(alph)*cos(betah)*dT^2;
% A(2,8) = -0.5*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))*dT^2;
% A(2,9) = -0.5*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah))*dT^2;
% 
% A(2,10) = 0.5*((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT^2;
% A(2,11) = 0.5*(-(acc(1)-abxh)*sin(alph)*sin(betah)+(acc(2)-abyh)*(sin(alph)*cos(betah)*sin(gammah))+(acc(3)-abzh)*(sin(alph)*cos(betah)*cos(gammah)))*dT^2;
% A(2,12) = 0.5*((acc(2)-abyh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah))+(acc(3)-abzh)*(-sin(alph)*sin(betah)*sin(gammah)-cos(alph)*cos(gammah)))*dT^2;



A(3,6) = dT;
A(3,7) = 0.5*sin(betah)*dT^2;
A(3,8) = 0.5*cos(betah)*sin(gammah)*dT^2;
A(3,9) = 0.5*cos(betah)*cos(gammah)*dT^2;

A(3,10) = 0;
A(3,11) = 0.5*(-(acc(1)-abxh)*cos(betah)+(acc(2)-abyh)*(-sin(betah)*sin(gammah)+(acc(3)-abzh)*(-sin(betah)*cos(gammah))))*dT^2;
A(3,12) = 0.5*((acc(2)-abyh)*cos(betah)*cos(gammah)-(acc(3)-abzh)*cos(betah)*sin(gammah))*dT^2;

A(5,7) = -0.5*cos(alph)*cos(betah)*dT;
A(5,8) = -0.5*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))*dT;
A(5,9) = -0.5*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah))*dT;

A(5,10) = 0.5*(-(acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(-sin(alph)*sin(betah)*sin(gammah)-cos(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT;
A(5,11) = 0.5*(-(acc(1)-abxh)*cos(alph)*sin(betah)+(acc(2)-abyh)*(cos(alph)*cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(alph)*cos(betah)*cos(gammah)))*dT;
A(5,12) = 0.5*((acc(2)-abyh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah))+(acc(3)-abzh)*(-cos(alph)*sin(betah)*sin(gammah)+sin(alph)*cos(gammah)))*dT;

A(4,7) = -0.5*sin(alph)*cos(betah)*dT;
A(4,8) = -0.5*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))*dT;
A(4,9) = -0.5*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah))*dT;

A(4,10) = 0.5*((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT;
A(4,11) = 0.5*(-(acc(1)-abxh)*sin(alph)*sin(betah)+(acc(2)-abyh)*(sin(alph)*cos(betah)*sin(gammah))+(acc(3)-abzh)*(sin(alph)*cos(betah)*cos(gammah)))*dT;
A(4,12) = 0.5*((acc(2)-abyh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah))+(acc(3)-abzh)*(-sin(alph)*sin(betah)*sin(gammah)-cos(alph)*cos(gammah)))*dT;

% A(4,7) = -0.5*cos(alph)*cos(betah)*dT;
% A(4,8) = -0.5*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))*dT;
% A(4,9) = -0.5*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah))*dT;
% 
% A(4,10) = 0.5*(-(acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(-sin(alph)*sin(betah)*sin(gammah)-cos(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT;
% A(4,11) = 0.5*(-(acc(1)-abxh)*cos(alph)*sin(betah)+(acc(2)-abyh)*(cos(alph)*cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(alph)*cos(betah)*cos(gammah)))*dT;
% A(4,12) = 0.5*((acc(2)-abyh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah))+(acc(3)-abzh)*(-cos(alph)*sin(betah)*sin(gammah)+sin(alph)*cos(gammah)))*dT;
% 
% A(5,7) = -0.5*sin(alph)*cos(betah)*dT;
% A(5,8) = -0.5*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))*dT;
% A(5,9) = -0.5*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah))*dT;
% 
% A(5,10) = 0.5*((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT;
% A(5,11) = 0.5*(-(acc(1)-abxh)*sin(alph)*sin(betah)+(acc(2)-abyh)*(sin(alph)*cos(betah)*sin(gammah))+(acc(3)-abzh)*(sin(alph)*cos(betah)*cos(gammah)))*dT;
% A(5,12) = 0.5*((acc(2)-abyh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah))+(acc(3)-abzh)*(-sin(alph)*sin(betah)*sin(gammah)-cos(alph)*cos(gammah)))*dT;

A(6,7) = 0.5*sin(betah)*dT;
A(6,8) = 0.5*cos(betah)*sin(gammah)*dT;
A(6,9) = 0.5*cos(betah)*cos(gammah)*dT;

A(6,10) = 0;
A(6,11) = 0.5*(-(acc(1)-abxh)*cos(betah)+(acc(2)-abyh)*(-sin(betah)*sin(gammah)+(acc(3)-abzh)*(-sin(betah)*cos(gammah))))*dT;
A(6,12) = 0.5*((acc(2)-abyh)*cos(betah)*cos(gammah)-(acc(3)-abzh)*cos(betah)*sin(gammah))*dT;

A(10,11) = ((p(2)-qbh)*sin(gammah)*sin(betah)/cos(betah)^2+(p(3)-rbh)*cos(gammah)*sin(betah)/cos(betah)^2)*dT;
A(10,12) = ((p(2)-qbh)*cos(gammah)/cos(betah)+(p(3)-rbh)*(-sin(gammah))/cos(betah))*dT;

A(10,13) = 0;
A(10,14) = -sin(gammah)/cos(betah);
A(10,15) = -cos(gammah)/cos(betah);

A(11,10) = 0;
A(11,11) = 1;
A(11,12) = (-(p(2)-qbh)*sin(gammah)-(p(3)-rbh)*cos(gammah))*dT;

A(11,13) = 0;
A(11,14) = -cos(gammah)*dT;
A(11,15) = sin(gammah)*dT;

A(12,10) = 0;
A(12,11) = ((p(2)-qbh)*sin(gammah)*sec(betah)^2+(p(3)-rbh)*cos(gammah)*sec(betah)^2)*dT;
A(12,12) = 1+((p(2)-qbh)*cos(gammah)*tan(betah)+(p(3)-rbh)*(-sin(gammah)*tan(betah)))*dT;

A(12,13) = -1*dT;
A(12,14) = -sin(gammah)*tan(betah)*dT;
A(12,15) = -cos(gammah)*tan(betah)*dT;







