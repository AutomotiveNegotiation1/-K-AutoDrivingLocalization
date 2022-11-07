% clear all;
% close all;

fname = 'rosbag\test_b1_line_20220803_v1.mat';

% load(fname,'Sens');
TimeStart = 0;
TimeEnd = 0;
AlignOn = 1;
InitPos = [0 0 0]';
k = 1;
for i = 1 : Sens.length
    if Sens.flag(i,4) > 0
        [UWB] = func_extract_uwb(Sens, i, fname);
        [UWB] = uwb_LS_positioning(UWB);
        UWB_pos(:,k) = UWB.pos;
        if k > 2
            prev_diff = norm(UWB_pos(:,k-1)-UWB_pos(:,k-2));
        end
            
        ActiveAnchor(k) = length(UWB.activeIndex);
        if k == 1
            TimeStart = UWB.time;
            UWB_time_prev = TimeStart;
        end
        dt = UWB.time - UWB_time_prev;
        if AlignOn == 1
            if UWB.time-TimeStart > 10
                AlignOn = 0;
                UWB_pos_r(:,k) = InitPos;
            else
                InitPos = InitPos*(k-1)/k + UWB.pos*(1/k);
            end
        else
            if (sum(isnan(UWB_pos(:,k)))>0) || (norm(UWB_pos_r(:,k-1)-UWB_pos(:,k)) > (prev_diff*10))
                UWB_pos_r(:,k) = UWB_pos_r(:,k-1);
            else
%                 UWB_pos_r(:,k) = UWB_pos_r(:,k-1)+[1 0 0;0 1 0;0 0 1]*(UWB_pos(:,k)-UWB_pos_r(:,k-1));
                UWB_pos_r(:,k) = UWB_pos_r(:,k-1)+[0.1 0 0;0 1 0;0 0 0.1]*(UWB_pos(:,k)-UWB_pos_r(:,k-1));

            end
        end
        
        
        k = k + 1;
    else
        UWB.flag = false;
    end

    
end

figure(1);hold off;plot(UWB_pos_r(1,:), UWB_pos_r(2,:)); hold on;plot(UWB_pos(1,:), UWB_pos(2,:),'r');