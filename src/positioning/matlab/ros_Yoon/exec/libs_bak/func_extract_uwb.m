function [UWB] = func_extract_uwb(Sens, i, fname)
    
    n = Sens.flag(i,4);

    UWB.time = Sens.uwb.time(n);
%     UWB.num = Sens.uwb.activeNum(n);
    UWB.dist = Sens.uwb.ranging(n,:)';
    UWB.activeIndex = find(UWB.dist>0);
    UWB.num = length(UWB.activeIndex);
    
    if strcmp(extractAfter(extractBefore(fname,'_'),'\'), 'stop002')
        UWB.anchor = Sens.uwb.anchorID';
        UWB.anchor(1,2:3) = [-1.32 16.21]; % 991B [Y, X]
        UWB.anchor(2,2:3) = [nan nan]; % 4630
        UWB.anchor(3,2:3) = [nan nan]; % 4302
        UWB.anchor(4,2:3) = [1.32 16.21]; % 1121
        UWB.anchor(5,2:3) = [nan nan]; % 439D
        UWB.anchor(6,2:3) = [nan nan]; % 89A4
        UWB.anchor(7,2:3) = [1.32  0.46]; % 4599
        UWB.anchor(8,2:3) = [-1.32  0.46]; % 9B8F
        UWB.anchor(:,4) = -2;

    else
        UWB.anchor = Sens.uwb.anchorID';
       %% 20220803_v1, v3, v5
        UWB.anchor(1,2:3) = [-1.14   26.1]; % 991B
        UWB.anchor(2,2:3) = [nan   nan]; % 4630
        UWB.anchor(3,2:3) = [ 1.165  13.05]; % 4302
        UWB.anchor(4,2:3) = [-1.14   13.05]; % 1221
        UWB.anchor(5,2:3) = [-1.14    0]; % 439D
        UWB.anchor(6,2:3) = [ 1.165   0]; % 89A4
        UWB.anchor(7,2:3) = [nan   nan]; % 4599
        UWB.anchor(8,2:3) = [ 1.165   26.1]; % 9B8F
        UWB.anchor(:,4) = -1.5;
    end

    UWB.index = n;
    
    UWB.pos = zeros(3,1);
    
    UWB.flag = true;
end

