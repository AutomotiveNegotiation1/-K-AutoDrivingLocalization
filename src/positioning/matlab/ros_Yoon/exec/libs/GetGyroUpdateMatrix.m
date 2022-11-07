function UpdateMat = GetGyroUpdateMatrix(PhiP, ThetaP)
UpdateMat = [1 sin(PhiP)*tan(ThetaP) cos(PhiP)*tan(ThetaP);0 cos(PhiP) -sin(PhiP);0 sin(PhiP)/cos(ThetaP) cos(PhiP)/cos(ThetaP)];