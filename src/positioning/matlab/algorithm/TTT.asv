clear all;

xa = [10 10 -10 -10];
ya = [10 -10 -10 10];
anch_pos = xa+j*ya;
Lp = 4;
Ln = 4;

dist = [      18.7019   17.7938   18.5801   18.3603;
    8.7174    8.1440    8.3043    7.4637;
   15.3726   16.2854   15.3044   16.0912;
   19.7530   19.6660   21.0351   20.8112];

dist_o = [  18.3423   17.6371   19.0510   18.3730;
    9.1560    8.4458    8.5124    7.7434;
   12.1885   13.1883   12.2499   13.2450;
   20.0292   20.3388   20.9890   21.2847];

Xt_c_e =    -2.9489;
Yt_c_e =    7.1308;

Xt_c_e =        4.4318;
Yt_c_e =    -4.2278;

tag_pos_est = [3.7585-4.0117i   4.6479-3.5545i   4.2157-4.9010i   5.1051-4.4439i];
A1 = [4 5 6];
A2 = [2 3 6];
A3 = [1 3 5];
A4 = [1 2 4];

TT = zeros(6,4);

for PP = 1 : Lp
    PosK{PP} = [];

    for NN = 1 : Ln
        for MM = NN+1 : Ln
            if NN~=MM
                PosK{PP}=[PosK{PP};TwoAnchPos2([xa(NN) xa(MM)], [ya(NN) ya(MM)], [dist(NN,PP) dist(MM,PP)],[real(tag_pos_est(PP)) imag(tag_pos_est(PP))],anch_pos,dist(:,PP))];
                %       Pos{PP}=[Pos{PP};TwoAnchPos([xa(NN) xa(MM)], [ya(NN) ya(MM)], [dist(NN,PP) dist(MM,PP)],[real(K_tag_pos_est{PP}) imag(K_tag_pos_est{PP})])];
            end
        end
    end
    PosCKK{PP}=PosK{PP}(:,1)+j*PosK{PP}(:,2);
    TT = TT + abs(PosCKK{PP}-anch_pos)-dist(:,PP)';
    RR{PP} = abs(PosCKK{PP}-anch_pos)-dist(:,PP)';
end

        ABC(1) = sum(sum(abs(TT(A1,[2 3 4]))));
        ABC(2) = sum(sum(abs(TT(A2,[1 3 4]))));
        ABC(3) = sum(sum(abs(TT(A3,[1 2 4]))));
        ABC(4) = sum(sum(abs(TT(A4,[1 2 3]))));

        BBC(1) = -sum(TT(A1,1));
        BBC(2) = -sum(TT(A2,2));
        BBC(3) = -sum(TT(A3,3));
        BBC(4) = -sum(TT(A4,4));