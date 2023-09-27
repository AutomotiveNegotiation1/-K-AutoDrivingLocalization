function [tag_pos_est, heading_est, cand_tag_pos] = GetPosRefine2(xa,ya,dist,RxID,tag_pos_b,Ln,PP,tag_pos_est, heading_est, cand_tag_pos)
anch_pos = xa(RxID)+j*ya(RxID);

calc_dist_err = zeros(1,Ln);
Est_P = zeros(1,length(tag_pos_b))+0j;

if Ln == 1

else

    for kk = 1 : Ln
        calc_dist_err(kk) = abs(abs(anch_pos(kk)-tag_pos_est(PP))-dist(kk))^2;
    end

    mean_err = sqrt(mean(calc_dist_err));

    [val,ind] = find(calc_dist_err<max(mean_err*0.3,0.2));

    Len = length(ind);

    NB = length(tag_pos_b);

    Tx = real(anch_pos(ind));
    Ty = imag(anch_pos(ind));

    if (Len >= 3) && ((Ty(2)-Ty(1))*(Tx(3)-Tx(1)) ~= (Ty(3)-Ty(1))*(Tx(2)-Tx(1)))
        [Pos,Prob]=ThreeAnchPos3(real(anch_pos(ind)), imag(anch_pos(ind)), dist(ind));
        tag_pos_est(PP) = Pos(1)+j*Pos(2);
        cand_tag_pos = Pos(1)+j*Pos(2);

        Est_C = 0+0j;
        for P = 1 : length(tag_pos_est)
            Est_C = Est_C + (tag_pos_est(P))/NB;
        end

        Est_H_p = 0+0j;
        for P = 1 : length(tag_pos_est)
            Est_H_p = (((tag_pos_est(P)))-Est_C)/tag_pos_b(P);
        end
        Est_H = angle(Est_H_p);

        Est_F = 0;
        for P = 1 : length(tag_pos_est)
            Est_P(P) = Est_C + tag_pos_b(P)*exp(j*Est_H);
            Est_F = Est_F + abs(Est_P(P)-(tag_pos_est(P)))^2;
        end

        tag_pos_est = Est_P;
        heading_est = Est_H;
    else

    end


end


