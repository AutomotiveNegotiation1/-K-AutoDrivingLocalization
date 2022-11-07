function [UWB] = uwb_LS_positioning_3(UWB, PosE)

    persistent Pos_prev
    persistent firstRun
    
    if isempty(firstRun)
        firstRun = 1;
        Pos_prev = [0 0 0]';
    end
    
    nAnchor = UWB.num;

    meas_dist = UWB.dist(UWB.activeIndex);
    anchor = UWB.anchor(UWB.activeIndex,:);

    for df = 1 : nAnchor
        Est_Dist(df) = norm(anchor(df,2:4)-PosE');
    end
    
    ErroDist = abs(Est_Dist' - meas_dist');
    
    Ind = find(ErroDist>0.2);
%     length(Ind)
    meas_dist(Ind) = meas_dist(Ind)-0.2;
    UWB.dist(UWB.activeIndex) = meas_dist;
    [UWB.pos] = uwb_pos_update_1(UWB, PosE);
    
%     uwb_x = Pos_prev;
% %     uwb_x(3) = 0;
%     del_x = 10000*ones(3,1);
% 
%     b = zeros(nAnchor,1);
%     A = zeros(nAnchor,3);
% 
% 
%     cnt = 0;
%     while norm(del_x(1:2)) > 0.01
%         for i=1:nAnchor
%             distance = norm( anchor(i,2:4) - uwb_x' );
%             b(i) = meas_dist(i) - distance;
%             A(i,:) = -( (anchor(i,2:4) - uwb_x') / distance );
%         end
% 
% 
%         del_x = pinv(A)*b; % Least Square Method
%         uwb_x = uwb_x + del_x;
%         cnt = cnt+1;
% 
%         if cnt>10000
%             break;
%         end
%     end
% 
% 
%     if norm(uwb_x(1:2))>1000
% %         uwb_x = uwb_x*NaN;
%         uwb_x = Pos_prev;
%     end
% 
%     UWB.pos = uwb_x;
%     Pos_prev = uwb_x;
end