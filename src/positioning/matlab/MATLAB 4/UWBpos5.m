function [UWBout] = UWBpos5(Ln, Lp, LnC, TagNum, Nanchor, RxIDin, RxDistin, s_time, tag_pos_b, xain, yain, prevTagPos, prevTagHeading)
%[tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
average_len = 30;
NumInterpPoint = 16;
tag_center_vel_est = 0;

persistent r Tag_Pos_List heading_est_a centerest_a centerest_a_aver headingest_a_aver DistList DistListTime PrevPos PrevHeading

if isempty (r)
    r = 0;
    Tag_Pos_List = zeros(NumInterpPoint,2,Lp)+0j;
    heading_est_a = zeros(average_len*2,1);
    centerest_a = zeros(average_len*2,2);
    centerest_a_aver = zeros(average_len*2,2);
    headingest_a_aver = zeros(average_len*2,1);
    DistList = zeros(Ln,Lp,average_len);
    DistListTime = zeros(Ln,Lp,average_len);
    PrevPos = zeros(Lp,1);
    PrevHeading = 0;
end
if r < 1000
    r = r + 1;
end

RxID = RxIDin(1:Nanchor);
xa = xain(1:LnC);
ya = yain(1:LnC);
anch_pos = xa + j*ya;
RxDist = RxDistin(1:Nanchor);

prevDist = abs(transpose(anch_pos) - reshape(prevTagPos,1,4));

PP = TagNum;
anch_pos = xa + j*ya;

tag_pos_est = zeros(1,Lp);
heading_est = 0;

tag_pos_est_aver=zeros(1,Lp);
headingest_a_aver_v = 0;

for q = 1 : Nanchor
    %if RxDistin(q)<20
    DistList(RxID(q),TagNum,1:end-1) = DistList(RxID(q),TagNum,2:end);
    DistList(RxID(q),TagNum,end) = RxDistin(q);
    DistListTime(RxID(q),TagNum,1:end-1) = DistListTime(RxID(q),TagNum,2:end);
    DistListTime(RxID(q),TagNum,end) = s_time;
    %end
end

DistPredicted = zeros(Ln,Lp);

for q = 1 : Ln
    for p = 1 : Lp
        for rq = 1 : average_len
            if (s_time-DistListTime(q,p,rq)) > 1
                DistList(q,p,rq) = 0;
                DistListTime(q,p,rq) = 0;
            end
        end
        if length(find(DistList(q,p,:)~=0))>3
            DistPredicted(q,p) = InterpPos3(DistListTime(q,p,:),DistList(q,p,:),s_time,1);
%             if (s_time - DistListTime(q,p,end))< 0.1
%                 DistPredicted(q,p) = DistList(q,p,end);
%             else
%                 DistPredicted(q,p) = 0;
%             end



%             %         elseif (q<=size(prevDist,1)) && (p<=size(prevDist,2))
%             %             DistPredicted(q,p) = prevDist(q,p);
        else
            DistPredicted(q,p) = 0;
        end

    end
end

if min(sum(DistPredicted)) > 0   % (length(find(DistPredicted(:)~=0))>3)

% if (length(find(DistPredicted(:)~=0))>8)    
    % [tag_pos_est_t, heading_est_t] =  GetUWBPos_v3(xa, ya, DistPredicted(1:LnC,:), tag_pos_b, tag_pos_est, heading_est);


%         [tag_pos_est_t3, heading_est_t3] =  GetUWBPos_v2(xa, ya, DistPredicted(1:LnC,:), tag_pos_b);
    [tag_pos_est_t3, heading_est_t3, ErrSum] =  GetInitPos3(xa,ya, DistPredicted(1:LnC,:),xa+j*ya,tag_pos_b,LnC,4,PrevPos,PrevHeading);
% %     [tag_pos_est_t3, heading_est_t3] =  GetInitPos(xa,ya, DistPredicted(1:LnC,:),xa+j*ya,tag_pos_b,LnC,4);
% 
% 
%         Te = sum(abs(abs(transpose(anch_pos)-(tag_pos_est_t2)) - prevDist).^2,"all");
%         Tl = sum(abs(abs(transpose(anch_pos)-(tag_pos_est_t3)) - prevDist).^2,"all");
%         if Te < Tl
%             tag_pos_est_t3 = tag_pos_est_t2;
%             heading_est_t3 = heading_est_t2;
%         end

else


    tag_pos_est_t3 = PrevPos;
    heading_est_t3 = PrevHeading;

end

    Tag_Pos_List(1:NumInterpPoint-1,:,:) = Tag_Pos_List(2:NumInterpPoint,:,:);
    Tag_Pos_List(NumInterpPoint,:,1) = [s_time tag_pos_est_t3(1)];
    Tag_Pos_List(NumInterpPoint,:,2) = [s_time tag_pos_est_t3(2)];
    Tag_Pos_List(NumInterpPoint,:,3) = [s_time tag_pos_est_t3(3)];
    Tag_Pos_List(NumInterpPoint,:,4) = [s_time tag_pos_est_t3(4)];
    

PrevPos = tag_pos_est_t3;
PrevHeading = heading_est_t3;

tag_pos_est = tag_pos_est_t3;
heading_est = heading_est_t3;

tag_center_pos_est = mean(tag_pos_est);
tag_center_vel_est = mean((Tag_Pos_List(12,2,:)-Tag_Pos_List(1,2,:))./(Tag_Pos_List(12,1,:)-Tag_Pos_List(1,1,:)));
% heading_est_temp = mean(Tag_Pos_List(12,2,[2 4]))-mean(Tag_Pos_List(1,2,[1 3]));
% heading_est = angle(heading_est_temp);

heading_est_a(1:end-1) = heading_est_a(2:end);

TempPh = heading_est - heading_est_a(end-1,:);
NoT = int32(abs(TempPh)/(2*pi) );
if (heading_est - heading_est_a(end-1,:)) > pi
    heading_est_a(end) = heading_est - 2*pi*NoT;
elseif (heading_est_a(end-1) - heading_est) > pi
    heading_est_a(end) = heading_est + 2*pi*NoT;
else
    heading_est_a(end) = heading_est;
end

centerest_a(1:end-1,:) = centerest_a(2:end,:);
centerest_a(end,:) = [real(tag_center_pos_est) imag(tag_center_pos_est)];
centerest_a_aver(1:end-1,:) = centerest_a_aver(2:end,:);
headingest_a_aver(1:end-1,:) = headingest_a_aver(2:end,:);

if (r>(average_len*2))
    MeanA = mean(centerest_a(1:average_len,1)+j*centerest_a(1:average_len,2));
    MeanB = mean(centerest_a(average_len+1:average_len*2,1)+j*centerest_a(average_len+1:average_len*2,2));
    centerest_a_aver(end,:) = [real(MeanB + (MeanB-MeanA)/2) imag((MeanB + (MeanB-MeanA)/2))] ;
    MeanA_head = mean(heading_est_a(1:average_len));
    MeanB_head = mean(heading_est_a(average_len+1:average_len*2));
    headingest_a_aver(end) = ([(MeanB_head + (MeanB_head-MeanA_head)/2)]) ;
else
    centerest_a_aver(end,:) = [real(mean(tag_pos_est)) imag(mean(tag_pos_est))];
    headingest_a_aver(end) = heading_est_a(end);
end

%     tag_pos_est_aver = get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2), headingest_a_aver(end), tag_pos_b);
[K_Xt_c_e] = centerest_a_aver(end,1);
[K_Yt_c_e] = centerest_a_aver(end,2);
tag_pos_est_aver = tag_pos_b*exp(j*(mod(headingest_a_aver(end),2*pi)))+mean(tag_pos_est);
headingest_a_aver_v = mod(headingest_a_aver(end),2*pi);

%[tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
UWBout = zeros(1,18);
UWBout(1:4) = real(tag_pos_est_t3);
UWBout(5:8) = imag(tag_pos_est_t3);
UWBout(9) = heading_est_t3;
UWBout(10:13) = real(tag_pos_est_aver);
UWBout(14:17) = imag(tag_pos_est_aver);
UWBout(18) = headingest_a_aver_v;
UWBout(19:20) = [real(tag_center_vel_est) imag(tag_center_vel_est)];


