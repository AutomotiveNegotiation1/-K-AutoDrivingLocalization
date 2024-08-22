function EulCurr = equPlane(quav)

% if abs(abs(eulv(1))-pi)<pi/4
%     EulCurr = eulv;
% else
%     eulvT = [eulv(1) -eulv(2) eulv(3)-pi];
%     EulCurr = [-eulvT(1) eulvT(2)+pi eulvT(3)];
% end
DCMTempO = qua2dcm(quav);

EulPi = zeros(8,3);
for k=-4:3
    DCMTempPi = DCMTempO*(eulr2dcm([0; k*pi/4; 0]));
    EulPi(k+5,:) = dcm2eulr(DCMTempPi)+[0;k*pi/4;0];
end

Qq = abs(mod(EulPi(:,1)+pi,2*pi)-pi)+abs(mod(EulPi(:,3)+pi,2*pi)-pi);
[v,ind] = min(Qq);

EulCurr = mod(EulPi(ind,:)+pi,2*pi)-pi;