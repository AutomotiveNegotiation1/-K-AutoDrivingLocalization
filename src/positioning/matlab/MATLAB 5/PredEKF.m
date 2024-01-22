function [Xbar] = PredEKF(Xhat,acc,p, dT)

Pxh = Xhat(1); Pyh = Xhat(2); Pzh = Xhat(3);
Vxh = Xhat(4); Vyh = Xhat(5); Vzh = Xhat(6);
abxh = Xhat(7); abyh = Xhat(8); abzh = Xhat(9);
alph = Xhat(10); betah = Xhat(11); gammah = Xhat(12);
pbh = Xhat(13); qbh = Xhat(14); rbh = Xhat(15);

%%% X Y change
Pxb = Pxh + Vxh*dT + 0.5*((acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))+(acc(3)-abzh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah)))*dT^2;
Pyb = Pyh + Vyh*dT + 0.5*((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT^2;
Pzb = Pzh + Vzh*dT + 0.5*(-1*(acc(1)-abxh)*sin(betah)+(acc(2)-abyh)*(cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(betah)*cos(gammah)))*dT^2;

Vxb = Vxh + ((acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))+(acc(3)-abzh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah)))*dT;
Vyb = Vyh + ((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT;
Vzb = Vzh + (-1*(acc(1)-abxh)*sin(betah)+(acc(2)-abyh)*(cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(betah)*cos(gammah)))*dT;

%%% X Y original
% Pxb = Pxh + Vxh*dT + 0.5*((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT^2;
% Pyb = Pyh + Vyh*dT + 0.5*((acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))+(acc(3)-abzh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah)))*dT^2;
% Pzb = Pzh + Vzh*dT + 0.5*(-1*(acc(1)-abxh)*sin(betah)+(acc(2)-abyh)*(cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(betah)*cos(gammah)))*dT^2;
% 
% Vxb = Vxh + ((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT;
% Vyb = Vyh + ((acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))+(acc(3)-abzh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah)))*dT;
% Vzb = Vzh + (-1*(acc(1)-abxh)*sin(betah)+(acc(2)-abyh)*(cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(betah)*cos(gammah)))*dT;


abxb = abxh;
abyb = abyh;
abzb = abzh;

alpb = alph + ((p(2)-qbh)*sin(gammah)/cos(betah)+(p(3)-rbh)*cos(gammah)/cos(betah))*dT;
betab = betah + ((p(2)-qbh)*cos(gammah)-(p(3)-rbh)*sin(gammah))*dT;
gammab = gammah + ((p(1)-pbh)+(p(2)-qbh)*(sin(gammah)*tan(betah))+(p(3)-rbh)*(cos(gammah)*tan(betah)))*dT;

pbb = pbh;
qbb = qbh;
rbb = rbh;

Xbar = [Pxb Pyb Pzb Vxb Vyb Vzb abxb abyb abzb alpb betab gammab pbb qbb rbb];



