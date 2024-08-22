function [Posn,Headn] = EKF_UWB_SLAM_4(PosUWB, HeadUWB, dPosSLAM, dHeadSLAM, QuatInit, PosSLAMc, HeadSLAMc, MapParam)

persistent PosP roll pitch yaw s H P Q R
persistent Hroll Hpitch Hyaw 
persistent firstRun
persistent totHead totA totHUWB totdHeadSLAM totdPosSLAM ParkingLot totPosP  totPosUWB UWBless sOrg rollOrg pitchOrg yawOrg

Front = 0;
if isempty(firstRun)
    roll = 0;
    pitch = 0;
    yaw = 0;
    Hroll = 0;
    Hpitch = 0;
    Hyaw = 0;
    UWBless = 0;

    totPosP = zeros(1,5)+0j;
   
    H = [1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0;
        0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0;
        0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0;
        0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0;
        0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0];

    PosP = zeros(3,1);
    Q = 1e-5*ones(16,16);
    
    Temp = Ajacob(ones(1,16));
    Q(Temp==0)=0;

    R = 100*eye(size(H,1),size(H,1));

    P = ones(16,16);
    P(Temp==0)=0;

    firstRun = 1;

    if Front == 0 
        %MapParam = [s roll pitch yaw sA_00 sA_01 sA_02 sA_10 sA_11 sA_12 sA_20 sA_21 sA_22 SLAMposInit_0 SLAMposInit_1 SLAMposInit_2];

        sOrg = MapParam(1);
        rollOrg = MapParam(2);
        pitchOrg = MapParam(3);
        yawOrg = MapParam(4);

        s = sOrg;
        roll =  rollOrg;
        pitch = pitchOrg;
        yaw =  yawOrg;

    end
end

if (PosUWB~=0)
    if (PosP(1)==0)
        PosP = PosSLAMc;
        if Front == 0
        s = sOrg;
        roll =  rollOrg;
        pitch = pitchOrg;
        yaw =  yawOrg;
        end
       
        Hroll = roll;
        Hpitch = pitch;
        Hyaw = yaw;
    
    elseif (dPosSLAM(1)==0)
 
    if Front == 0 
                
        s = sOrg;
        roll =  rollOrg;
        pitch = pitchOrg;
        yaw =  yawOrg;
    end

    elseif (sqrt(sum(dHeadSLAM.^2)) > 0.5) || (sqrt(sum(dPosSLAM.^2)) > 0.1)
    
        dPosSLAM = [0; 0; 0];
        dHeadSLAM = [0; 0; 0];

    if Front == 0 
        s = sOrg;
        roll =  rollOrg;
        pitch = pitchOrg;
        yaw =  yawOrg;
    end
       
    end
end

FigOK = 0;

xhat = [PosP(1) PosP(2) PosP(3) dPosSLAM(1) dPosSLAM(2) dPosSLAM(3) roll pitch yaw s Hroll Hpitch Hyaw dHeadSLAM(1) dHeadSLAM(2) dHeadSLAM(3)];
xp = fx(xhat);

if (PosUWB~=0)
    A = Ajacob(xhat);

    if (HeadUWB-Hpitch)>2*pi
        Nh = floor((HeadUWB-Hpitch)/(2*pi));
        HeadUWB = HeadUWB - 2*pi*Nh;
    elseif (Hpitch-HeadUWB) > 2*pi
        Nh = floor((Hpitch - HeadUWB)/(2*pi));
        HeadUWB = HeadUWB + 2*pi*Nh;
    end

    if (HeadUWB-Hpitch) > pi
        HeadUWB = HeadUWB -2*pi;
    elseif (HeadUWB-Hpitch) < -pi
        HeadUWB = HeadUWB + 2*pi;
    end

    z = [real(PosUWB);0;imag(PosUWB);0;HeadUWB;0];

    Pp = A*P*A'+Q;
    K = Pp*H'*inv(H*Pp*H'+R);

    x = xp + K*(z-H*xp);
    P = Pp - K*H*Pp;
    UWBless = 0;
else
    UWBless = UWBless + 1;
    x = xp;
  
    TT1 = totPosP(end)-totPosP(1);
    if Front == 0
        TT1 = TT1 + pi;
    end

    TT2 = mod(atan2(real(TT1),imag(TT1))+2*pi,2*pi);
    
    if UWBless > 3

    x(1) = PosSLAMc(1);
    x(2) = PosSLAMc(3);
    x(3) = PosSLAMc(2);

    x(11) = HeadSLAMc(1);
    x(12) = HeadSLAMc(2);
    x(13) = HeadSLAMc(3);

    end
    
end

PosP = [x(1);x(2);x(3)];

roll = x(7);
pitch = x(8);
yaw = x(9);
s = x(10);

Hroll = x(11);
Hpitch = x(12);
Hyaw = x(13);

Posn = PosP(1)+j*PosP(3);
Headn = [Hroll Hpitch Hyaw];

totPosP = [totPosP(2:end) PosP(1)+j*PosP(3)];

if FigOK == 1
figure(123411);hold off;
    imshow(ParkingLot);
ScaleY = 24.2;
ScaleX = 24.45;
xoffset = 0.1552;
yoffset = 5.0516;

xa = [33.8500   36.6500   26.7500   26.7500   16.8500   16.8500   11.7000   6.95];
ya = [22.3100   -1.0500   22.3100   -0.9000   23.7100   -0.9000         0  23.71];

hold on;plot(ScaleX*(xa)+xoffset*ScaleX, (size(ParkingLot,1)-ScaleY*(ya+yoffset)),'rsquare');
plot(ScaleX*(PosP(1)+xoffset),size(ParkingLot,1)-ScaleY*(PosP(3)+yoffset),'ro')

xarrow = -1*ScaleX*[-0.8 0.8 0 -0.8];
yarrow = -1*ScaleY*[-1 -1 1 -1];
arrowc = xarrow+j*yarrow;
arrowHeading = arrowc*exp(j*(Hpitch));
hold on;plot(real(arrowHeading)+ScaleX*real(Posn)+xoffset*ScaleX,imag((arrowHeading))+(size(ParkingLot,1)-ScaleY*imag(Posn)-ScaleY*yoffset));

axis equal
end

function xp = fx(xhat)
xn_1 = xhat(1);yn_1 = xhat(2);zn_1 = xhat(3);dx = xhat(4);dy = xhat(5);dz = xhat(6);
theta = xhat(7);gama = xhat(8);phi = xhat(9); sn_1 = xhat(10);
Htheta = xhat(11);Hgama = xhat(12);Hphi = xhat(13);
dHtheta = xhat(14); dHgama = xhat(15); dHphi = xhat(16);

xn = xn_1 + sn_1*(cos(theta)*cos(gama)*dx-sin(theta)*cos(phi)*dy+cos(theta)*sin(gama)*sin(phi)*dy+sin(theta)*sin(phi)*dz+cos(theta)*sin(gama)*cos(phi)*dz);
yn = yn_1 + sn_1*(sin(theta)*cos(gama)*dx+cos(theta)*cos(phi)*dy+sin(theta)*sin(gama)*sin(phi)*dy-cos(theta)*sin(phi)*dz+sin(theta)*sin(gama)*cos(phi)*dz);
zn = zn_1 + sn_1*(-sin(gama)*dx+cos(gama)*sin(phi)*dy+cos(gama)*cos(phi)*dz);

Hthetan = Htheta + mod(dHtheta+pi/2,pi)-pi/2;
Hgaman = Hgama + mod(dHgama+pi,2*pi)-pi;
Hphin = Hphi + mod(dHphi+pi/2,pi)-pi/2;

xp = transpose([xn yn zn dx dy dz theta gama phi sn_1 Hthetan Hgaman Hphin dHtheta dHgama dHphi]);


%------------------------
function A = Ajacob(xhat)
xn_1 = xhat(1);yn_1 = xhat(2);zn_1 = xhat(3);dx = xhat(4);dy = xhat(5);dz = xhat(6);
theta = xhat(7);gama = xhat(8);phi = xhat(9); sn_1 = xhat(10);
Htheta = xhat(11);Hgama = xhat(12);Hphi = xhat(13);
dHtheta = xhat(14); dHgama = xhat(15); dHphi = xhat(16);

A = eye(length(xhat));
A(1,4) = sn_1*cos(theta)*cos(gama);
A(1,5) = sn_1*(-sin(theta)*cos(phi)+cos(theta)*sin(gama)*sin(phi));
A(1,6) = sn_1*(sin(theta)*sin(phi)+cos(theta)*sin(gama)*cos(phi));
A(1,7) = sn_1*(-sin(theta)*cos(gama)*dx-cos(theta)*cos(phi)*dy-sin(theta)*sin(gama)*sin(phi)*dy+cos(theta)*sin(phi)*dz-sin(theta)*sin(gama)*cos(phi)*dz);
A(1,8) = sn_1*(cos(theta)*(-sin(gama))*dx+cos(theta)*cos(gama)*sin(phi)*dy+cos(theta)*cos(gama)*cos(phi)*dz);
A(1,9) = sn_1*(sin(theta)*sin(phi)*dy+cos(theta)*sin(gama)*cos(phi)*dy+sin(theta)*cos(phi)*dz-cos(theta)*sin(gama)*sin(phi)*dz);
A(1,10) = (cos(theta)*cos(gama)*dx-sin(theta)*cos(phi)*dy+cos(theta)*sin(gama)*sin(phi)*dy+sin(theta)*sin(phi)*dz+cos(theta)*sin(gama)*cos(phi)*dz);
A(2,4) = sn_1*(sin(theta)*cos(gama));
A(2,5) = sn_1*(+cos(theta)*cos(phi)+sin(theta)*sin(gama)*sin(phi));
A(2,6) = sn_1*(-cos(theta)*sin(phi)+sin(theta)*sin(gama)*cos(phi));
A(2,7) = sn_1*(cos(theta)*cos(gama)*dx-sin(theta)*cos(phi)*dy+cos(theta)*sin(gama)*sin(phi)*dy+sin(theta)*sin(phi)*dz+cos(theta)*sin(gama)*cos(phi)*dz);
A(2,8) = sn_1*(sin(theta)*(-sin(gama))*dx+sin(theta)*cos(gama)*sin(phi)*dy+sin(theta)*cos(gama)*cos(phi)*dz);
A(2,9) = sn_1*(-cos(theta)*sin(phi)*dy+sin(theta)*sin(gama)*cos(phi)*dy-cos(theta)*cos(phi)*dz-sin(theta)*sin(gama)*sin(phi)*dz);
A(2,10) = (sin(theta)*cos(gama)*dx+cos(theta)*cos(phi)*dy+sin(theta)*sin(gama)*sin(phi)*dy-cos(theta)*sin(phi)*dz+sin(theta)*sin(gama)*cos(phi)*dz);
A(3,4) = sn_1*(-sin(gama));
A(3,5) = sn_1*(cos(gama)*sin(phi));
A(3,6) = sn_1*(cos(gama)*cos(phi));
A(3,8) = sn_1*(-cos(gama)*dx-sin(gama)*sin(phi)*dy-sin(gama)*cos(phi)*dz);
A(3,9) = sn_1*(cos(gama)*cos(phi)*dy-cos(gama)*sin(phi)*dz);
A(3,10) = (-sin(gama)*dx+cos(gama)*sin(phi)*dy+cos(gama)*cos(phi)*dz);
A(11,14) = 1;
A(12,15) = 1;
A(13,16) = 1;



