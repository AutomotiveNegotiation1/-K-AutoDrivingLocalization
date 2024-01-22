function [p_ned] = uwb_pos_update_1(UWB, Pos)

    anchor = UWB.anchor(UWB.activeIndex,:);

    P = 1000*eye(3);

    p_ned = Pos;
    
    zhat_tmp = zeros(1,UWB.num);
    for i=1:UWB.num
        zhat_tmp(i) = norm(anchor(i,2:4) - p_ned');
    end
    zhat = zhat_tmp';


    los = zeros(UWB.num,3);
    for i=1:UWB.num
        los(i,:) = (anchor(i,2:4) - p_ned') / norm(anchor(i,2:4) - p_ned');
    end
    
    H = zeros(UWB.num,3);
    H(:,1:3) = -los;
    
    R = 1000*eye(UWB.num);
    Z = UWB.dist(UWB.activeIndex)' - zhat;
    
%     %%% Altitude
%     H(end+1,3) = 1;
%     R(end+1,end+1) = 0.01;
%     Z(end+1) = 0 - p_ned(3);
    
    
    K = P*H'/(H*P*H'+R);
    P = P - K*H*P;                
    err_X = zeros(3,1) + K*Z;
    
    
    p_ned = p_ned + err_X(1:3);
    
end