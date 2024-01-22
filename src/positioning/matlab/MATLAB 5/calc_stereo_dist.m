function [distI,distQ,angI,angQ] = calc_stereo_dist(M,N,FoV,LensDist, IPoints_Location,QPoints_Location)

theta = FoV;

theta1 = atan((sin(FoV))/(IPoints_Location(1)/(M-IPoints_Location(1))+cos(FoV)));
theta2 = atan((sin(FoV))/(QPoints_Location(1)/(M-QPoints_Location(1))+cos(FoV)));

distI = LensDist*sin(pi/2+theta/2-theta1)/sin(theta1-theta2);
distQ = LensDist*sin(pi/2-theta/2+theta2)/sin(theta1-theta2);

angI = (pi-theta)/2+theta1;
angQ = (pi-theta)/2+theta2;


