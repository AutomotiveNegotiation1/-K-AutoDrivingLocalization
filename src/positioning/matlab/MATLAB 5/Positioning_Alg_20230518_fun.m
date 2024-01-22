clear all;

xa = [10 10 -10 -10];
ya = [10 -10 -10 10];

xt_b = [-0.5 0.5 -0.5 0.5];
yt_b = [0.5 0.5 -0.5 -0.5];

xt_b_center = sum(xt_b);
yt_b_center = sum(yt_b);
angles_from_heading = atan2(yt_b,xt_b);
rl = sqrt(xt_b.^2+yt_b.^2);

tag_center = [-1 5];
heading = 30*pi/180;

tag_pos_b = xt_b + j*yt_b;
tag_pos_g = tag_pos_b*exp(j*(heading))+tag_center(1)+j*tag_center(2);

xt = real(tag_pos_g);
yt = imag(tag_pos_g);

figure(1);
hold off;
plot(xa,ya,'bo');
hold on;
plot(tag_pos_g(2:end),'g*')
plot(tag_pos_g(1),'g^')

for n = 1 : 4
    for p = 1 : 4
        dist_o(n,p) = sqrt((xa(n)-xt(p))^2+(ya(n)-yt(p))^2);
    end
end

for kk = 1 : 1000
%     Temp = zeros(size(dist_o));
%     dist_a = awgn(Temp,40)*(dist_o)+dist_o;
    dist_a = awgn(dist_o,10)
    dist = dist_a;
%     dist(1,1:2) = dist_a(1,1:2) + 3;
%     if kk > 100
%         dist(1,1:2) = dist_a(1,1:2) + 3;
%     end

    if kk == 1
        %%%%%%%%%%%%%% Generate Matrix %%%%%%%%%%%%%%%%%%%
        [tag_pos_est, heading_est] = GetUWBPos_v1(xa, ya, dist, angles_from_heading);

        Xt_e = real(tag_pos_est);
        Yt_e = imag(tag_pos_est);
        Xt_c_e = mean(Xt_e);
        Yt_c_e = mean(Yt_e);

        %%
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        figure(100);hold off;plot(tag_pos_est);hold on;plot(tag_pos_g,'g')
        figure(2);
        hold off;
        plot(dist_o(:),'b-')
        hold on;
        plot(dist(:),'r-')
    else

        [tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e] = GetUWBPosUpdate_v1(xa, ya, Xt_c_e, Yt_c_e, heading_est, rl, dist, angles_from_heading);
        tag_pos_est = get_tag_pos(tag_center_pos_est, heading_est, tag_pos_b);
    end


    for n = 1 : 4
        for p = 1 : 4
            anchorPos = xa(n)+ya(n)*i;
            dist_est(n,p) = abs(anchorPos - tag_pos_est(p));
        end
    end
%     (dist-dist_est)
    figure(1);
    hold off;
    plot(xa,ya,'bo');
    hold on;
    plot(tag_pos_g(2:end),'g*')
    plot(tag_pos_g(1),'g^')
    % 화살표 그리기
    quiver(tag_center(1), tag_center(2), cos(heading), sin(heading), 'g', 'LineWidth', 1, 'MaxHeadSize', 0.5);
    quiver(Xt_c_e, Yt_c_e, cos(heading_est), sin(heading_est), 'b', 'LineWidth', 1, 'MaxHeadSize', 0.5);
    %quiver(Xt_c_ne, Yt_c_ne, cos(heading_est_n), sin(heading_est_n), 'r', 'LineWidth', 1, 'MaxHeadSize', 0.5);

    % plot(Xt_e(1),Yt_e(1),'ro');
    % plot(Xt_e(2),Yt_e(2),'r*');
    % plot(Xt_e(3),Yt_e(3),'rv');
    % plot(Xt_e(4),Yt_e(4),'r^');title('Original(Blue) and estimated(Red) Tag position Anchor(Greed)')

    figure(2);
    hold off;
    plot(dist_o(:),'b-')
    hold on;

    heading_est_a(kk) = heading_est;
    centerest_a(kk,:) = [Xt_c_e Yt_c_e];
    % heading_est_na(kk) = heading_est_n;
    % centerest_na(kk,:) = [Xt_c_ne Yt_c_ne];
    
    if (kk>20)
        MeanA = mean(centerest_a(kk-19:kk-10,1)+j*centerest_a(kk-19:kk-10,2));
        MeanB = mean(centerest_a(kk-9:kk,1)+j*centerest_a(kk-9:kk,2));
        centerest_a_aver(kk,:) = [real(MeanB + (MeanB-MeanA)/2) imag((MeanB + (MeanB-MeanA)/2))] ;
    else
        centerest_a_aver(kk,:) = [Xt_c_e Yt_c_e];
    end
end
plot(dist(:),'r-')
title('Original(Blue) and estimated(Red) Tag to Anchor distance')

dist_error = sqrt(mean((dist_o(:)-dist(:)).^2))
centerpos_error = sqrt(mean((centerest_a(:,1)-tag_center(1)).^2+(centerest_a(:,2)-tag_center(2)).^2))
heading_error = mean(heading-heading_est_a)*180/pi

approx_error1 = sin(heading_error*pi/180)*180/pi - heading_error
approx_error2 = cos(heading_error*pi/180)*180/pi - 1*180/pi



