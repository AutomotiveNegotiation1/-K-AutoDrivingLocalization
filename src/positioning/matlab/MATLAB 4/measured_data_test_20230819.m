clear al;

xt_b = [-0.525 0.525 -0.525 0.525];
yt_b = [0.505 0.505 -0.505 -0.505];
zt_b = 1.53;

tag_pos_b = (xt_b)+j*yt_b;

anch_pos_o  = [-0.01-0.3i	0+15.1i	7.85-0.3i	7.84+10.5i	22.7-4i    24.6+16.3i];

xa = real(anch_pos_o);
ya = imag(anch_pos_o);

dist_o1 = [6.42 5.44 6.85 5.94;13.92 13.58 13.00 12.63;3.37 3.98 4.18 4.72;8.27 8.66 7.24 7.70;17.84 18.83 18.29 19.24;0 24.52 23.00 23.85];
dist_o2 = [22.94 23.93 22.64 23.68;22.54 0 22.94 23.92;0 0 0 0;0 0 0 0;12.05 12.05 11.06 11.23;9 8.67 9.94 9.65];
ind = find(dist_o1==0);
ind2 = find(dist_o1==0);

dist_o1 = sqrt(dist_o1.^2-(2.3-zt_b)^2);
dist_o1(ind) = 0;

dist_o2 = sqrt(dist_o2.^2-(2.3-zt_b)^2);
dist_o2(ind2) = 0;

[tag_pos_est, heading_est] = GetUWBPos_v2(xa, ya, dist_o1, tag_pos_b);
[tag_pos_est3, heading_est3] = GetInitPos(xa,ya, dist_o1,anch_pos_o,tag_pos_b,6,4);

[tag_pos_est2, heading_est2] = GetUWBPos_v2(xa, ya, dist_o2, tag_pos_b);
[tag_pos_est4, heading_est4] = GetInitPos(xa,ya,dist_o2,anch_pos_o,tag_pos_b,6,4);

figure(87);hold off;plot(tag_pos_est,'ro');hold on;plot(anch_pos_o,'b*'); plot(tag_pos_est3,'r*')
figure(88);hold off;plot(tag_pos_est2,'ro');hold on;plot(anch_pos_o,'b*'); plot(tag_pos_est4,'r*')

for u = 1 : 6
    for v = 1 : 4
        measb1(u,v) = abs(anch_pos_o(u) - tag_pos_est(v));
        measc1(u,v) = abs(anch_pos_o(u) - tag_pos_est2(v));
        errb1(u,v) = abs(anch_pos_o(u) - tag_pos_est(v)) - dist_o1(u,v);
        errc1(u,v) = abs(anch_pos_o(u) - tag_pos_est2(v)) - dist_o2(u,v);
        measb2(u,v) = abs(anch_pos_o(u) - tag_pos_est3(v));
        measc2(u,v) = abs(anch_pos_o(u) - tag_pos_est4(v));
        errb2(u,v) = abs(anch_pos_o(u) - tag_pos_est3(v)) - dist_o1(u,v);
        errc2(u,v) = abs(anch_pos_o(u) - tag_pos_est4(v)) - dist_o2(u,v);
    end
end

aa = rosbag('test_0_2023-08-17-11-28-39.bag');

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

IMURoSV = select(aa,'Topic','/imu/angular_velocity');
IMUVMsg = readMessages(IMURoSV,'DataFormat','struct');

IMURoSA = select(aa,'Topic','/imu/acceleration');
IMUAMsg = readMessages(IMURoSA,'DataFormat','struct');

IMURoSM = select(aa,'Topic','/imu/mag');
IMUMMsg = readMessages(IMURoSM,'DataFormat','struct');

k0 = ones(1,4);

for PP = 1 : 4
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
%         A = [5 1;23 1];
%         Y = [5.0;23.20];
%         X = inv(A)*Y;
%         RxDistOrig = RxDistOrig*X(1)+X(2);

        RxDist = real(sqrt(RxDistOrig.^2-(za(1)-zt_b)^2));
        %         RxDistOrigTot(k,:) = zeros(1,6);
        %         RxDistTot(k,:) = zeros(1,6);
        %         RxDistOrigTot(k,RxID) = RxDistOrig;
        %         RxDistTot(k,RxID) = RxDist;

        Nanchor = length(RxID);
        RxIDUWB = zeros(Ln,1);
        RxIDUWB(1:Nanchor) = RxID;
        RxDistUWB = zeros(Ln,1);
        RxDistUWB(1:Nanchor) = RxDist;

        RxDistTot(RxID,PP,:) = RxDist
        xain = zeros(1,Ln);
        xain(1:LnC) = xa;

        yain = zeros(1,Ln);
        yain(1:LnC) = ya;
        
end

