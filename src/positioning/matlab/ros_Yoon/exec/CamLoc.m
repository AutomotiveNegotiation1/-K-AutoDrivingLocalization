% clear all;
% clc
% warning off

cam = webcam(1);
RefChecker = checkerboard(26,4,3);

[imagePointsRef,boardSize] = detectCheckerboardPoints(RefChecker);


marker_pos = [0 0 0;0 1825 3650];
figure(2);hold off;
plot(marker_pos(1,:),marker_pos(2,:),'Rd');

% function pos = CamLoc(marker_pos, imagePointsRef)

CamLocTest(cam, imagePointsRef);
