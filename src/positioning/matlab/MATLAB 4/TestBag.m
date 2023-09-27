clear all;

% aa = rosbag('2023-07-26-18-35-13.bag');
aa = rosbag('2023-08-07-11-03-21.bag');
aa = rosbag('2023-08-07-14-34-54.bag');

LenTot = 0;
for dfe = 1 : 4
    UWB{dfe} = select(aa,'Topic',['/dwm1001/anchor/ttyUWB',num2str(dfe-1,1)] );
    LenTot = LenTot + size(UWB{dfe}.MessageList,1);
    UWBMsg{dfe} = readMessages(UWB{dfe},'DataFormat','struct');
end

for dfe = 1 : 4
    UWBTag{dfe} = select(aa,'Topic',['/dwm1001/ttyUWB',num2str(dfe-1,1)] );
    UWBTagMsg{dfe} = readMessages(UWBTag{dfe},'DataFormat','struct');
end

LnC = 0;
Lp = 4;
Ln = 6;

% xt_b = [-0.075 0.075 -0.075 0.075];
% yt_b = [0.12 0.12 -0.12 -0.12];

xt_b = [-0.07 0.07 -0.07 0.07];
yt_b = [0.135 0.135 -0.135 -0.135];

% Center = [0.700 3.024]
% tag1 = [0.694 3.166]
% tag2 = [0.786 3.165]
% tag3 = [0.674 2.893]
% tag4 = [0.814 2.892]

tag_pos_b = xt_b + j*yt_b;

AnchorIDmapH = [];
AnchorIDmap = [];
xa = [];
ya = [];
za = [];
k0 = ones(1,4);
RxDistTot = zeros(1,5);

for k = 1 : LenTot-10

    for lk = 1 : 4
        s_time_a(lk) = double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Sec)+double(UWBMsg{lk}{k0(lk)}.Header.Stamp.Nsec)/10^9;
    end

    [min_stamp_v, min_stamp_i] = min(s_time_a);
    s_time = min_stamp_v;

    PP = min_stamp_i;
    RxID = [];
    TT = UWBMsg{PP}{k0(PP)}.Id;
    for df = 1 : length(TT)
        [val,ids] = find(AnchorIDmap == hex2dec(TT{df}));
        if length(ids)>0
            RxID(df) = ids;
            
        else
            LnC = LnC + 1;
            AnchorIDmapH{LnC} = TT{df};
            AnchorIDmap(LnC) = hex2dec(AnchorIDmapH{LnC});
            RxID(df) = LnC;
            xa(LnC) = UWBMsg{PP}{k0(PP)}.X(df);
            ya(LnC) = UWBMsg{PP}{k0(PP)}.Y(df);
            za(LnC) = UWBMsg{PP}{k0(PP)}.Z(df);
        end
    end

    RxDistOrig = UWBMsg{PP}{k0(PP)}.DistanceFromTag;
    RxDist = real(sqrt(RxDistOrig.^2-(za(1)-1.45)^2));
    %RxDist = RxDistOrig;

    Nanchor = length(RxID);

    [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v] = UWBpos(Ln, Lp, PP, Nanchor, RxID, RxDist, s_time, tag_pos_b, xa, ya);
    
    RxIDT{k} = RxID;
    RxTime(k,PP) = s_time;
    RxIDTot(k,PP) = length(RxID);
    
    figure(1);hold off;plot(xa,ya,'ro');hold on;plot(tag_pos_est_aver,'.');axis equal

    tag_pos_est_total{k} = tag_pos_est;
    center_pos_est_total(k) = mean(tag_pos_est);
    headingest_total(k) = heading_est;
    center_pos_est_aver_total(k) = mean(tag_pos_est_aver);
    headingest_aver_total(k) = headingest_a_aver_v;
    k0(PP) = k0(PP) + 1;
    RxDistOrigTot(k,RxID,PP)=RxDistOrig;
    RxDistTot(k,RxID,PP)=real(sqrt(RxDist.^2-0.15^2));
end

for qik = 1 : 4
    [val,indx] = find(RxTime(:,qik)~=0);
    RxTimeTag{qik} = RxTime(val,qik);
    RxIDTotTag{qik} = RxIDTot(val,qik);
end

figure(33);hold off;plot(center_pos_est_total(40+1:end),'.')
figure(33);hold on;plot(center_pos_est_aver_total(40+1:end),'r.')
figure(35);hold off;plot(mod(headingest_total(40+1:end)+pi,2*pi)*180/pi)
figure(35);hold on;plot(mod(headingest_aver_total(40+1:end)+pi,2*pi)*180/pi,'r')

An = xa + j*ya;
% tag_pos = 0.21+2.96j;
tag_pos = 0.700 + j*3.024;
for kk = 1 : 4
    dist_exp(:,kk) = abs(An-(tag_pos+tag_pos_b(kk)));
end

for g = 1 : LnC
    figure(100+g)
    for m = 1 : Lp
        figure(100+g);hold off;subplot(2,2,m);plot(RxDistTot(:,g,m),'.');hold on;plot(dist_exp(g,m)*ones(1,size(RxDistTot,1)),'r')
    end
end

for g = 1 : LnC
    figure(200+g);
    for m = 1 : Lp
        figure(200+g);hold off;subplot(2,2,m);plot(RxDistOrigTot(:,g,m),'.');hold on;plot(dist_exp(g,m)*ones(1,size(RxDistOrigTot,1)),'r')
    end
end

for g = 1 : LnC
    for m = 1 : 4

        RxDistOrigTotNonzeroInd = find(RxDistOrigTot(:,g,m)~=0);
        RxDistOrigTotNonzero = RxDistOrigTot(RxDistOrigTotNonzeroInd,g,m);
        aver_dist_org(g,m)=mean(RxDistOrigTotNonzero);

        RxDistTotNonzeroInd = find(RxDistTot(:,g,m)~=0);
        RxDistTotNonzero = RxDistTot(RxDistTotNonzeroInd,g,m);
        aver_dist(g,m)=mean(RxDistTotNonzero);
        
    end
end

RxIDTot = zeros(length(RxIDT),5);
for df = 1 : length(RxIDT)
    RxIDL(df) = length(RxIDT{df});
    RxIDTot(df,1:RxIDL(df)) = RxIDT{df};
end
figure(99);plot(RxIDTot,'.')

figure(88);hold off;plot(RxIDL/10,'r');hold on;plot(center_pos_est_total)

