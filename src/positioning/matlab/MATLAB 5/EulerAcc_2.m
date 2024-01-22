function [phi, theta] = EulerAcc_2(ax, ay, az,g)

theta = asin(ax/g);
phi = asin(-ay/g/cos(theta));
