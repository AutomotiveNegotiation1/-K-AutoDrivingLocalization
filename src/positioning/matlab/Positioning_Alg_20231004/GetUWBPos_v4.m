function [Xc, Yc, heading, flag] = GetUWBPos_v4(xa, ya, dist, Ln, Lp, tag_pos_b)

%% Get Center Pos
qq = 1;
flagc = 0;
flage = 0;
flags = 0;
flagw = 0;
flagn = 0;
flag = 0;

%% Preallocation
max_size = Ln; % Assuming Ln is the maximum size you'd iterate over
xat = zeros(1, max_size);
yat = zeros(1, max_size);
distc = zeros(1, max_size);

xae = zeros(1, max_size);
yae = zeros(1, max_size);
diste = zeros(1, max_size);

xaw = zeros(1, max_size);
yaw = zeros(1, max_size);
distw = zeros(1, max_size);

xan = zeros(1, max_size);
yan = zeros(1, max_size);
distn = zeros(1, max_size);

xas = zeros(1, max_size);
yas = zeros(1, max_size);
dists = zeros(1, max_size);

for kk = 1 : Ln
    if length(find(dist(kk,:)~=0)) == Lp
        xat(qq) = xa(kk);
        yat(qq) = ya(kk);
        distc(qq) = (CenterPosDist(dist(kk,1), dist(kk,4), abs(tag_pos_b(1)-tag_pos_b(4)))+CenterPosDist(dist(kk,2), dist(kk,3), abs(tag_pos_b(2)-tag_pos_b(3))))/2;
        %distc(qq) = mean(dist(kk,:));
        qq = qq + 1;
    elseif (dist(kk,1)~=0) && (dist(kk,4)~=0)
        xat(qq) = xa(kk);
        yat(qq) = ya(kk);
        distc(qq) = CenterPosDist(dist(kk,1), dist(kk,4), abs(tag_pos_b(1)-tag_pos_b(4)));
        %distc(qq) = mean(dist(kk,[1 4]));

        qq = qq + 1;
    elseif (dist(kk,2)~=0) && (dist(kk,3)~=0)
        xat(qq) = xa(kk);
        yat(qq) = ya(kk);
        distc(qq) = CenterPosDist(dist(kk,2), dist(kk,3), abs(tag_pos_b(2)-tag_pos_b(3)));
        %distc(qq) = mean(dist(kk,[2 3]));

        qq = qq + 1;
    end
end

if length(xat) >2
    [Xt_c_e, Yt_c_e] = TrilateralPos(xat, yat, distc); 
    flagc = 1;
else
    Xt_c_e = 0;
    Yt_c_e = 0;
end

% %% Get Side Pos
qq = 1;
rr = 1;
ss = 1;
tt = 1;

for kk = 1 : Ln
    if (dist(kk,2)~=0) && (dist(kk,4)~=0)
        xae(qq) = xa(kk);
        yae(qq) = ya(kk);
        %diste(qq) = mean(dist(kk,[2 4]));
        diste(qq) = CenterPosDist(dist(kk,2), dist(kk,4), abs(tag_pos_b(2)-tag_pos_b(4)));
        qq = qq + 1;
    end
    if (dist(kk,1)~=0) && (dist(kk,3)~=0)
        xaw(rr) = xa(kk);
        yaw(rr) = ya(kk);
        %distw(rr) = mean(dist(kk,[2 4]));
        distw(rr) = CenterPosDist(dist(kk,1), dist(kk,3), abs(tag_pos_b(1)-tag_pos_b(3)));
        rr = rr + 1;
    end
    if (dist(kk,1)~=0) && (dist(kk,2)~=0)
        xan(ss) = xa(kk);
        yan(ss) = ya(kk);
        %dists(ss) = mean(dist(kk,[2 4]));
        distn(ss) = CenterPosDist(dist(kk,1), dist(kk,2), abs(tag_pos_b(1)-tag_pos_b(2)));
        ss = ss + 1;
    end
    if (dist(kk,3)~=0) && (dist(kk,4)~=0)
        xas(tt) = xa(kk);
        yas(tt) = ya(kk);
        %dists(tt) = mean(dist(kk,[2 4]));
        dists(tt) = CenterPosDist(dist(kk,3), dist(kk,4), abs(tag_pos_b(3)-tag_pos_b(4)));
        tt = tt + 1;
    end
end

if length(xas) >2
    [Xt_s_e, Yt_s_e] = TrilateralPos(xas, yas, dists); 
    flags = 1;
else
    Xt_s_e = 0;
    Yt_s_e = 0;
end

if length(xae) >2
    [Xt_e_e, Yt_e_e] = TrilateralPos(xae, yae, diste); 
    flage = 1;
else
    Xt_e_e = 0;
    Yt_e_e = 0;
end

if length(xaw) >2
    [Xt_w_e, Yt_w_e] = TrilateralPos(xaw, yaw, distw); 
    flagw = 1;
else
    Xt_w_e = 0;
    Yt_w_e = 0;
end

if length(xan) >2
    [Xt_n_e, Yt_n_e] = TrilateralPos(xan, yan, distn); 
    flagn = 1;
else
    Xt_n_e = 0;
    Yt_n_e = 0;
end

if flagc == 1
    heading = angle((Xt_e_e-Xt_c_e)+j*(Yt_e_e-Yt_c_e));
    Xc = Xt_c_e;
    Yc = Yt_c_e;
    flag = 1;
elseif flage*flagw == 1
    heading = angle((Xt_e_e-Xt_w_e)+j*(Yt_e_e-Yt_w_e));
    Xc = (Xt_e_e+Xt_w_e)/2;
    Yc = (Yt_e_e+Yt_w_e)/2;
    flag = 1;
elseif flagn*flags == 1
    heading = angle((Xt_n_e-Xt_s_e)+j*(Yt_n_e-Yt_s_e))-pi/2;
    Xc = (Xt_n_e+Xt_s_e)/2;
    Yc = (Yt_n_e+Yt_s_e)/2;
    flag = 1;
else
    Xc = 0;
    Yc = 0;
    heading = 0;
    flag = 0;
end

