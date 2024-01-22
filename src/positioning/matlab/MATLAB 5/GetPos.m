function [tag_pos_est, heading_est, cand_tag_pos] = GetPos(xa,ya,dist,anch_pos,tag_pos_b,Ln,PP,PredPos)

Pos2C = zeros(length(PredPos),2);
Pos2C(:,1) = PredPos;
Va_ind = zeros(1,length(PredPos));
Sel_F = 10000000;
for L1 = 1 : Ln
    for L2 = L1 + 1 : Ln

        for Pn = PP : PP
            [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)], [dist(L1) dist(L2)],[0 0],[0 0],anch_pos(1:Ln),dist);
            Pos2C(Pn,1) = Pos2(1,1)+j*Pos2(1,2);
            Pos2C(Pn,2) = Pos2(2,1)+j*Pos2(2,2);
        end
        for Qn = 0 : 1
            Va = reverse(dec2bin(2^(PP*Qn)-1,length(PredPos)));
            Va_ind (PP) = 1;
            [Est_F(Qn+1),Est_C(Qn+1),Est_H(Qn+1)] = GetMultiTagPosGen(Pos2C,Va,tag_pos_b,Va_ind);
        end
        [val,ind] = min(Est_F);
        Cand_F = val;
        Cand_C = Est_C(ind);
        Cand_H = Est_H(ind);
        if Sel_F > Cand_F
            Sel_F = Cand_F;
            Sel_C = Cand_C;
            Sel_H = Cand_H;
            cand_tag_pos = Pos2C(PP,ind);
        end
    end
end


heading_est = Sel_H;
tag_pos_est = Sel_C+tag_pos_b*exp(j*Sel_H);