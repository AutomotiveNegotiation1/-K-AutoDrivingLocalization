function [tag_pos_est, heading_est, cand_tag_pos] = GetPosRefine(xa,ya,dist,anch_pos,tag_pos_b,Ln,PP,tag_pos_est, heading_est, cand_tag_pos)
if Ln == 1

else

    for kk = 1 : Ln
        calc_dist_err(kk) = abs(abs(anch_pos(kk)-tag_pos_est(PP))-dist(kk))^2;
    end

    mean_err = sqrt(mean(calc_dist_err));

    [val,ind] = find(calc_dist_err<max(mean_err*0.3,0.2));

    Len = length(ind);
    NB = length(tag_pos_b);

    if Len >= 3
        [Pos,Prob]=ThreeAnchPos3(xa(ind), ya(ind), dist(ind));
        tag_pos_est(PP) = Pos(1)+j*Pos(2);
        cand_tag_pos = Pos(1)+j*Pos(2);

        Est_C = 0;
        for P = 1 : length(tag_pos_est)
            Est_C = Est_C + (tag_pos_est(P))/NB;
        end

        Est_H_p = 0;
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


