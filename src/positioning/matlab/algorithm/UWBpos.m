function [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v] = UWBpos(Ln, Lp, TagNum, Nanchor, RxID, RxDist, s_time, tag_pos_b, xa, ya)

average_len = 10;
NumInterpPoint = 12;
persistent r InterpPosition Tag_Pos_List TagDistInitCount TagDistInit heading_est_a centerest_a centerest_a_aver headingest_a_aver RxIDprev PPprev RxIDprevLen

if isempty (r)
    r = 0;
    InterpPosition = zeros(Lp,2)+0j;
    Tag_Pos_List = zeros(NumInterpPoint,2,Lp)+0j;
    TagDistInitCount = zeros(Ln,Lp);
    TagDistInit = zeros(Ln,Lp);
    heading_est_a = zeros(average_len*2,1);
    centerest_a = zeros(average_len*2,2);
    centerest_a_aver = zeros(average_len*2,2);
    headingest_a_aver = zeros(average_len*2,1);
    RxIDprev = zeros(4,Ln);
    RxIDprevLen = zeros(1,4);
    PPprev = zeros(1,4);
end
r = r + 1;

PP = TagNum;
anch_pos = xa + j*ya;
    tag_pos_est = zeros(1,Lp);
    heading_est = 0;
    tag_pos_est_aver=zeros(1,Lp);
    headingest_a_aver_v = 0;

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

    for LLp= 1 : Lp
        for lo = 1 : NumInterpPoint
            Tag_Pos_List(lo,:,LLp) = [s_time+(-NumInterpPoint+lo)*0.1 tag_pos_est(LLp)];
        end
    end

else
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%  Next Position Prediction    %%%%%%%%%%%%%%%%%%
    for PPC = 1 : Lp
        [InterpPosition(PPC,1)] = InterpPos(Tag_Pos_List(:,1,PPC),real(Tag_Pos_List(:,2,PPC)),s_time);
        [InterpPosition(PPC,2)] = InterpPos(Tag_Pos_List(:,1,PPC),imag(Tag_Pos_List(:,2,PPC)),s_time);
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    TagDistInitPrev = TagDistInit;
    TagDistInit(RxID,PP) = RxDist(:); 

%     NP = size(TagDistInit,1);
%     AnchIDList = [1:NP];
%     for kk = 1 : 4
%         if kk == 1
%             A = RxIDprev{kk};
%         else
%             for ll = 1 : length(RxIDprev{kk})
%                 if length(find(RxIDprev{kk}(ll)==A))==0
%                     A = [A RxIDprev{kk}(ll)];
%                 end
%             end
%         end
%     end

    Xt_c_e = mean(InterpPosition(:,1));
    Yt_c_e = mean(InterpPosition(:,2));


    PosC = [];
    PosC_E = [];

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%% New Position Calc.   %%%%%%%%%%%%%%%%%%%%%%%%%%%

    %         for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1
    if length(RxID)>1
%         [tag_pos_est, heading_est, CandPos] = GetPos2(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,InterpPosition(:,1)+j*InterpPosition(:,2));
%         [tag_pos_est, heading_est, CandPos] = GetPos3(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,InterpPosition(:,1)+j*InterpPosition(:,2),TagDistInitPrev,RxIDprev);
        [tag_pos_est, heading_est, CandPos] = GetPos3(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,InterpPosition(:,1)+j*InterpPosition(:,2),TagDistInit,RxIDprev, RxIDprevLen, PPprev);
% 
        [tag_pos_est, heading_est, CandPos] = GetPosRefine2(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,tag_pos_est, heading_est, CandPos);

%         [tag_pos_est, heading_est] = GetInitPos(xa(A),ya(A),TagDistInit,anch_pos(A),tag_pos_b,length(A),Lp);
%         CandPos = tag_pos_est(PP);
    else
        InterpPosT = InterpPosition(:,1)+j*InterpPosition(:,2);
        TempC = mean(InterpPosT);
        Est_H_p = 0+0j;
        for P = 1 : length(InterpPosT)
            Est_H_p = Est_H_p+((InterpPosT(P))-TempC)/tag_pos_b(P);
        end
        Est_H = angle(Est_H_p);

        for P = 1 : size(InterpPosT,1)

            tag_pos_est(P) = TempC + tag_pos_b(P)*exp(j*Est_H);

        end

        CandPos = InterpPosition(PP,1)+j*InterpPosition(PP,2);
    end
    
    %%%%%%%%%%%%%%  Original %%%%%%%%%%%%%
    Tag_Pos_List(1:NumInterpPoint-1,:,PP) = Tag_Pos_List(2:NumInterpPoint,:,PP);
    Tag_Pos_List(NumInterpPoint,:,PP) = [s_time CandPos];
    
    %%%%%%%%%%%%%%  Test %%%%%%%%%%%%%
%     Tag_Pos_List(1:NumInterpPoint-1,:,:) = Tag_Pos_List(2:NumInterpPoint,:,:);
%     Tag_Pos_List(NumInterpPoint,1,:) = s_time;
%     Tag_Pos_List(NumInterpPoint,2,:) = tag_pos_est;
%     Tag_Pos_List(NumInterpPoint,:,PP) = [s_time CandPos];
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


    %             Tag_Pos_List(1:2,:,PP) = Tag_Pos_List(2:3,:,PP);
    %             Tag_Pos_List(3,:,PP) = [s_time(r) tag_pos_est(PP)];
    Xt_c_e = real(mean(tag_pos_est));
    Yt_c_e = imag(mean(tag_pos_est));
    %         end

    tag_center_pos_est = mean(tag_pos_est);

    heading_est_a(1:end-1) = heading_est_a(2:end);
    
        if (heading_est - heading_est_a(end-1,:)) > pi
            heading_est_a(end) = heading_est - 2*pi;
        elseif (heading_est_a(end-1) - heading_est) > pi
            heading_est_a(end) = heading_est + 2*pi;
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
        headingest_a_aver(end) = mod([(MeanB_head + (MeanB_head-MeanA_head)/2)],2*pi) ;
    else
        centerest_a_aver(end,:) = [real(Xt_c_e) real(Yt_c_e)];
        headingest_a_aver(end) = heading_est_a(end);
    end

%     tag_pos_est_aver = get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2), headingest_a_aver(end), tag_pos_b);
    [K_Xt_c_e] = centerest_a_aver(end,1);
    [K_Yt_c_e] = centerest_a_aver(end,2);
    tag_pos_est_aver = tag_pos_b*exp(j*(headingest_a_aver(end)))+K_Xt_c_e+j*K_Yt_c_e;
    headingest_a_aver_v = headingest_a_aver(end);
%     K_heading_est = mod(headingest_a_aver(r),2*pi);
%     K_centerest_a_aver(r, :) = centerest_a_aver(r,:);
%     K_headingest_a_aver(r) = mod(headingest_a_aver(r),2*pi);

end

% RxIDprev{1} = RxIDprev{2};
% RxIDprev{2} = RxID;
RxIDprevLen(1:3) = RxIDprevLen(2:end);
RxIDprevLen(end) = length(RxID);

RxIDprev(1:3,:) = RxIDprev(2:end,:);
RxIDprev(end,1:RxIDprevLen(end)) = RxID;
PPprev(1:3) = PPprev(2:4);
PPprev(4) = PP;


