clear all;
fname = 'rosbag\test_0818_v3.mat';
% fname = 'rosbag\test_b1_line_20220803_v1.mat';

load(fname,'Sens');

picnum = 100;
for dfd = 1 : 8
    picnum=picnum+1;figure(picnum);hold off;plot(Sens.uwb.ranging(:,dfd));
end
%% 20220818
UWB.anchor(1,2:3) = [0     26.1]; % 991B
UWB.anchor(2,2:3) = [nan   nan]; % 4630
UWB.anchor(3,2:3) = [1.8   13.05]; % 4302
UWB.anchor(4,2:3) = [0     13.05]; % 1221
UWB.anchor(5,2:3) = [0     0]; % 439D
UWB.anchor(6,2:3) = [1.8   0]; % 89A4
UWB.anchor(7,2:3) = [nan   nan]; % 4599
UWB.anchor(8,2:3) = [1.8   26.1]; % 9B8F
UWB.anchor(:,4) = 1;
kdc = 1;

P(1,1) = UWB.anchor(3,2);
P(2,1) = UWB.anchor(4,2);
P(3,1) = UWB.anchor(5,2);
P(4,1) = UWB.anchor(6,2);
P(1,2) = UWB.anchor(3,3);
P(2,2) = UWB.anchor(4,3);
P(3,2) = UWB.anchor(5,3);
P(4,2) = UWB.anchor(6,3);
P(1,3) = UWB.anchor(3,4);
P(2,3) = UWB.anchor(4,4);
P(3,3) = UWB.anchor(5,4);
P(4,3) = UWB.anchor(6,4);

R(1,:) = Sens.uwb.ranging(1:280,3)';
R(2,:) = Sens.uwb.ranging(1:280,4)';
R(3,:) = Sens.uwb.ranging(1:280,5)';
R(4,:) = Sens.uwb.ranging(1:280,6)';

for k = 2 : 280
    MatA = -4*[P(1,1)-P(4,1) P(1,2)-P(4,2);P(2,1)-P(4,1) P(2,2)-P(4,2);P(3,1)-P(4,1) P(3,2)-P(4,2)];
    MatB = [R(1,k-1)^2+R(1,k)^2-(R(4,k-1)^2+R(4,k)^2)-2*(P(1,1)^2-P(4,1)^2)-2*(P(1,2)^2-P(4,2)^2);
            R(2,k-1)^2+R(2,k)^2-(R(4,k-1)^2+R(4,k)^2)-2*(P(2,1)^2-P(4,1)^2)-2*(P(2,2)^2-P(4,2)^2);
            R(3,k-1)^2+R(3,k)^2-(R(4,k-1)^2+R(4,k)^2)-2*(P(3,1)^2-P(4,1)^2)-2*(P(3,2)^2-P(4,2)^2)];
    pos(k-1,:) = inv(MatA'*MatA)*MatA'*MatB;
end

for fdd = 1 : Sens.length
    if Sens.flag(fdd,4) ~= 0
        Range(kdc,:) = Sens.uwb.ranging(kdc,:);
        
        UWB.time = Sens.uwb.time(kdc);
        UWB.dist = Sens.uwb.ranging(kdc,:)';
        UWB.activeIndex = find(UWB.dist>0);
        UWB.num = length(UWB.activeIndex);
        
     
        legn = 50;
        if kdc > legn
            RangeSumA = mean(Range(kdc-legn/2+1:kdc,:));
            RangeSumB = mean(Range(kdc-legn+1:kdc-legn/2,:));
            
            for kkd = 1 : 8
                if (~isnan(RangeSumA(kkd)))&&((~isnan(RangeSumB(kkd))))
                    TEMP_MEAN = (RangeSumA(kkd)+RangeSumB(kkd))/2;
                    TEMP_DIFF = RangeSumA(kkd)-TEMP_MEAN;
                    RangeN(kdc,kkd) =  RangeSumA(kkd) + TEMP_DIFF;
                else
                    if isnan(Range(kdc,kkd))
                        RangeN(kdc,kkd) = RangeN(kdc-1,kkd);
                    else
                        RangeN(kdc,kkd) = Range(kdc,kkd);
                    end
                    
                end
            end
            nAnchor = length(UWB.activeIndex);
            TEMPc = RangeN(kdc,:)';
            meas_dist = TEMPc(UWB.activeIndex);
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
                UWB_P(:,kdc) = UWB_P(:,kdc-1);
            else
                UWB_P(:,kdc) = uwb_x;
            end
            
            UWB_P(:,kdc) = uwb_x;
            
        else
            RangeN(kdc,:) = Range(kdc,:);
        end
        kdc = kdc + 1;
    end
end
picnum = 100;
for dfd = 1 : 8
    picnum=picnum+1;figure(picnum);hold on;plot(RangeN(:,dfd),'r');
end

picnum=picnum+1;figure(picnum);plot(UWB_P(1,:),UWB_P(2,:),'ro')
picnum=picnum+1;figure(picnum);plot(UWB_LS_Pos(1,:),UWB_P(2,:),'ro')