function [tag_pos_est, heading_est, ErrSum] = GetInitPos4(xa,ya,dist,anch_pos,tag_pos_b,Ln,Lp,PrevPos, PrevHeading)

persistent PrevUWBpos PrevUWBHeading

if isempty(PrevUWBpos)
    PrevUWBpos = zeros(1,4)+0*j;
    PrevUWBHeading = 0;
end

Sel_F = 10000000;
Porb2 = 0;
Pos2C = zeros(Lp,2*(Ln*(Ln-1)/2))+0j;
Pos2CL= zeros(1,Lp);
Est_F = zeros(2^Lp,1);
Est_C = zeros(2^Lp,1)+0j;
Est_H = zeros(2^Lp,1);
Cand_F = 0;
Cand_C = 0;
Cand_H = 0+0*j;
Sel_C = 0;
Sel_H = 0;
TempH = zeros(1,16)+0*j;
TempAbsH = 1000*ones(1,16);
Temp = zeros(1,3)+0*j;
lP = zeros(1,4)+0*j;
%

disth = dist;
disth(dist==0)=100;

[Xc, Yc, heading, flag] = GetUWBPos_v4(xa, ya, dist, Ln, Lp, tag_pos_b);
tag_pos_c = (Xc+j*Yc) + tag_pos_b*exp(j*(heading));

for Pn = 1 : Lp
    l = 1;
    if sum(dist(:,Pn))~=0
        [dist1, L1i] = sort(disth(:,Pn));
        L1 = L1i(1);
        L2 = L1i(2);
        
        [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)], [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn));

        if length(dist1(dist1<30)) > 2
            
            if (dist1(4)<30) %&& (dist1(4)<25)
                [Pos3,Prob3]=ThreeAnchPos3([xa(L1i(1:4))], [ya(L1i(1:4))], [dist(L1i(1:4),Pn)]);
            else
                [Pos3,Prob3]=ThreeAnchPos3([xa(L1i(1:3))], [ya(L1i(1:3))], [dist(L1i(1:3),Pn)]);
            end
            
            Temp(1) = Pos3(1)+j*Pos3(2);
            Temp(2) = Pos2(1,1)+j*Pos2(1,2);
            Temp(3) = Pos2(2,1)+j*Pos2(2,2);
            if flag == 1
                DistPrev = abs(Temp - tag_pos_c(Pn));
%                 Temp(1) = tag_pos_c(Pn);
%                 Temp(2) = tag_pos_c(Pn);
%                 Temp(2) = tag_pos_c(Pn);
                
            else
                DistPrev = abs(Temp - PrevPos(Pn));
            end
            [vale, inde] = min(DistPrev);
            DistPrevUWB = abs(Temp - PrevUWBpos(Pn));
            [valeU, indeU] = min(DistPrevUWB);
            if inde ~= indeU

                if (vale < 1) && (valeU < 1)
                    Pos2C(Pn,l) = Temp(inde);
                    Pos2C(Pn,l+1) = Temp(indeU);
                    l = 2;
                elseif (vale < 1) && (valeU >= 1)
                    Pos2C(Pn,l) = Temp(inde);
                    l = 1;
                elseif (vale >= 1) && (valeU < 1)
                    Pos2C(Pn,l) = Temp(indeU);
                    l = 1;
                else
                    Pos2C(Pn,l) = Temp(1);
                    Pos2C(Pn,l+1) = Temp(2);
                    Pos2C(Pn,l+2) = Temp(3);
                    
                    l = 3;
                end
                
            else
                Pos2C(Pn,l) = Temp(inde);
%                 Pos2C(Pn,l+1) = Temp(2);
%                 Pos2C(Pn,l+2) = Temp(3);
                    
                l = 1;
            end
            
        else
            Temp(1) = Pos2(1,1)+j*Pos2(1,2);
            Temp(2) = Pos2(2,1)+j*Pos2(2,2);
            DistPrev = abs(Temp - PrevPos(Pn));
            [vale, inde] = min(DistPrev);
            DistPrevUWB = abs(Temp - PrevUWBpos(Pn));
            [valeU, indeU] = min(DistPrevUWB);
            if inde ~= indeU
                if (vale < 1) && (valeU < 1)
                    Pos2C(Pn,l) = Temp(inde);
                    Pos2C(Pn,l+1) = Temp(indeU);
                    l = 2;
                elseif (vale < 1) && (valeU >= 1)
                    Pos2C(Pn,l) = Temp(inde);
                    l = 1;
                elseif (vale >= 1) && (valeU < 1)
                    Pos2C(Pn,l) = Temp(indeU);
                    l = 1;
                else
                    Pos2C(Pn,l) = Temp(1);
                    Pos2C(Pn,l+1) = Temp(2);
                    
                    
                    l = 2;
                end
            else
                Pos2C(Pn,l) = Temp(1);
                l = 1;
            end
        end

        Pos2CL(Pn) = l;

    end

end

% for Pn = 1 : Lp
%     l = 1;
%     if sum(dist(:,Pn))~=0
%         [dist1, L1i] = sort(disth(:,Pn));
%         L1 = L1i(1);
%         L2 = L1i(2);
%         
%         [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)], [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn));
% 
%         if length(dist(dist1~=0)) > 2
%             if (dist1(3)~=0) %&& (dist1(3)<25)
%                 L3 = L1i(3);
%                 if (dist1(4)~=0) %&& (dist1(4)<25)
%                     L4 = L1i(4);
%                     [Pos3,Prob3]=ThreeAnchPos3([xa(L1i(1:4))], [ya(L1i(1:4))], [dist(L1i(1:4),Pn)]);
%                 else
%                     [Pos3,Prob3]=ThreeAnchPos3([xa(L1i(1:3))], [ya(L1i(1:3))], [dist(L1i(1:3),Pn)]);
%                 end
%                 Pos2_C = Pos2(:,1) + j*Pos2(:,2);
%                 Pos3_C = Pos3(1) + j*Pos3(2);
% 
%                 d_1 = abs(Pos2_C(1)-Pos3_C);
%                 d_2 = abs(Pos2_C(2)-Pos3_C);
% 
%                 if (abs(d_1-d_2) > 1) 
% 
%                     if d_1<d_2
%                         Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2);
%                     else
%                         Pos2C(Pn,l) = Pos2(2,1)+j*Pos2(2,2);
%                     end
%                     
% 
%                 else
%                     Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2);
%                     Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2);
%                     l = 2;
%                 end
%             else
%                 Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2);
%                 Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2);
%                 l = 2;
%             end
%         else
%             Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2);
%             Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2);
%             l = 2;
%         end
% 
%         Pos2CL(Pn) = l;
% 
%     end
% 
% end

NZeroId = find(Pos2CL~=0);
Sel_H = 0;
Sel_HErr = 1e6;
Sel_Err = 1e6;
Sel_ErrH = 1e6;

Sel_PosDiff = 1e6;
Sel_HeadingDiff = 1e6;
Sel_DistErr = 1e6;
Sel_DistAcc = 1e6;
ErrSum = 1e6;

%Ld = min(Pos2Prob(:))*5;

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
                    Cand_PosDiff = abs(mean(lP)-mean(PrevPos))/10;
                else
                    Cand_PosDiff = 0;
                end
                
                SelectedPos = zeros(16,2);
                for oer = 1 : length(NZeroId)
                    PPI = NZeroId(oer);
                    for ork = oer + 1 : length(NZeroId)
                        cnt = cnt + 1;
                        PPQ = NZeroId(ork);
                        TempH(cnt) = (lP(PPI)-lP(PPQ))/(tag_pos_b(PPI)-tag_pos_b(PPQ));
                        Cand_H = Cand_H+(lP(PPI)-lP(PPQ))/(tag_pos_b(PPI)-tag_pos_b(PPQ));
                        TempAbsH(cnt) = abs(abs((lP(PPI)-lP(PPQ))/(tag_pos_b(PPI)-tag_pos_b(PPQ)))-1)^2;
                        SelectedPos(cnt,:) = [PPI PPQ];
                    end

                end

                [Val,Inds] = sort(TempAbsH);
                Cand_Err = mean(TempAbsH(Inds(1:3)));
                Cand_ErrH = abs(abs(mean(TempH(Inds(1:3))))-1);
                Cand_HErr = mean(abs(imag(TempH(Inds(1:3))*conj(mean(TempH(Inds(1:3)))))).^2);


                if mean(PrevPos) ~=0
                    Cand_HeadingDiffT = mod(angle(mean(TempH(Inds(1:3)))),2*pi)-mod(PrevHeading,2*pi);
                    Cand_HeadingDiff = min(abs(Cand_HeadingDiffT),abs(abs(Cand_HeadingDiffT)-2*pi))/30;
                else
                    Cand_HeadingDiff = 0;
                end

                %if ((Cand_HErr + Cand_Err + Cand_ErrH + Cand_HeadingDiff) < (Sel_HErr + Sel_Err + Sel_ErrH+ Sel_HeadingDiff )) && (cnt~=0) % && (Cand_PosDiff < 10)
                %if ((Cand_HErr + Cand_Err + Cand_ErrH) < (Sel_HErr + Sel_Err + Sel_ErrH  )) && (cnt~=0) % && (Cand_PosDiff < 10)
                if ((Cand_HErr + Cand_Err + Cand_ErrH + Cand_PosDiff + Cand_HeadingDiff ) < (Sel_HErr + Sel_Err + Sel_ErrH + Sel_PosDiff + Sel_HeadingDiff )) && (cnt~=0) % && (Cand_PosDiff < 10)
                    %if ((Cand_HErr + Cand_Err + Cand_ErrH + Cand_PosDiff + Cand_HeadingDiff + DistErr + DistAcc) < (Sel_HErr + Sel_Err + Sel_ErrH + Sel_PosDiff + Sel_HeadingDiff + Sel_DistErr  + Sel_DistAcc)) && (cnt~=0) % && (Cand_PosDiff < 10)
                    Sel_H = mean(TempH(Inds(1:3)));
                    Sel_HErr = Cand_HErr;
                    Sel_Err = Cand_Err;
                    Sel_ErrH = Cand_ErrH;
                    %ErrSum = Cand_HErr + Cand_Err + Cand_ErrH ;
                    ErrSum = Cand_HErr + Cand_Err + Cand_ErrH + Cand_PosDiff + Cand_HeadingDiff;
                    Sel_PosDiff = Cand_PosDiff;
                    Sel_HeadingDiff = Cand_HeadingDiff;
                    %                                     Sel_DistErr = DistErr;
                    %                                     Sel_DistAcc = DistAcc;

                    Sel_C = 0+0*j;
                    for dfg = 1 : 3
                        Sel_C = Sel_C + lP(SelectedPos(Inds(dfg),1))/6 + lP(SelectedPos(Inds(dfg),2))/6;
                    end
                    %                     for dfg = 1 : length(NZeroId)
                    %                         Sel_C = Sel_C + (lP(NZeroId(dfg))-exp(j*angle(Sel_H))*tag_pos_b(NZeroId(dfg)))/length(NZeroId);
                    %                     end
                end
            end

            %                         end
            %                     end
            %                 end
            %             end
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

PrevUWBpos = tag_pos_est;
PrevUWBHeading = heading_est;


