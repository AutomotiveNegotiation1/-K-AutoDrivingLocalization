clear all;

Kalman_on = 0;
OverDel = 1;
InitLeng = 100;

xa = [10 10 -10 -10 20 -20];
ya = [10 -10 -10 10 15 -15];
% xa = [10 10 -10 -10];
% ya = [10 -10 -10 10];


xt_b = [-0.5 0.5 -0.5 0.5];
yt_b = [0.5 0.5 -0.5 -0.5];
xtc_b = xt_b + yt_b*j;

% xt_b = [-0.1 0.5 -0.5 0.5];
% yt_b = [0.1 0.5 -0.5 -0.5];

Ln = length(xa);

Lp = length(xt_b);
NumInterpPoint = 20;

xt_b_center = mean(xt_b);
yt_b_center = mean(yt_b);
angles_from_heading = atan2(yt_b,xt_b);
rl = sqrt(xt_b.^2+yt_b.^2);

global_coor = [0 0]
global_radius = 7

tag_center = [0 0];
heading_init = 90*pi/180;

theta = 0 : 0.01 : 2*pi;                        % range of theta from 0 to 2pi
rho = global_radius * ones(1, length(theta));   % radius is global_radian
[x, y] = pol2cart(theta, rho);                  % convert polar to Cartesian coordinates
x = x + global_coor(1);                         % adjust the x-coordinates by the x-coordinate of global_coor
y = y + global_coor(2);                         % adjust the y-coordinates by the y-coordinate of global_coor
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
anch_pos = xa+j*ya;
s_time = [0:length(x)-1]/10;
v_pred_x = 0;
v_pred_y = 0;
TagDistInit = zeros(Ln,Lp);
TagDistInitCount = zeros(Ln,Lp);
heading_est_a = [];
NumRxID = zeros(1,length(x));

for r = 1 : length(x)

    %%%%%%%%%% Create ground-truth Tag posigion & distance %%%%%%%%%%
    tag_pos_b = xt_b + j*yt_b;
    tag_pos_g = tag_pos_b*exp(j*(heading(r)))+x(r)+j*y(r);

    xt = real(tag_pos_g);
    yt = imag(tag_pos_g);

    for n = 1 : Ln
        for p = 1 : Lp
            dist_o(n,p) = sqrt((xa(n)-xt(p))^2+(ya(n)-yt(p))^2);
        end
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%% Create simulation data by distance %%%%%%%%%%%%%%
    dist_a = awgn(dist_o,20);

    if (r > InitLeng)
        dist = dist_a;
        %         dist(1,1:Ln) = dist_a(1,1:Ln)+3;
        %         dist = dist_a;
    else
        dist = dist_a;
        %         dist(2,1:Ln) = dist_a(2,1:Ln)+3;
    end

    Nanchor = min(randi(8)+1,4);
%     Nanchor = randi(4);
%     Nanchor = 4;
    RxID = randperm(Ln,Nanchor);
    RxTag = randi(Lp);
    RxTag = mod(r-1,Lp)+1;

    PP = RxTag;
    RxDist = dist(RxID,PP);
    NumRxID(r) = Nanchor;

    if r < 10*Lp

        for NN = 1 : Nanchor
            %             for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1
            TagDistInitCount(RxID(NN),PP) = TagDistInitCount(RxID(NN),PP) + 1;
            div = TagDistInitCount(RxID(NN),PP);
            TagDistInit(RxID(NN),PP) = TagDistInit(RxID(NN),PP)*(div-1)/div+RxDist(NN)/div;
            %             end
        end

    elseif r == 10*Lp

        for NN = 1 : Nanchor
            %             for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1
            TagDistInitCount(RxID(NN),PP) = TagDistInitCount(RxID(NN),PP) + 1;
            div = TagDistInitCount(RxID(NN),PP);
            TagDistInit(RxID(NN),PP) = TagDistInit(RxID(NN),PP)*(div-1)/div+RxDist(NN)/div;
            %             end
        end

        DistT = TagDistInit;
        DistT(TagDistInit==0) = 1000000;
        IndT = find(sum(DistT,2)<100000);
        DistT = DistT(IndT,:);
        AnchID = IndT;
        
        [tag_pos_est, heading_est] =  GetInitPos(xa(AnchID),ya(AnchID),DistT,anch_pos(AnchID),tag_pos_b,length(IndT),Lp);

        s_time_prev = 0;
        for LLp= 1 : Lp
            for lo = 1 : NumInterpPoint
                Tag_Pos_List(lo,:,LLp) = [s_time(r-NumInterpPoint+lo) tag_pos_est(LLp)];
            end
        end

    else
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%%%%%%%%%%%%  Next Position Prediction    %%%%%%%%%%%%%%%%%%
        for PPC = 1 : Lp
            [InterpPosition(PPC,1)] = InterpPos(Tag_Pos_List(:,1,PPC),real(Tag_Pos_List(:,2,PPC)),s_time(r));
            [InterpPosition(PPC,2)] = InterpPos(Tag_Pos_List(:,1,PPC),imag(Tag_Pos_List(:,2,PPC)),s_time(r));
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

        Xt_c_e = mean(InterpPosition(:,1));
        Yt_c_e = mean(InterpPosition(:,2));


        PosC = [];
        PosC_E = [];

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%%%%%%%%%%% New Position Calc.   %%%%%%%%%%%%%%%%%%%%%%%%%%%

        %         for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1
        if length(RxID)>1
            [tag_pos_est, heading_est, CandPos] = GetPos2(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,InterpPosition(:,1)+j*InterpPosition(:,2));

            [tag_pos_est, heading_est, CandPos] = GetPosRefine2(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,tag_pos_est, heading_est, CandPos);

        else
            InterpPosT = InterpPosition(:,1)+j*InterpPosition(:,2);
            TempC = mean(InterpPosT);
            Est_H_p = 0;
            for P = 1 : length(InterpPosT)
                Est_H_p = Est_H_p+((InterpPosT(P))-Est_C)/tag_pos_b(P);
            end
            Est_H = angle(Est_H_p);

            for P = 1 : size(Pos2C,1)

                tag_pos_est(P) = TempC + tag_pos_b(P)*exp(j*Est_H);

            end

            CandPos = InterpPosition(PP,1)+j*InterpPosition(PP,2);
        end
        Tag_Pos_List(1:NumInterpPoint-1,:,PP) = Tag_Pos_List(2:NumInterpPoint,:,PP);
        Tag_Pos_List(NumInterpPoint,:,PP) = [s_time(r) CandPos];
        %             Tag_Pos_List(1:2,:,PP) = Tag_Pos_List(2:3,:,PP);
        %             Tag_Pos_List(3,:,PP) = [s_time(r) tag_pos_est(PP)];
        Xt_c_e = real(mean(tag_pos_est));
        Yt_c_e = imag(mean(tag_pos_est));
        %         end

        tag_center_pos_est = mean(tag_pos_est);

        if ~isempty(heading_est_a)
            if (heading_est - heading_est_a(r-1)) > pi
                heading_est_a(r) = heading_est - 2*pi;
            elseif (heading_est_a(r-1) - heading_est) > pi
                heading_est_a(r) = heading_est + 2*pi;
            else
                heading_est_a(r) = heading_est;
            end
        else
            heading_est_a(r) = heading_est;
        end

        centerest_a(r,:) = [real(tag_center_pos_est) imag(tag_center_pos_est)];

        if (r>(average_len*2))
            MeanA = mean(centerest_a(r-19:r-10,1)+j*centerest_a(r-19:r-10,2));
            MeanB = mean(centerest_a(r-9:r,1)+j*centerest_a(r-9:r,2));
            centerest_a_aver(r,:) = [real(MeanB + (MeanB-MeanA)/2) imag((MeanB + (MeanB-MeanA)/2))] ;
            MeanA_head = mean(heading_est_a(r-average_len*2+1:r-average_len));
            MeanB_head = mean(heading_est_a(r-average_len+1:r));
            headingest_a_aver(r) = mod([(MeanB_head + (MeanB_head-MeanA_head)/2)],2*pi) ;
        else
            centerest_a_aver(r,:) = [Xt_c_e Yt_c_e];
            headingest_a_aver(r) = heading_est_a(r);
        end

        tag_pos_est_aver = get_tag_pos(centerest_a_aver(r,1)+j*centerest_a_aver(r,2), headingest_a_aver(r), tag_pos_b);
        [K_Xt_c_e] = centerest_a_aver(r,1);
        [K_Yt_c_e] = centerest_a_aver(r,2);
        tag_pos_est = tag_pos_b*exp(j*(headingest_a_aver(r)))+K_Xt_c_e+j*K_Yt_c_e;

        K_heading_est = mod(headingest_a_aver(r),2*pi);
        K_centerest_a_aver(r, :) = centerest_a_aver(r,:);
        K_headingest_a_aver(r) = mod(headingest_a_aver(r),2*pi);


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

        quiver(x(r), y(r), cos(heading(r)), sin(heading(r)), 'g', 'LineWidth', 1, 'MaxHeadSize', 0.5);

        quiver(real(tag_center_pos_est), imag(tag_center_pos_est), cos(headingest_a_aver(r)), sin(headingest_a_aver(r)), 'b', 'LineWidth', 1, 'MaxHeadSize', 0.5);

        if abs((x(r)+j*y(r))-(Xt_c_e+j*Yt_c_e))>1
            gg = 1;
        end
    end
end

title('Original(Blue) and estimated(Red) Tag to Anchor distance')

figure(4)
hold off;
plot(x(:), y(:), 'g^')
hold on;

%plot(K_centerest_a_aver(:,1), K_centerest_a_aver(:,2), 'ro')
plot(centerest_a(InitLeng+1:r,1), centerest_a(InitLeng+1:r,2), 'ko')
plot(centerest_a_aver(InitLeng+1:r,1), centerest_a_aver(InitLeng+1:r,2), 'bo')


centerpos_error = sqrt(mean((centerest_a(InitLeng+1:end,1)-x(1,InitLeng+1:end,1)').^2+(centerest_a(InitLeng+1:end,2)-y(1,InitLeng+1:end,1)').^2))
heading_error = mean(abs(heading(InitLeng+1:end)-heading_est_a(InitLeng+1:end)))*180/pi

K_centerpos_error = sqrt(mean((K_centerest_a_aver(InitLeng+1:end,1)-x(1,InitLeng+1:end,1)').^2+(K_centerest_a_aver(InitLeng+1:end,2)-y(1,InitLeng+1:end,1)').^2))
K_heading_error = mean(abs(mod(heading(InitLeng+1:end),2*pi)-K_headingest_a_aver(InitLeng+1:end)))*1