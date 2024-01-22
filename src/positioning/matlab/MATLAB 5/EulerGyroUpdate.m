function [phi, theta, psi] = EulerGyroUpdate(p,q,r,dt,phi_p, theta_p, psi_p)

UpdateMat = [1 sin(phi_p)*tan(theta_p) cos(phi_p)*tan(theta_p);0 cos(phi_p) -sin(phi_p);0 sin(phi_p)/cos(theta_p) cos(phi_p)/cos(theta_p)];

NextEuler = [phi_p;theta_p;psi_p] + dt*UpdateMat*[p;q;r];

phi = NextEuler(1);
theta = NextEuler(2);
psi = NextEuler(3);
