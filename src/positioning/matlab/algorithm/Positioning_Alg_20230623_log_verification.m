clear all;

data = load('dist_log_20230622.mat');
arr = permute(data.dist_log, [2 3 1]); % 차원 순서 변경


Kalman_on = 0;
OverDel = 1;
InitLeng = 100;

% xa = [10 10 -10];
% ya = [10 -10 -10];
% xa = [10 10 -10 -10];
% ya = [10 -10 -10 10];
xa = [0.   1.67 0 1.67]
ya = [4.22 4.22 0 0.  ]
% xa = [0.   0.   1.67 1.67]
% ya = [4.22 0.   4.22 0.  ]

Xa = xa + j*ya;
tag_center = [0.232, 3.07]
tag_c = tag_center(1)+tag_center(2)*j;
abs(Xa - tag_c)


% xa = [10 10 -10 -10 20 -10 20];
% ya = [10 -10 -10 10 10 20 -10];

% xt_b = [-0.5 0.5 -0.5 0.5];
% yt_b = [0.5 0.5 -0.5 -0.5];
% xt_b = [0.05, 0.05, -0.05, -0.05]
% yt_b = [0.05, -0.05, 0.05, -0.05]
xt_b = [-0.09 0.09 0.09 -0.09];
yt_b = [0.12 0.12 -0.12 -0.12];
% xt_b = [-0.09 0.09 -0.09 0.09];
% yt_b = [0.12 0.12 -0.12 -0.12];

height = 0.2;
ct_b = xt_b+j*yt_b;

% xt_b = [-0.1 0.5 -0.5 0.5];
% yt_b = [0.1 0.5 -0.5 -0.5];

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

average_len = 10;
anch_pos = xa+j*ya;



for r = 1 : size(arr,3)
    dist_a = transpose(arr(:,:,r));
    dist = sqrt(dist_a.^2-height^2);
    tag_pos_b = xt_b + j*yt_b;

    [tag_pos_est, heading_est] = GetUWBPos_v1(xa, ya, dist, angles_from_heading);

    TT = max(max(abs(abs(transpose(Xa)-tag_pos_est)-dist_a)));
    
    Xt_e = real(tag_pos_est);
    Yt_e = imag(tag_pos_est);
    Xt_c_e = mean(Xt_e);
    Yt_c_e = mean(Yt_e);

    %% Check Blocked Anchor-Tag
    for gg = 1 : Ln
        A{gg}=[];
    end

    QDD = 1;
    for qd = 1 : Ln
        for qc = qd+1 : Ln
            if qd ~= qc
                for gg = 1 : Ln
                    if qd~=gg && qc~=gg
                        A{gg} = [A{gg} QDD];
                    end
                end
                QDD = QDD + 1;
            end
        end
    end

    %         KK = zeros(6,4);
    KK = [];

    for PP = 1 : Lp
        Pos{PP} = [];
        PosB{PP} = [];
        Prob2T{PP} = [];

        for NN = 1 : Ln
            for MM = NN+1 : Ln
                if NN~=MM
                    [Pos2,Prob2]=TwoAnchPos4([xa(NN) xa(MM)], [ya(NN) ya(MM)], [dist(NN,PP) dist(MM,PP)],[real(tag_pos_est(PP)) imag(tag_pos_est(PP))],[Xt_c_e Yt_c_e],anch_pos,dist(:,PP));

                    Pos{PP}=[Pos{PP};Pos2(1,:)];
                    PosB{PP}=[PosB{PP};Pos2(2,:)];
                    Prob2T{PP}=[Prob2T{PP};Prob2'];
                    %                         Pos{PP}=[Pos{PP};TwoAnchPos2([xa(NN) xa(MM)], [ya(NN) ya(MM)], [dist(NN,PP) dist(MM,PP)],[real(tag_pos_est(PP)) imag(tag_pos_est(PP))],[Xt_c_e Yt_c_e],anch_pos,dist(:,PP))];
                    %                         Pos{PP}=[Pos{PP};TwoAnchPos([xa(NN) xa(MM)], [ya(NN) ya(MM)], [dist(NN,PP) dist(MM,PP)],[Xt_c_e Yt_c_e])];
                end
            end
        end

        Temp = Prob2T{PP}(:,1)./Prob2T{PP}(:,2)*2;
        [ValM,IndM] = sort(Temp,'descend');
        TopI = 1;
        while ((ValM(TopI)>1)&&(ValM(TopI)>(ValM(end)*1)))
            if (sum((PosB{PP}(IndM(1))-Pos{PP}(IndM(end))).^2)-sum((Pos{PP}(IndM(1))-Pos{PP}(IndM(end))).^2))<0
                Pos{PP}(IndM(1))=PosB{PP}(IndM(1));
            end
            TopI = TopI + 1;
        end


        PosC{PP}=Pos{PP}(:,1)+j*Pos{PP}(:,2);
        if isempty(KK)
            KK = abs(PosC{PP}-anch_pos)-dist(:,PP)';
        else
            KK = KK + abs(PosC{PP}-anch_pos)-dist(:,PP)';
        end

        QQ{PP} = abs(PosC{PP}-anch_pos)-dist(:,PP)';
    end

    Td_sum = 0;
    Td_abs_sum = 0;

    for Pq = 2 : Lp
        Td{Pq-1} = (PosC{Pq}-PosC{Pq-1})/(ct_b(Pq)-ct_b(Pq-1));
        Td_sum = Td_sum + Td{Pq-1};
        Td_abs_sum = Td_abs_sum + abs(Td{Pq-1});
    end
    Td{Lp} = (PosC{1}-PosC{Lp})/(ct_b(1)-ct_b(Lp));
    Td_sum = Td_sum + Td{Lp};
    Td_sum = Td_sum/Lp;
    Td_abs_sum = Td_abs_sum + abs(Td{Lp});
    Td_abs_sum = Td_abs_sum/Lp;
    %         abs(Td_sum)./Td_abs_sum

    for CD = 1 : Ln
        TE = [1:Ln];
        TE(CD)=[];
        AB(CD) = sum(sum(abs(KK(A{CD},TE))));
        BB(CD) = -sum(KK(A{CD},CD));
    end

    CC = BB./AB;
    AB(find(BB<0)) = AB(find(BB<0)) + 10;
    %         [Val,indi] = sort((CC),'descend');
    [Val,indi] = sort(abs(AB));

    if (indi(1))~=3
        dd = 1;
    end

    AT = A{indi(1)};

    for ls = 1 : Lp
        biasV(ls) = mean(QQ{ls}(AT,indi(1)));
    end

    dist_n = dist;
    dist_n(indi(1),:) = dist_n(indi(1),:)+biasV;

    [tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e] = GetUWBPosUpdate_v1(xa, ya, Xt_c_e, Yt_c_e, heading_est, rl, dist_n, angles_from_heading);
    tag_pos_est = get_tag_pos(tag_center_pos_est, heading_est, tag_pos_b);
    

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

    figure(1);
    hold off;
    plot(xa,ya,'bo');
    hold on;

    %     plot(tag_pos_g(1), 'go')
    %     plot(tag_pos_g(2), 'g*')
    %     plot(tag_pos_g(3), 'gv')
    %     plot(tag_pos_g(4), 'g^')

    plot(tag_pos_est(1), 'bo')
    plot(tag_pos_est(2), 'b*')
    plot(tag_pos_est(3), 'bv')
    plot(tag_pos_est(4), 'b^')

    plot(K_tag_pos_est(1), 'ro')
    plot(K_tag_pos_est(2), 'r*')
    plot(K_tag_pos_est(3), 'rv')
    plot(K_tag_pos_est(4), 'r^')

    % 화살표 그리기
    %     quiver(x(r), y(r), cos(heading(r)), sin(heading(r)), 'g', 'LineWidth', 1, 'MaxHeadSize', 0.5);

    quiver(Xt_c_e, Yt_c_e, cos(heading_est), sin(heading_est), 'b', 'LineWidth', 1, 'MaxHeadSize', 0.5);

    quiver(K_Xt_c_e, K_Yt_c_e, cos(K_heading_est), sin(K_heading_est), 'r', 'LineWidth', 1, 'MaxHeadSize', 0.5);

    %     if abs((x(r)+j*y(r))-(Xt_c_e+j*Yt_c_e))>1
    %         gg = 1;
    %     end
end

title('Original(Blue) and estimated(Red) Tag to Anchor distance')

figure(4)
hold off;
plot(centerest_a_aver(:,1), centerest_a_aver(:,2), 'bo')
hold on;
% plot(x(:), y(:), 'g^')
plot(K_centerest_a_aver(:,1), K_centerest_a_aver(:,2), 'ro')
plot(centerest_a(:,1), centerest_a(:,2), 'ko')
axis([-5 5 -5 5]); axis equal





