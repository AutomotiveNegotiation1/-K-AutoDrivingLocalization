function [PositionOut] = PositioningSystem_V5_1(PositionVector, MapParam)

persistent s_time_prev  s_time_vec xt_b yt_b P FiFoUWBpos FiFoUWBhead FiFoUWBtime FiFoSLAMpos FiFoSLAMhead FiFoSLAMtime FiFoSLAMposN FiFoSLAMheadN FiFoSLAMEulDiff
persistent  PrevPosHF PrevHeadingHF firstV RotMatn SLAMposPrev SLAMinitPos SLAMinitHead UWBPosAcc UWBPosInitAcc SLAMPosAcc sAFlag sA RotMatInit QuatInit SLAMposInit s SLAMSet
persistent FiFoIMUacc FiFoIMUgyro FiFoIMUtime
persistent PosUWBIMU HeadUWBIMU GammUWBIMU BetaUWBIMU
persistent grav FiFoSLAMUWBpos FiFoSLAMUWBhead FiFoSLAMUWBtime CAMPos diff_Heading
Front = 0;

if isempty (s_time_prev)
    s_time_prev = 0;
    PrevPosHF = 0+0*j;
    PrevHeadingHF = 0;

    s_time_vec = zeros(1,4);
    P = eye(15);

    FiFoUWBpos = zeros(1,100)+0*j;
    FiFoUWBhead = zeros(1,100);
    FiFoUWBtime = zeros(1,100);

    FiFoSLAMpos = zeros(3,100);
    FiFoSLAMhead = zeros(4,100);
    FiFoSLAMtime = zeros(1,100);

    FiFoSLAMUWBpos = zeros(1,100)+0*j;
    FiFoSLAMUWBhead = zeros(1,100);
    FiFoSLAMUWBtime = zeros(1,100);

    FiFoIMUacc = zeros(3,4);
    FiFoIMUgyro = zeros(3,4);
    FiFoIMUtime = zeros(1,4);

    FiFoSLAMposN = zeros(3,100);
    FiFoSLAMEulDiff = zeros(3,100);
    FiFoSLAMheadN = zeros(1,100);

    SLAMinitPos = [0;0;0];
    SLAMinitHead = 0;
    firstV = 0;

    UWBPosAcc = 0;
    UWBPosInitAcc = 0;
    SLAMPosAcc = 0;

    sAFlag = 0;

    QuatInit = zeros(1,4);
    SLAMSet = 0;
    diff_Heading = zeros(1,3);

    grav = 9.85;
    if Front == 1
        CAMPos = (-0.065+0.77*j);
    else
        CAMPos = (-0.06-0.77*j);
    end
end

% change (08.21. by joo) Extract the MapParam 
s = MapParam(1);
roll = MapParam(2);
pitch = MapParam(3); 
yaw = MapParam(4); 

sA = [MapParam(5) MapParam(6) MapParam(7); MapParam(8) MapParam(9) MapParam(10); MapParam(11) MapParam(12) MapParam(13)]; 

SLAMposInit = [MapParam(14);MapParam(15);MapParam(16)];  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

kk = 1;
s_time = PositionVector(kk); kk = kk + 1;
SensorNum = PositionVector(kk); kk = kk + 1;

if SensorNum == 5
    PosHF = 0;
    HeadingHF = 0;
    PosHF5 = 0;
    HeadingHF5 = 0;
    GammHF = 0;
    BetaHF = 0;
    PosH = 0;
    HeadingH = 0;
    dt = 5e-3;

    IMUtime = s_time;
    IMUacc_c = PositionVector(3:5);
    IMUgyro_c = PositionVector(6:8);

    if IMUacc_c(1)~=0

        FiFoIMUacc = [FiFoIMUacc(:,2:end) transpose(IMUacc_c)];
        FiFoIMUgyro = [FiFoIMUgyro(:,2:end) transpose(IMUgyro_c)];

        GyroD = (FiFoIMUgyro(:,end));
        acc = (FiFoIMUacc(:,end));
        acc(3) = acc(3) + grav;

        %[PosHF5, HeadingHF5, GammHF5, BetaHF5] = EKF_UWB_SLAM_IMU_1(dt, acc, GyroD, IMUtime, FiFoSLAMUWBpos(end), FiFoSLAMUWBhead(end), FiFoSLAMUWBtime(end), grav);
  
    end
elseif SensorNum == 6 %% GPS
    PosHF = 0;
    HeadingHF = 0;
    GammHF = 0;
    BetaHF = 0;
    PosH = 0;
    HeadingH = 0;
    PosHF5 = 0;
    HeadingHF5 = 0;

elseif SensorNum == 7 %% SLAM
    PosHF = 0;
    HeadingHF = 0;
    GammHF = 0;
    BetaHF = 0;
    PosH = 0;
    HeadingH = 0;
    PosHF5 = 0;
    HeadingHF5 = 0;

    SLAMpos = PositionVector(3:5);
    SLAMorient = PositionVector(6:9);


    if (SLAMpos(1)~=0) && ((FiFoSLAMtime(end)-s_time)<-1e-3)
        FiFoSLAMpos = [FiFoSLAMpos(:,2:end) transpose(SLAMpos)];
        FiFoSLAMhead = [FiFoSLAMhead(:,2:end) transpose(SLAMorient)];
        FiFoSLAMtime = [FiFoSLAMtime(2:end) s_time];

        L = length(FiFoSLAMtime(FiFoSLAMtime~=0));

        lagT = 50e-3;

        if (FiFoSLAMtime(end-L+1)< (s_time-lagT)) && (L>2)
            curr_pos = zeros(1,3);
            prev_pos = zeros(1,3);

            curr_pos(1) = interp1(FiFoSLAMtime(end-L+1:end),FiFoSLAMpos(1,end-L+1:end),FiFoSLAMtime(end)-lagT,'spline');
            curr_pos(2) = interp1(FiFoSLAMtime(end-L+1:end),FiFoSLAMpos(2,end-L+1:end),FiFoSLAMtime(end)-lagT,'spline');
            curr_pos(3) = interp1(FiFoSLAMtime(end-L+1:end),FiFoSLAMpos(3,end-L+1:end),FiFoSLAMtime(end)-lagT,'spline');
            if (FiFoSLAMtime(end-L+1)< (FiFoSLAMtime(end-1)-lagT))
                prev_pos(1) = interp1(FiFoSLAMtime(end-L+1:end),FiFoSLAMpos(1,end-L+1:end),FiFoSLAMtime(end-1)-lagT,'spline');
                prev_pos(2) = interp1(FiFoSLAMtime(end-L+1:end),FiFoSLAMpos(2,end-L+1:end),FiFoSLAMtime(end-1)-lagT,'spline');
                prev_pos(3) = interp1(FiFoSLAMtime(end-L+1:end),FiFoSLAMpos(3,end-L+1:end),FiFoSLAMtime(end-1)-lagT,'spline');
            else
                prev_pos = curr_pos;
            end

        else
            curr_pos = transpose(FiFoSLAMpos(:,end));
            prev_pos = curr_pos;
        end

        SLAMtime = s_time;

        if  (SLAMpos(1)~=0)

            if firstV == 0
                if FiFoUWBpos(end)~=0
                    RotMatInit = eye(3);
                    QuatInit = SLAMorient;
                    firstV = 1;
                end
            else

                curr_quat = SLAMorient;

                prev_quat = transpose(FiFoSLAMhead(:,end-1));

                prev_pos_org = transpose(prev_pos);
                curr_pos_org = transpose(curr_pos);
                pos_diff_org = curr_pos_org - prev_pos_org;                
                RotEul_diff = mod(transpose(equPlane(curr_quat)-equPlane(prev_quat)+pi),2*pi)-pi;

                if abs(RotEul_diff)>pi/8
                    if RotEul_diff>0
                        RotEul_diff = RotEul_diff-2*pi;
                    else
                        RotEul_diff = RotEul_diff+2*pi;
                    end
                end

                if sqrt(sum(abs(pos_diff_org).^2))<1

                else
                    RotMat_diff = eye(3);
                    RotEul_diff = dcm2eulr(RotMat_diff);
                    pos_diff_org = zeros(3,1);
                    QuatInit = SLAMorient;
                    firstV = 2;
                end

                FiFoSLAMEulDiff = [FiFoSLAMEulDiff(:,2:end) (RotEul_diff)];
              
                if (FiFoSLAMtime(end-1)<(FiFoUWBtime(end)+lagT))

                    tempEulr = equPlane(SLAMorient);
                    PosSLAM = sA*transpose(SLAMpos([1 3 2]))+SLAMposInit;

                    [EKFpos,EKFhead] = EKF_UWB_SLAM_4(FiFoUWBpos(end)+CAMPos*exp(j*(FiFoUWBhead(end))), -FiFoUWBhead(end), pos_diff_org, RotEul_diff, QuatInit, PosSLAM, tempEulr-diff_Heading, MapParam);
                    diff_Heading = tempEulr-EKFhead;

                else
                 
                    tempEulr = equPlane(SLAMorient);
                    
                    PosSLAM = sA*transpose(SLAMpos([1 3 2]))+SLAMposInit;
                    QuatInit = SLAMorient;
                 
                    [EKFpos,EKFhead] = EKF_UWB_SLAM_4(PosSLAM(1)+j*PosSLAM(2),tempEulr(2)-diff_Heading(2), pos_diff_org, RotEul_diff, QuatInit, PosSLAM, tempEulr-diff_Heading, MapParam);
                        
                    diff_Heading = tempEulr-EKFhead;

                end

                if (SLAMSet==0)
                    if (EKFpos~=0) && (abs(EKFpos-(FiFoUWBpos(end)+CAMPos*exp(j*(FiFoUWBhead(end)))))<0.5) && (FiFoUWBpos(end)~=0) 
                        SLAMSet = 1;

                        PosHF = EKFpos-CAMPos*exp(-j*EKFhead(2));
                        HeadingHF = EKFhead(2);
                        FiFoSLAMUWBpos = [FiFoSLAMUWBpos(2:end) PosHF];
                        FiFoSLAMUWBhead = [FiFoSLAMUWBhead(2:end) HeadingHF];
                        FiFoSLAMUWBtime = [FiFoSLAMUWBtime(2:end) s_time];

                        PrevHeading = EKFhead;
                        PrevHeadingHF = EKFhead(2);
                    else
                        PosHF = 0;
                        HeadingHF = 0;
                        PrevHeading = 0;
                        PrevHeadingHF = 0;
                    end
                else
                    
                    PosHF = EKFpos-CAMPos*exp(-j*EKFhead(2));
                    HeadingHF = EKFhead(2);
                        

                    FiFoSLAMUWBpos = [FiFoSLAMUWBpos(2:end) PosHF];
                    FiFoSLAMUWBhead = [FiFoSLAMUWBhead(2:end) HeadingHF];
                    FiFoSLAMUWBtime = [FiFoSLAMUWBtime(2:end) s_time];
                    PrevHeading = EKFhead;
                    PrevHeadingHF = EKFhead(2);
                end
            end
        end
    end

else %% UWB
    PosHF5 = 0;
    HeadingHF5 = 0;

    d_uwb = s_time - s_time_prev ;

    s_time_vec = [s_time_vec(2:4) s_time];

    Ln = PositionVector(kk); kk = kk + 1;
    LnC = PositionVector(kk); kk = kk + 1;
    Nanchor = PositionVector(kk); kk = kk + 1;
    RxIDUWB = PositionVector(kk:kk+Nanchor-1); kk = kk + Ln;
    RxDistOrig = PositionVector(kk:kk+Nanchor-1); kk = kk + 4;

    xain = PositionVector(kk:kk+LnC-1);  kk = kk + LnC;
    yain = PositionVector(kk:kk+LnC-1);  kk = kk + LnC;
    zain = PositionVector(kk:kk+LnC-1);  kk = kk + LnC;

    xt_b = PositionVector(kk:kk+4-1);  kk = kk + 4;
    yt_b = PositionVector(kk:kk+4-1);  kk = kk + 4;
    zt_b = PositionVector(kk); kk = kk + 1;

    

    [PosH, HeadingH, UWBAnc_Full] = UWBPosition_V4_1(s_time, Ln, LnC, Nanchor, SensorNum, RxIDUWB, RxDistOrig, xain, yain, zain, xt_b, yt_b, zt_b, PrevPosHF, PrevHeadingHF);

    if PosH~=0

        FiFoUWBpos = [FiFoUWBpos(2:end) PosH];
        FiFoUWBhead = [FiFoUWBhead(2:end) HeadingH];
        FiFoUWBtime = [FiFoUWBtime(2:end) s_time];
    end

    if SLAMSet == 0
        PosHF = PosH;
        HeadingHF = -HeadingH;
        if PosHF~=0
            FiFoSLAMUWBpos = [FiFoSLAMUWBpos(2:end) PosHF];
            FiFoSLAMUWBhead = [FiFoSLAMUWBhead(2:end) HeadingHF];
            FiFoSLAMUWBtime = [FiFoSLAMUWBtime(2:end) s_time];
        end

    else
        PosHF = 0;
        HeadingHF = 0;
    end
    GammHF = 0;
    BetaHF = 0;

    s_time_prev = s_time;
end

Uncertainty = P;
PositionOut = [real(PosHF), imag(PosHF), 0, HeadingHF, GammHF, BetaHF, Uncertainty(1,1), real(PosH), imag(PosH), HeadingH  ];

PrevPosHF = PosHF;
PrevHeadingHF = HeadingHF;


