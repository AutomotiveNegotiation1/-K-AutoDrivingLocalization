function [zhat, H] = EstEKF_Single_Simple(Xhat,acc,p,dT, xb, xa, ya)

Pxh = Xhat(1); Pyh = Xhat(2); 
Vxh = Xhat(3); Vyh = Xhat(4); 
abxh = Xhat(5); abyh = Xhat(6); 
alph = Xhat(7); rbh = Xhat(8);

distSq = (xa-(Pxh+xb(1)*cos(-alph)-xb(2)*sin(-alph)))^2+(ya-(Pyh+xb(1)*sin(-alph)+xb(2)*cos(-alph)))^2;


zhat(1) = distSq;


H = zeros(1,8);
H(1,1) = -2*(xa-(Pxh+xb(1)*cos(-alph)-xb(2)*sin(-alph)));
H(1,2) = -2*(ya-(Pyh+xb(1)*sin(-alph)+xb(2)*cos(-alph)));
H(1,7) = 2*(xa-(Pxh+xb(1)*cos(-alph)-xb(2)*sin(-alph)))*(xb(1)*sin(-alph)-xb(2)*cos(-alph))+2*(ya-(Pyh+xb(1)*sin(-alph)+xb(2)*cos(-alph)))*(xb(1)*cos(-alph)+xb(2)*sin(-alph));


