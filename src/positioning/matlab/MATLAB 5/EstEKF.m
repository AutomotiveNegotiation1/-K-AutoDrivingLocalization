function [zhat, H] = EstEKF(Xhat,acc,p,dT, xb)

Pxh = Xhat(1); Pyh = Xhat(2); Pzh = Xhat(3);
Vxh = Xhat(4); Vyh = Xhat(5); Vzh = Xhat(6);
abxh = Xhat(7); abyh = Xhat(8); abzh = Xhat(9);
alph = Xhat(10); betah = Xhat(11); gammah = Xhat(12);
pbh = Xhat(13); qbh = Xhat(14); rbh = Xhat(15);

betae = betah;
gammae = gammah;
Pxe = Pxh + (xb(1)*cos(alph)-xb(2)*sin(alph));
Pye = Pyh + (xb(1)*sin(alph)+xb(2)*cos(alph));
Pze = Pzh;
zhat(1) = betae;
zhat(2) = gammae;
zhat(3) = Pxe;
zhat(4) = Pye;
zhat(5) = Pze;

H = zeros(5,15);
H(1,11) = 1;
H(2,12) = 1;
H(3,1) = 1;
H(4,2) = 1;
H(3,10) = -xb(1)*sin(alph)-xb(2)*cos(alph);
H(4,10) = xb(1)*cos(alph)-xb(2)*sin(alph);
H(5,3) = 1;


