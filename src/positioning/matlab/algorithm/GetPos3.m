function [tag_pos_est, heading_est, cand_tag_pos] = GetPos3(xa,ya,dist,RxID,tag_pos_b,Ln,PP,PredPos,DistPrev,RxIDprev, RxIDprevLen, PPprev)

Pos2C = zeros(length(PredPos),2)+0j;
Pos2C(:,1) = PredPos;
Va_ind = zeros(1,length(PredPos));
Sel_F = 10000000;
Prob2 = 0;
Est_F = zeros(1,2);
Est_C = zeros(1,2)+0j;
Est_H = zeros(1,2);

Cand_F = 0;
Cand_C = 0+0j;
Cand_H = 0;
Sel_C = 0+0j;
Sel_H = 0;
cand_tag_pos = 0+0j;


for L1 = 1 : Ln
    for L2 = L1 + 1 : Ln

        for Pn = PP : PP
            [Pos2,Prob2]=TwoAnchPos3([xa(RxID(L1)) xa(RxID(L2))], [ya(RxID(L1)) ya(RxID(L2))], [dist(L1) dist(L2)],[0 0],[0 0],xa(RxID)+j*ya(RxID),dist);
            Pos2C(Pn,1) = Pos2(1,1)+j*Pos2(1,2);
            Pos2C(Pn,2) = Pos2(2,1)+j*Pos2(2,2);
        end
        for Qn = 0 : 1
            Va = reverse(dec2bin(2^(PP*Qn)-1,length(PredPos)));
            Va_ind (PP) = 1;
            [Est_F(Qn+1),Est_C(Qn+1),Est_H(Qn+1)] = GetMultiTagPosGen2(Pos2C,Va,tag_pos_b,Va_ind,DistPrev,RxIDprev, RxIDprevLen,xa,ya,PPprev);
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