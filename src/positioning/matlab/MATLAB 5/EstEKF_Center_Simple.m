function [zhat, H] = EstEKF_Center_Simple(Xhat)

Pxh = Xhat(1); Pyh = Xhat(2); 
Vxh = Xhat(3); Vyh = Xhat(4); 
abxh = Xhat(5); abyh = Xhat(6); 
alph = Xhat(7); rbh = Xhat(8);

Pxe = Pxh;
Pye = Pyh;

zhat(1) = alph;
zhat(2) = Pxe;
zhat(3) = Pye;

H = zeros(3,8);
H(1,7) = 1;
H(2,1) = 1;
H(3,2) = 1;

% H(2,7) = -xb(1)*sin(alph)+xb(2)*cos(alph);
% H(3,7) = -xb(1)*cos(alph)-xb(2)*sin(alph);


