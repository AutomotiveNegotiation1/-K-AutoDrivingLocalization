function [Pos1,Pos2] = UWBpos_V2_2(Nanchor, RxIDin, RxDistin, xain, yain)

RxID = RxIDin(1:Nanchor);
xa = xain(RxID);
ya = yain(RxID);

RxDist = RxDistin(1:Nanchor);

v = 1;
if Nanchor == 2
    [Pos] = TwoAnchPos5(xa, ya, RxDist);
    Pos1 = 0;
    Pos2(1) = Pos(1,1)+j*Pos(1,2);
    Pos2(2) = Pos(2,1)+j*Pos(2,2);
    
elseif Nanchor >= 3
    [Pos] = ThreeAnchPos4(xa, ya, RxDist);
    Pos1 = Pos(1)+j*Pos(2);

    for k = 1 : Nanchor - 1
        for l = k+1: Nanchor
            [Pos] = TwoAnchPos5(xa([k,l]), ya([k,l]), RxDist([k,l]));
            PosC1 = Pos(1,1)+j*Pos(1,2);
            PosC2 = Pos(2,1)+j*Pos(2,2);
            if abs(Pos1-PosC1)>abs(Pos1-PosC2)
                Pos2(v) = PosC2;
            else
                Pos2(v) = PosC1;
                
            end
            v = v + 1;
        end
    end
else 
    Pos1 = 0;
    Pos2 = 0;
end

