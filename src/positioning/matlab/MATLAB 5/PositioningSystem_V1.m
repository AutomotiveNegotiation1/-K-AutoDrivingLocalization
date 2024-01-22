function [PosHF, HeadingHF, Uncertainty] = PositioningSystem_V1(PositionVector)

persistent s_time_prev prev_state IMUacc_c_fifo IMUgyro_c_fifo XhatUWB XhatIMU PrevPosHUWB PrevHeadingHUWB s_time_vec PosHR AnchorR UWBDataSet prevUWBDataSet xt_b yt_b P UWBAnc_Full

if isempty (s_time_prev)
    s_time_prev = 0;
    IMUacc_c_fifo = zeros(4,3);
    IMUgyro_c_fifo = zeros(4,3);
    XhatUWB = zeros(1,8);
    XhatIMU = zeros(1,8);
    
    prev_state = 0; % 1 : UWB, 2 : IMU
    PrevPosHUWB = 0;
    PrevHeadingHUWB = 0;
    s_time_vec = zeros(1,4);
    PosHR = zeros(1,10);
    AnchorR= zeros(1,10);

    UWBDataSet = 0;
    prevUWBDataSet = 0;

    xt_b = [-0.525 0.525 -0.525 0.525];
    yt_b = [0.505 0.505 -0.505 -0.505];

    P = eye(8);
    UWBAnc_Full = 0;

end

dT = 0.03;
kk = 1;
s_time = PositionVector(kk); kk = kk + 1;
SensorNum = PositionVector(kk); kk = kk + 1;

if SensorNum == 5
    IMUacc_c = PositionVector(3:5);
    IMUgyro_c = PositionVector(6:8);
    
    IMUacc_c_fifo = [IMUacc_c_fifo(2:4,:) ;IMUacc_c];
    IMUgyro_c_fifo = [IMUgyro_c_fifo(2:4,:) ;IMUgyro_c];

    GyroD = mean(IMUgyro_c_fifo)*pi/180;
    acc = mean(IMUacc_c_fifo);

    if prev_state == 1   
        XhatIMU = XhatUWB;
        prev_state = 2;
        XhatIMU = PredEKF_Simple(XhatIMU,acc,GyroD,dT);
    else
        XhatIMU = PredEKF_Simple(XhatIMU,acc,GyroD,dT);

    end


    if (prevUWBDataSet ~= 0) && (UWBDataSet ~= 0) && ((s_time-UWBDataSet > 0.02)||(UWBAnc_Full==1)) && (UWBDataSet - prevUWBDataSet < 1)
        dt_uwb = UWBDataSet - prevUWBDataSet;
        Xbar = PredEKF_Simple(XhatUWB,acc,GyroD,dt_uwb);
        A = makePredA_Simple(XhatUWB,acc,GyroD,dt_uwb);
        [Zv,H] = EstEKF_Center_Simple(Xbar);
        Q = eye(8)*1e-7;
        Q(1:4,1:4) = eye(4)*1e-5;
        R = eye(3)*5e-3;
        P = A*P*transpose(A)+Q;
        K = P*transpose(H)*inv(H*P*transpose(H)+R);
        XhatUWB = transpose(Xbar) + K*(transpose([-PrevHeadingHUWB real(PrevPosHUWB) imag(PrevPosHUWB)])-transpose(Zv));
        P = P - K*H*P;

        prevUWBDataSet = UWBDataSet;
        prev_state = 1;
        PosHF = XhatUWB(1)+XhatUWB(2)*j;
        HeadingHF = XhatUWB(7);

    elseif (UWBDataSet ~= 0) && ((s_time-UWBDataSet > 0.02)||(UWBAnc_Full==1))
        prevUWBDataSet = UWBDataSet;
        XhatIMU(1) = real(PrevPosHUWB);
        XhatIMU(2) = imag(PrevPosHUWB);
        XhatIMU(7) = -PrevHeadingHUWB;
                
        XhatUWB = XhatIMU;
        prev_state = 1;
        PosHF = XhatUWB(1)+XhatUWB(2)*j;
        HeadingHF = XhatUWB(7);
    else
        PosHF = XhatIMU(1)+XhatIMU(2)*j;
        HeadingHF = XhatIMU(7);
    end


    % PosH = Xbar(1)+Xbar(2)*j;
    % HeadingH = Xbar(7);
    % PosH = 0;
    % HeadingH = PrevHeadingHUWB;
    PrevHeadingHUWB = -HeadingHF;
    PrevPosHUWB = PosHF;
else
    % PositionVector = [PositionVector Nanchor RxIDUWB RxDistOrig xain(RxIDUWB) yain(RxIDUWB) za(RxIDUWB)];
    d_uwb = s_time - s_time_prev ;
    
    s_time_vec = [s_time_vec(2:4) s_time];
    
    LnC = PositionVector(kk); kk = kk + 1;
    Nanchor = PositionVector(kk); kk = kk + 1;
    RxIDUWB = PositionVector(kk:kk+Nanchor-1); kk = kk + Nanchor;
    RxDistOrig = PositionVector(kk:kk+Nanchor-1); kk = kk + Nanchor;
    
    xain = PositionVector(kk:kk+LnC-1);  kk = kk + LnC;
    yain = PositionVector(kk:kk+LnC-1);  kk = kk + LnC;
    zain = PositionVector(kk:kk+LnC-1);  kk = kk + LnC;
    
    xt_b = PositionVector(kk:kk+4-1);  kk = kk + 4;
    yt_b = PositionVector(kk:kk+4-1);  kk = kk + 4;
    zt_b = PositionVector(kk); kk = kk + 1;

    PrevHeading = PrevHeadingHUWB; 

    xa = xain(RxIDUWB);
    ya = yain(RxIDUWB);

    RxDistUWB = real(sqrt(RxDistOrig.^2-(zain(1)-zt_b)^2));

    
    if Nanchor > 1
        [Pos1, Pos2] = UWBpos_V2_2(Nanchor, RxIDUWB, RxDistUWB, xain, yain);
        if Pos1 == 0
            [Minv, MinI] = min(abs(PrevPosHUWB-Pos2));
            Pos1 = Pos2(MinI);
            PosH = Pos2(MinI) - ((xt_b(SensorNum)*cos(PrevHeading)-yt_b(SensorNum)*sin(PrevHeading))+j*(xt_b(SensorNum)*sin(PrevHeading)+yt_b(SensorNum)*cos(PrevHeading)));
        else
            PosH = Pos1 - ((xt_b(SensorNum)*cos(PrevHeading)-yt_b(SensorNum)*sin(PrevHeading))+j*(xt_b(SensorNum)*sin(PrevHeading)+yt_b(SensorNum)*cos(PrevHeading)));
        end
        
    else
        PosH = 0;
        Pos1 = 0;
    end

    PosHR = [PosHR(2:end) PosH];
    AnchorR = [AnchorR(2:end) SensorNum];
    [DistMap, StimeMap, NumTags, PosMap] = MakeDistMap(s_time, LnC, RxDistUWB, RxIDUWB, SensorNum, Pos1);
    

    if length(PosMap(:,1)~=0) == 4
        UWBAnc_Full = 1;
    else
        UWBAnc_Full = 0;
    end

    flag = 0;
    Heading = PrevHeadingHUWB;
    for kk = 1 : 1
        for ll = 1 : 4
            if PosMap(ll,kk) ~=0
                if flag == 0
                    RefPos = PosMap(ll,kk);
                    RefPosBody = xt_b(ll)+j*yt_b(ll);
                    flag = 1;
                else
                    PosBody = xt_b(ll)+j*yt_b(ll);
                    Heading(flag) = angle((PosMap(ll,kk)-RefPos)/(PosBody-RefPosBody));
                    flag = flag + 1;
                end
            end
        end
    end

    HeadingH = meanAngle(PrevHeadingHUWB,Heading);
    PrevHeadingHUWB = HeadingH;

    [PosHT] = UWBMultiTagPos(DistMap, NumTags, HeadingH, xain, yain, xt_b, yt_b);
    if PosHT ~= 0
        PosH = PosHT;
    end

    if PosH ~= 0
        UWBDataSet = s_time;
        PrevPosHUWB = PosH;
    else
        UWBDataSet = 0;
    end

    if abs(PosH) > 50
        dd = 1;
    end
    % PosHF = PrevPosHUWB;
    % HeadingHF = -PrevHeadingHUWB;
    PosHF = 0;
    HeadingHF = 0;
end

Uncertainty = P;    




