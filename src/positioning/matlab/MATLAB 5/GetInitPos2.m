function [tag_pos_est, heading_est] = GetInitPos2(xa,ya,dist,anch_pos,tag_pos_b,Ln,Lp,PrevPos, PrevHeading)



Sel_F = 10000000;
Porb2 = 0;
Pos2C = zeros(Lp,2*(Ln*(Ln-1)/2))+0j;
Pos2CL= zeros(1,Lp);
Est_F = zeros(2^Lp,1);
Est_C = zeros(2^Lp,1)+0j;
Est_H = zeros(2^Lp,1);
Cand_F = 0;
Cand_C = 0;
Cand_H = 0;
Sel_C = 0;
Sel_H = 0;
%

for Pn = 1 : Lp
    l = 1;
    if sum(dist(:,Pn))~=0
        for L1 = 1 : Ln
            for L2 = L1 + 1 : Ln
                if (dist(L1,Pn)~=0) && (dist(L2,Pn)~=0)
                    [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)], [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn));
                    Pos2C(Pn,(l-1)*2+1) = Pos2(1,1)+j*Pos2(1,2);
                    Pos2C(Pn,(l-1)*2+2) = Pos2(2,1)+j*Pos2(2,2);
                    Pos2CL(Pn) = l*2;
                    l = l + 1;
                end
            end
        end
    end

end

NZeroId = find(Pos2CL~=0);
Sel_H = 0;
Sel_HErr = 1e6;
Sel_Err = 1e6;
Sel_ErrH = 1e6;


for l1 = 1 : max(1,Pos2CL(1))
    if (Pos2CL(1)~=0)
        lP(1) = Pos2C(1,l1);
    else
        lP(1) = 0;
    end
    for l2 = 1 : max(1,Pos2CL(2))
        if (Pos2CL(2)~=0)
            lP(2) = Pos2C(2,l2);
        else
            lP(2) = 0;
        end
        for l3 = 1 : max(1,Pos2CL(3))
            if (Pos2CL(3)~=0)
                lP(3) = Pos2C(3,l3);
            else
                lP(3) = 0;
            end
            for l4 = 1 : max(1,Pos2CL(4))
                if (Pos2CL(4)~=0)
                    lP(4) = Pos2C(4,l4);
                else
                    lP(4) = 0;
                end

                Cand_H = 0;
                cnt = 0;
                Cand_absH = 0;
                if mean(PrevPos) ~=0
                    Cand_PosDiff = abs(mean(lP)-mean(PrevPos))^2;
                else
                    Cand_PosDiff = 0;
                end

                for oer = 1 : length(NZeroId)
                    PPI = NZeroId(oer);
                    for ork = oer + 1 : length(NZeroId)
                        cnt = cnt + 1;
                        PPQ = NZeroId(ork);
                        TempH(cnt) = (lP(PPI)-lP(PPQ))/(tag_pos_b(PPI)-tag_pos_b(PPQ));
                        Cand_H = Cand_H+(lP(PPI)-lP(PPQ))/(tag_pos_b(PPI)-tag_pos_b(PPQ));
                        TempAbsH(cnt) = abs(abs((lP(PPI)-lP(PPQ))/(tag_pos_b(PPI)-tag_pos_b(PPQ)))-1)^2;
                        
                    end
                end

                Cand_Err = mean(TempAbsH);
                Cand_ErrH = (abs(mean(TempH))-1)^2;
                Cand_HErr = mean(abs(imag(TempH*conj(mean(TempH)))).^2);

                if ((Cand_HErr + Cand_Err + Cand_ErrH) < (Sel_HErr + Sel_Err + Sel_ErrH)) && (cnt~=0) % && (Cand_PosDiff < 10)
                    Sel_H = Cand_H/cnt;
                    Sel_HErr = Cand_HErr;
                    Sel_Err = Cand_Err;
                    Sel_ErrH = Cand_ErrH;

                    Sel_C = 0;
                    for dfg = 1 : length(NZeroId)
                        Sel_C = Sel_C + (lP(NZeroId(dfg))-exp(j*angle(Sel_H))*tag_pos_b(NZeroId(dfg)))/length(NZeroId);
                    end
                end

            end
        end
    end
end



% 
% for L1 = 1 : Ln
%     for L2 = L1 + 1 : Ln
%         for Pn = 1 : Lp
%             if (dist(L1,Pn)~=0) && (dist(L2,Pn)~=0)
%                 [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)], [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn));
%                 Pos2C(Pn,1) = Pos2(1,1)+j*Pos2(1,2);
%                 Pos2C(Pn,2) = Pos2(2,1)+j*Pos2(2,2);
%             else
%                 Pos2C(Pn,1) = 0;
%                 Pos2C(Pn,2) = 0;
%             end
%         end
%         for Qn = 0 : 2^Lp-1
%             Va = dec2bin(Qn,Lp);
%             [Est_F(Qn+1),Est_C(Qn+1),Est_H(Qn+1)] = GetMultiTagPosGen(Pos2C,Va,tag_pos_b,[1 1 1 1]);
%         end
%         [val,ind] = min(Est_F);
%         Cand_F = val;
%         Cand_C = Est_C(ind);
%         Cand_H = Est_H(ind);
%         if Sel_F > Cand_F
%             Sel_F = Cand_F;
%             Sel_C = Cand_C;
%             Sel_H = Cand_H;
%         end
%     end
% end


heading_est = angle(Sel_H);
tag_pos_est = Sel_C+tag_pos_b*exp(j*heading_est);


