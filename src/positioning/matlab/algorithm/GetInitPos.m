function [tag_pos_est, heading_est] = GetInitPos(xa,ya,dist,anch_pos,tag_pos_b,Ln,Lp)



Sel_F = 10000000;
Porb2 = 0;
Pos2C = zeros(Lp,2)+0j;
Est_F = zeros(2^Lp,1);
Est_C = zeros(2^Lp,1)+0j;
Est_H = zeros(2^Lp,1);
Cand_F = 0;
Cand_C = 0;
Cand_H = 0;
Sel_C = 0;
Sel_H = 0;

for L1 = 1 : Ln
    for L2 = L1 + 1 : Ln

        for Pn = 1 : Lp
            [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)], [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn));
            Pos2C(Pn,1) = Pos2(1,1)+j*Pos2(1,2);
            Pos2C(Pn,2) = Pos2(2,1)+j*Pos2(2,2);
        end
        for Qn = 0 : 2^Lp-1
            Va = dec2bin(Qn,Lp);
            [Est_F(Qn+1),Est_C(Qn+1),Est_H(Qn+1)] = GetMultiTagPosGen(Pos2C,Va,tag_pos_b,[1 1 1 1]);
        end
        [val,ind] = min(Est_F);
        Cand_F = val;
        Cand_C = Est_C(ind);
        Cand_H = Est_H(ind);
        if Sel_F > Cand_F
            Sel_F = Cand_F;
            Sel_C = Cand_C;
            Sel_H = Cand_H;
        end
    end
end


heading_est = Sel_H;
tag_pos_est = Sel_C+tag_pos_b*exp(j*Sel_H);


