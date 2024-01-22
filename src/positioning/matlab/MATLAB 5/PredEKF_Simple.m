function [Xbar] = PredEKF_Simple(Xhat,acc,p, dT)

Pxh = Xhat(1); Pyh = Xhat(2); 
Vxh = Xhat(3); Vyh = Xhat(4); 
abxh = Xhat(5); abyh = Xhat(6); 
alph = Xhat(7); rbh = Xhat(8);

%%% X Y change
Pxb = Pxh + Vxh*dT + 0.5*((acc(1)-abxh)*sin(alph)+(acc(2)-abyh)*cos(alph))*dT^2;
Pyb = Pyh + Vyh*dT + 0.5*((acc(1)-abxh)*cos(alph)+(acc(2)-abyh)*(-sin(alph)))*dT^2;

Vxb = Vxh + ((acc(1)-abxh)*sin(alph)+(acc(2)-abyh)*cos(alph))*dT;
Vyb = Vyh + ((acc(1)-abxh)*cos(alph)+(acc(2)-abyh)*(-sin(alph)))*dT;

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

alpb = alph + (p(3)-rbh)*dT;

rbb = rbh;

Xbar = [Pxb Pyb Vxb Vyb abxb abyb alpb rbb];



