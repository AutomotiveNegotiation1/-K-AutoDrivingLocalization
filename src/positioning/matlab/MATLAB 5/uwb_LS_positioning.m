function [UWB] = uwb_LS_positioning(UWB)

    nAnchor = UWB.num;

    meas_dist = UWB.dist(UWB.activeIndex);
    anchor = UWB.anchor(UWB.activeIndex,:);

    uwb_x = zeros(3,1);
    del_x = 10000*ones(3,1);

    b = zeros(nAnchor,1);
    A = zeros(nAnchor,3);


    cnt = 0;
    while norm(del_x(1:2)) > 0.001
        for i=1:nAnchor
            distance = norm( anchor(i,2:4) - uwb_x' );
            b(i) = meas_dist(i) - distance;
            A(i,:) = -( (anchor(i,2:4) - uwb_x') / distance );
        end


        del_x = pinv(A)*b; % Least Square Method
        uwb_x = uwb_x + del_x;
        cnt = cnt+1;

        if cnt>10000
            break;
        end
    end


    if norm(uwb_x(1:2))>1000
        uwb_x = uwb_x*NaN;
    end

    UWB.pos = uwb_x;
end