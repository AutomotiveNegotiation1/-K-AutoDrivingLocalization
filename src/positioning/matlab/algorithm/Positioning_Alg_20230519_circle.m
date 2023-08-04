clear all;

xa = [10 10 -10 -10];
ya = [10 -10 -10 10];

xt_b = [-0.5 0.5 -0.5 0.5];
yt_b = [0.5 0.5 -0.5 -0.5];

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

figure(100)
plot(x, y)  % plot the circle
axis equal  % make x and y axis equal
grid on  % turn on the grid
% heading = atan2(y - global_coor(2), x - global_coor(1));
%heading = mod(atan2(y - global_coor(2), x - global_coor(1)) + pi/2, 2*pi);


circle_dist_o = [];
di_o = [];
di = []
flag = 0;
average_len = 10;

for r = 1 : length(x)
    tag_pos_b = xt_b + j*yt_b;
    tag_pos_g = tag_pos_b*exp(j*(heading(r)))+x(r)+j*y(r);

    xt = real(tag_pos_g);
    yt = imag(tag_pos_g);

    for n = 1 : 4
        for p = 1 : 4
            dist_o(n,p) = sqrt((xa(n)-xt(p))^2+(ya(n)-yt(p))^2);
        end
    end

    dist_a = awgn(dist_o,10)
    dist = dist_a;
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
    else
        [tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e] = GetUWBPosUpdate_v1(xa, ya, Xt_c_e, Yt_c_e, heading_est, rl, dist, angles_from_heading);
        tag_pos_est = get_tag_pos(tag_center_pos_est, heading_est, tag_pos_b);
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

    figure(1);
    hold off;
    plot(xa,ya,'bo');
    hold on;

    plot(tag_pos_g(1), 'go')
    plot(tag_pos_g(2), 'g*')
    plot(tag_pos_g(3), 'gv')
    plot(tag_pos_g(4), 'g^')
    % 화살표 그리기
    quiver(x(r), y(r), cos(heading(r)), sin(heading(r)), 'g', 'LineWidth', 1, 'MaxHeadSize', 0.5);

    quiver(centerest_a_aver(r,1), centerest_a_aver(r,2), cos(headingest_a_aver(r)), sin(headingest_a_aver(r)), 'b', 'LineWidth', 1, 'MaxHeadSize', 0.5);

end

title('Original(Blue) and estimated(Red) Tag to Anchor distance')

figure(4)
hold off;
plot(centerest_a_aver(:,1), centerest_a_aver(:,2), 'bo')
hold on;
plot(x(:), y(:), 'g^')

centerpos_error = sqrt(mean((centerest_a_aver(:,1)-x').^2+(centerest_a_aver(:,2)-y').^2))
heading_error = mean(heading-headingest_a_aver)*180/pi





