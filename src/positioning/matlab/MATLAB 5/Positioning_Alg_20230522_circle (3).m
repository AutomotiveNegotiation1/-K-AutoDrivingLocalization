clear all;

Kalman_on = 0;
OverDel = 1;
InitLeng = 100;

xa = [10 10 -10 -10];
ya = [10 -10 -10 10];

xt_b = [-0.5 0.5 -0.5 0.5];
yt_b = [0.5 0.5 -0.5 -0.5];
Ln = length(xa);
Lp = length(xt_b);

xt_b_center = mean(xt_b);
yt_b_center = mean(yt_b);
angles_from_heading = atan2(yt_b,xt_b);
rl = sqrt(xt_b.^2+yt_b.^2);

global_coor = [0 0]
global_radius = 7

tag_center = [0 0];
heading_init = 90*pi/180;

theta = 0 : 0.01 : 2*pi;   % range of theta from 0 to 2pi
rho = global_radius * ones(1, length(theta));  % radius is global_radian
[x, y] = pol2cart(theta, rho);  % convert polar to Cartesian coordinates
x = x + global_coor(1);  % adjust the x-coordinates by the x-coordinate of global_coor
y = y + global_coor(2);  % adjust the y-coordinates by the y-coordinate of global_coor
heading = heading_init+theta;

x = [x(1)*ones(1,InitLeng) x];
y = [y(1)*ones(1,InitLeng) y];
heading = [heading(1)*ones(1,InitLeng) heading];
figure(100)
plot(x, y)  % plot the circle
axis equal  % make x and y axis equal
grid on  % turn on the grid
% heading = atan2(y - global_coor(2), x - global_coor(1));
%heading = mod(atan2(y - global_coor(2), x - global_coor(1)) + pi/2, 2*pi);


% circle_dist_o = [];
% di_o = [];
% di = []
% flag = 0;
average_len = 10;

for r = 1 : length(x)
    %%%%%%%%%% Create ground-truth Tag posigion & distance %%%%%%%%%%
    tag_pos_b = xt_b + j*yt_b;
    tag_pos_g = tag_pos_b*exp(j*(heading(r)))+x(r)+j*y(r);

    xt = real(tag_pos_g);
    yt = imag(tag_pos_g);

    for n = 1 : Ln
        for p = 1 : Ln
            dist_o(n,p) = sqrt((xa(n)-xt(p))^2+(ya(n)-yt(p))^2);
        end
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    %%%%%%%%%%%%%% Create simulation data by distance%%%%%%%%%%%%%%%
    dist_a = awgn(dist_o,10);
    if r > InitLeng
        dist = dist_a;
        dist(3,1:3) = dist_a(3,1:3)+3;
%         dist = dist_a;
    else
        dist = dist_a;
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    %     dist(1,1:2) = dist_a(1,1:2) + 3;
    %     if kk > 100
    %         dist(1,1:2) = dist_a(1,1:2) + 3;
    %     end


    if r == 1
        [tag_pos_est, heading_est] = GetUWBPos_v1(xa, ya, dist, angles_from_heading);

        Xt_e = real(tag_pos_est);
        Yt_e = imag(tag_pos_est);
        Xt_c_e = mean(Xt_e);
        Yt_c_e = mean(Yt_e);

        %% Check Blocked Anchor-Tag
        for PP = 1 : Lp
            Pos{PP} = [];
            for NN = 1 : Ln
                for MM = NN+1 : Ln
                    if NN~=MM
                        Pos{PP}=[Pos{PP};TwoAnchPos([xa(NN) xa(MM)], [ya(NN) ya(MM)], [dist(NN,PP) dist(MM,PP)],[Xt_c_e Yt_c_e])];
                    end
                end
            end
        end

figure(12);hold off;plot(Pos{1,1}(:,1),Pos{1,1}(:,2),'.')
figure(12);hold on;plot(Pos{1,2}(:,1),Pos{1,2}(:,2),'.')
figure(12);hold on;plot(Pos{1,3}(:,1),Pos{1,3}(:,2),'.')
figure(12);hold on;plot(Pos{1,4}(:,1),Pos{1,4}(:,2),'.')
    else
        for n = 1 : Ln
            for p = 1 : Lp
                anchorPos = xa(n)+ya(n)*i;
                RefDistSet(n,p) = abs(anchorPos - K_tag_pos_est(p));
            end
        end
        dist_n = dist;
        if OverDel
            se = abs((dist_n - RefDistSet));
            mse = mean(se(:));
            [V,ind] = max(se(:));
            if V>0.5
                dist_n(ind) = RefDistSet(ind);
            end
            
            se = abs((dist_n - RefDistSet));
            mse = mean(se(:));
            [V,ind] = max(se(:));
            if V>0.5
                dist_n(ind) = RefDistSet(ind);
            end
            se = abs((dist - RefDistSet));
            mse = mean(se(:));
            [V,ind] = max(se(:));
            if V>0.5
                dist_n(ind) = RefDistSet(ind);
            end
            
            se = abs((dist_n - RefDistSet));
            mse = mean(se(:));
            [V,ind] = max(se(:));
            if V>0.5
                dist_n(ind) = RefDistSet(ind);
            end            
        end
%         [tag_pos_est, heading_est] = GetUWBPos_v1(xa, ya, dist_n, angles_from_heading);
% 
%         Xt_e = real(tag_pos_est);
%         Yt_e = imag(tag_pos_est);
%         Xt_c_e = mean(Xt_e);
%         Yt_c_e = mean(Yt_e);

        [tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e] = GetUWBPosUpdate_v1(xa, ya, K_Xt_c_e, K_Yt_c_e, K_heading_est, rl, dist_n, angles_from_heading);
        tag_pos_est = get_tag_pos(tag_center_pos_est, heading_est, tag_pos_b);
                %% Check Blocked Anchor-Tag
        for PP = 1 : Lp
            Pos{PP} = [];
            for NN = 1 : Ln
                for MM = NN+1 : Ln
                    if NN~=MM
                        Pos{PP}=[Pos{PP};TwoAnchPos([xa(NN) xa(MM)], [ya(NN) ya(MM)], [dist(NN,PP) dist(MM,PP)],[Xt_c_e Yt_c_e])];
                    end
                end
            end
        end
   
    end

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
    if Kalman_on
    %%%%%%%%%%%%%%%%%%% Apply Kalman Filter %%%%%%%%%%%%%%%%%%%%%
%     kalman_dt = 0.1;
    kalman_dt = 0;
    kalman_A = [[1 kalman_dt 0]; [0 1 kalman_dt]; [0 0 1]];
    kalman_z = [heading_est; Xt_c_e; Yt_c_e];
    kalman_x = [headingest_a_aver(r); centerest_a_aver(r,1); centerest_a_aver(r,2)];
%     kalman_z = [heading_est; Xt_c_e; Yt_c_e];
    
    kalman_R = 1e-3*eye(3);

    kalman_x_est = PosKalman_v1(kalman_x,kalman_A,kalman_z,kalman_R);

    K_heading_est = kalman_x_est(1);
    K_Xt_c_e = kalman_x_est(2);
    K_Yt_c_e = kalman_x_est(3);


    K_tag_pos_est = tag_pos_b*exp(j*(K_heading_est))+K_Xt_c_e+j*K_Yt_c_e;

    K_centerest_a_aver(r, :) = [K_Xt_c_e K_Yt_c_e];
    K_headingest_a_aver(r) = K_heading_est;
    else
    [K_Xt_c_e] = centerest_a_aver(r,1);
    [K_Yt_c_e] = centerest_a_aver(r,2);
    K_tag_pos_est = tag_pos_b*exp(j*(headingest_a_aver(r)))+K_Xt_c_e+j*K_Yt_c_e;

    K_heading_est = headingest_a_aver(r);
    K_centerest_a_aver(r, :) = centerest_a_aver(r,:);
    K_headingest_a_aver(r) = headingest_a_aver(r);
    end    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    figure(1);
    hold off;
    plot(xa,ya,'bo');
    hold on;

    plot(tag_pos_g(1), 'go')
    plot(tag_pos_g(2), 'g*')
    plot(tag_pos_g(3), 'gv')
    plot(tag_pos_g(4), 'g^')

    plot(tag_pos_est(1), 'bo')
    plot(tag_pos_est(2), 'b*')
    plot(tag_pos_est(3), 'bv')
    plot(tag_pos_est(4), 'b^')

    plot(K_tag_pos_est(1), 'ro')
    plot(K_tag_pos_est(2), 'r*')
    plot(K_tag_pos_est(3), 'rv')
    plot(K_tag_pos_est(4), 'r^')

    % 화살표 그리기
    quiver(x(r), y(r), cos(heading(r)), sin(heading(r)), 'g', 'LineWidth', 1, 'MaxHeadSize', 0.5);

    quiver(Xt_c_e, Yt_c_e, cos(heading_est), sin(heading_est), 'b', 'LineWidth', 1, 'MaxHeadSize', 0.5);
    
    quiver(K_Xt_c_e, K_Yt_c_e, cos(K_heading_est), sin(K_heading_est), 'r', 'LineWidth', 1, 'MaxHeadSize', 0.5);


end

title('Original(Blue) and estimated(Red) Tag to Anchor distance')

figure(4)
hold off;
plot(centerest_a_aver(:,1), centerest_a_aver(:,2), 'bo')
hold on;
plot(x(:), y(:), 'g^')
plot(K_centerest_a_aver(:,1), K_centerest_a_aver(:,2), 'ro')

centerpos_error = sqrt(mean((centerest_a_aver(InitLeng+1:end,1)-x(1,InitLeng+1:end,1)').^2+(centerest_a_aver(InitLeng+1:end,2)-y(1,InitLeng+1:end,1)').^2))
heading_error = mean(abs(heading(InitLeng+1:end)-headingest_a_aver(InitLeng+1:end)))*180/pi

K_centerpos_error = sqrt(mean((K_centerest_a_aver(InitLeng+1:end,1)-x(1,InitLeng+1:end,1)').^2+(K_centerest_a_aver(InitLeng+1:end,2)-y(1,InitLeng+1:end,1)').^2))
K_heading_error = mean(abs(heading(InitLeng+1:end)-K_headingest_a_aver(InitLeng+1:end)))*180/pi





