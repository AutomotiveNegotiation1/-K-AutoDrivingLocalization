clear all;

xa = [0 0 1.67 1.67];
ya = [4.22 0 4.22 0];
dist = transpose([[1.06 3.14 1.89 3.51]; [0.99 3.19 1.75 3.43]; [1.33 2.89 1.99 3.32]; [1.48 3.04 2.06 3.31]]);
anch_pos = xa+j*ya;


% Xa = xa + j*ya;
% tag_center = [0.232, 3.07]
% tag_c = tag_center(1)+tag_center(2)*j;
% abs(Xa - tag_c)


xt_b = [-0.09 0.09 0.09 -0.09];
yt_b = [0.12 0.12 -0.12 -0.12];
tag_pos_b = xt_b + j*yt_b;

xt_b_center = mean(xt_b);
yt_b_center = mean(yt_b);
angles_from_heading = atan2(yt_b,xt_b);

rl = sqrt(xt_b.^2+yt_b.^2);

average_len = 10;


for r = 1 : 600
%     if r == 1
        [tag_pos_est, heading_est] = GetUWBPos_v1(xa, ya, (dist), angles_from_heading);
%     else
%         tag_pos_est = K_tag_pos_est;
%         heading_est = K_heading_est;
%     end
    
   
    % % 
    % % tag_pos_est = get_tag_pos(tag_center_pos_est, heading_est, tag_pos_b);
    figure(1)
    hold off;
    plot(xa, ya, 'go');
    hold on;
    plot(tag_pos_est(1), 'bo');
    plot(tag_pos_est(2), 'b*');
    plot(tag_pos_est(3), 'bv');
    plot(tag_pos_est(4), 'b^');
    axis equal

    xt_est = real(tag_pos_est);
    yt_est = imag(tag_pos_est);

    Xt_c_e = mean(xt_est);
    Yt_c_e = mean(yt_est);

    [tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e] = GetUWBPosUpdate_v1(xa, ya, Xt_c_e, Yt_c_e, heading_est, rl, dist, angles_from_heading);
    tag_pos_est = get_tag_pos(tag_center_pos_est, heading_est, tag_pos_b);

    figure(2);
    hold off;
    plot(xa, ya, 'go');
    hold on;
    plot(tag_pos_est(1), 'bo');
    plot(tag_pos_est(2), 'b*');
    plot(tag_pos_est(3), 'bv');
    plot(tag_pos_est(4), 'b^');
    axis equal

    heading_est_a(r) = heading_est;
    centerest_a(r,:) = [Xt_c_e Yt_c_e];

    if (r>(average_len*2))
        MeanA = mean(centerest_a(r-19:r-10,1)+j*centerest_a(r-19:r-10,2));
        MeanB = mean(centerest_a(r-9:r,1)+j*centerest_a(r-9:r,2));
        centerest_a_aver(r,:) = [real(MeanB + (MeanB-MeanA)/2) imag((MeanB + (MeanB-MeanA)/2))] ;
        MeanA_head = mean(heading_est_a(r-average_len*2+1:r-average_len));
        MeanB_head = mean(heading_est_a(r-average_len+1:r));
        headingest_a_aver(r) = [(MeanB_head + (MeanB_head-MeanA_head)/2)] ;
    else
        centerest_a_aver(r,:) = [Xt_c_e Yt_c_e];
        headingest_a_aver(r) = heading_est;
    end

    tag_pos_est_aver = get_tag_pos(centerest_a_aver(r,1)+j*centerest_a_aver(r,2), headingest_a_aver(r), tag_pos_b);
    [K_Xt_c_e] = centerest_a_aver(r,1);
    [K_Yt_c_e] = centerest_a_aver(r,2);
    K_tag_pos_est = tag_pos_b*exp(j*(headingest_a_aver(r)))+K_Xt_c_e+j*K_Yt_c_e;

    K_heading_est = headingest_a_aver(r);
    K_centerest_a_aver(r, :) = centerest_a_aver(r,:);
    K_headingest_a_aver(r) = headingest_a_aver(r);

end