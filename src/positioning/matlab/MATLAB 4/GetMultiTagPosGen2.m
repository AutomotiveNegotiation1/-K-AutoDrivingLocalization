function [Est_F,Est_C,Est_H] = GetMultiTagPosGen2(Pos2C,Va,tag_pos_b,Va_ind,PrevDist,RxIDprev, RxIDprevLen,xa,ya,PPprev)

ia=zeros(length(Va),1);
NP = size(PrevDist,1);
% AnchIDList = [1:NP];
% for kk = 1 : 4
%     if kk == 1
%         A = RxIDprev(kk,1:RxIDprevLen(kk));
%     else
%         for ll = 1 : (RxIDprevLen(kk))
%             if length(find(RxIDprev(kk,ll)==A))==0
%                  A = [A RxIDprev(kk,ll)];
%             end
%         end
%     end
% end


Est_P  = zeros(size(Pos2C,1),1)+0j;

for a = 1 : length(Va)
    if (Va(a)=='0') && (Va_ind(a)~=0)
        ia(a) = 1;
    elseif (Va(a)=='1') && (Va_ind(a)~=0)
        ia(a) = 2;
    else
        ia(a) = 1;
    end
end

% NB = length(tag_pos_b);
NB = length((Va));


Est_C = 0+0j;
for P = 1 : size(Pos2C,1)
    Est_C = Est_C + (Pos2C(P,ia(P)))/NB;
end

Est_H_p = 0+0j;
for P = 1 : size(Pos2C,1)
    Est_H_p = Est_H_p+((Pos2C(P,ia(P)))-Est_C)/tag_pos_b(P);
end
Est_H = angle(Est_H_p);

Est_F = 0;
for P = 1 : size(Pos2C,1)
%     [v,indp] = find(PPprev==P);
%     if length(indp)>1
%         indp = indp(end);
%     end
%     anch_pos = xa(RxIDprev{indp})+j*ya(RxIDprev{indp});
    Est_P(P) = Est_C + tag_pos_b(P)*exp(j*Est_H);
    Est_F = Est_F + abs(Est_P(P)-Pos2C(P,ia(P)))^2;
%     for q = 1 : length(RxIDprev{indp})
%         Est_F = Est_F + abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
%     end
end

% Est_F = 0;

for P = 1 : size(Pos2C,1)
    [v,indp] = find(PPprev==P);
    if length(indp)>1
        indp = indp(end);
    else
        indp = 1;
    end
    anch_pos = xa(RxIDprev(indp,1:RxIDprevLen(indp)))+j*ya(RxIDprev(indp,1:RxIDprevLen(indp)));
    Est_Ft = zeros(RxIDprevLen(indp),1);
    for q = 1 : RxIDprevLen(indp)
        Est_Ft(q) = abs(abs(anch_pos(q)-Est_P(P))-PrevDist(RxIDprev(indp,q),P))^2;
%         Est_Ft(q) = abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
    end
    Tt = sort(Est_Ft);
    Est_F = Est_F + sum(Tt(1:min(2,RxIDprevLen(indp))));
end



