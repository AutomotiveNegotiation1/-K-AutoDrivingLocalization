
function [dPhi, dTheta, dG] = CalcDiffEuler(phi, theta, g, A)
CT = cos(theta); CP = cos(phi);
ST = sin(theta); SP = sin(phi);

Ptrans = [  0           g*CT        ST ;
            -g*CP*CT     g*ST*SP      -CT*SP;
            g*CT*SP     g*ST*CP     -CT*CP];

Result = inv(Ptrans'*Ptrans)*Ptrans'*A;
dPhi = Result(1);
dTheta = Result(2);
dG = Result(3);



