
Nsample = 90000;
% CamPos = (-0.065+j*0.74);
CamPos = CAMPos;
Front = 0;

TimeLag = 1e-3;
% TimeLag = 0;

SLAMposTr = transpose(SLAMpos);

tempB = find(SLAMposTr(1,1:end)~=0);
tempC = find(PosHUWB~=0);

[LsI] = min(find(SLAMtime(tempB(1)) < (PosHUWBtime(tempC))));
[LfI] = max(find(SLAMtime(tempB(end)) > (PosHUWBtime(tempC))));
Ls = tempC(LsI);
Lf = tempC(LfI);

Desa = find(PosHUWBtime(tempC)<PosHUWBtime(tempC(LsI))+TimeLag);

TimeIndx = [PosHUWBtime(tempC(Desa(end)+1)):1e-3:PosHUWBtime(tempC(LfI))];

SLAMposInterp = [];
SLAMposInterp(1,:) = interp1(SLAMtime(tempB),SLAMposTr(1,tempB),TimeIndx);
SLAMposInterp(2,:) = interp1(SLAMtime(tempB),SLAMposTr(3,tempB),TimeIndx);
SLAMposInterp(3,:) = interp1(SLAMtime(tempB),SLAMposTr(2,tempB),TimeIndx);

UWBposInterp = [];

%% 중복되는 PosHUWBtime 제거 (sh.jang 2024.08.09)
[PosHUWBtimeUnique, uniqueIdx] = unique(PosHUWBtime(tempC));
PosHUWBUnique = PosHUWB(tempC);
PosHUWBUnique = PosHUWBUnique(uniqueIdx);

%% (sh.jang 2024.08.09)
UWBposInterp(1,:) = interp1(PosHUWBtimeUnique, real(PosHUWBUnique), TimeIndx-TimeLag);
UWBposInterp(2,:) = interp1(PosHUWBtimeUnique, imag(PosHUWBUnique), TimeIndx-TimeLag);

%% Complex value generation
UWBposInterpC = UWBposInterp(1,:)+j*UWBposInterp(2,:);

HeadingHUWBcont(1) = HeadingHUWB(tempC(1));
for dge = 2 : length(tempC)
    teta = (HeadingHUWBcont(dge-1)-HeadingHUWB(tempC(dge)));
    if teta >= 2*pi
        ncount = floor(teta/(2*pi));
        HeadingHUWBcont(dge) = HeadingHUWB(tempC(dge))+2*pi*ncount;
    elseif teta <= -2*pi
        ncount = floor(-teta/(2*pi));
        HeadingHUWBcont(dge) = HeadingHUWB(tempC(dge))-2*pi*ncount;
    else
        HeadingHUWBcont(dge) = HeadingHUWB(tempC(dge));
    end
    if (HeadingHUWBcont(dge)-HeadingHUWBcont(dge-1))>pi
        HeadingHUWBcont(dge) = HeadingHUWBcont(dge)-2*pi;
    elseif (HeadingHUWBcont(dge)-HeadingHUWBcont(dge-1))<-pi
        HeadingHUWBcont(dge) = HeadingHUWBcont(dge)+2*pi;
    end
end

%% 중복되는 PosHUWBtime 제거 (sh.jang 2024.08.09)
[PosHUWBtimeUnique, HeadinguniqueIdx] = unique(PosHUWBtime(tempC));
% HeadingHUWBcontUnique = HeadingHUWBcont(tempC);
HeadingHUWBcontUnique = HeadingHUWBcont(HeadinguniqueIdx);
%% 중복되는 PosHUWBtime 제거 (sh.jang 2024.08.09)
% UWBHeadingInterp = interp1(PosHUWBtime(tempC),HeadingHUWBcont,TimeIndx);
UWBHeadingInterp = interp1(PosHUWBtimeUnique,HeadingHUWBcontUnique,TimeIndx);

%%
Y = transpose(SLAMposInterp(3,1:Nsample)); A = [transpose(SLAMposInterp(1:2,1:Nsample)) ones(Nsample,1)];     %% Z = ax + by + c   
X = inv(transpose(A)*A)*transpose(A)*Y;

tep = asin(X);  %% atan?
MatX = eulr2dcm([-tep(2) tep(1) 0]);
Xtr = MatX'*SLAMposInterp(:,1:Nsample);

%figure(2134);plot3(Xtr(1,1:end),Xtr(2,1:end),-Xtr(3,1:end),'.')
%figure(2135);plot3(SLAMposInterp(1,:),SLAMposInterp(2,:),SLAMposInterp(3,:),'.')

Y = transpose(Xtr(3,:)); A = [transpose(Xtr(1:2,:)) ones(size(Xtr,2),1)];
X = inv(transpose(A)*A)*transpose(A)*Y;

UWBposVoffset = CamPos*exp(j*(UWBHeadingInterp));
% UWBposVoffset = CamPos*exp(j*(-UWBHeadingInterp));

UWBposV = [(real(UWBposVoffset+UWBposInterpC));(imag(UWBposVoffset+UWBposInterpC));2.3*ones(1,length(TimeIndx))];

UWBposVc = (UWBposV(1,1:Nsample)+j*UWBposV(2,1:Nsample));
Xtrc = Xtr(1,:)+j*Xtr(2,:);

TempDU = (UWBposVc(1:end-1))-(UWBposVc(end));
TempDS = (Xtrc(1:end-1))-(Xtrc(end));
% TempDU = [TempDU (UWBposVc(2:end))-(UWBposVc(1))];
% TempDS = [TempDS (Xtrc(2:end))-(Xtrc(1))];
% TempDU = [TempDU (UWBposVc([1:299,301:end]))-(UWBposVc(300))];
% TempDS = [TempDS (Xtrc([1:299,301:end]))-(Xtrc(300))];

TempDUs = [UWBposV(1:2,1:Nsample)];
TempDSs = [Xtr(1:2,:);ones(1,size(Xtr,2))];

Am = TempDUs*transpose(TempDSs)*inv(TempDSs*transpose(TempDSs));


% sA = TempDU*transpose(TempDS)*inv(TempDS*transpose(TempDS));
sA = mean(TempDU./TempDS);
s = sqrt(sum(abs(sA*[1;0;0]).^2));

% s = abs(sA)*1.03;

theta = angle(sA/s);
% theta = angle(sA/s)+0.14;
% s = s*1.03;
% sA = s*[cos(theta) -sin(theta) 0 ; sin(theta) cos(theta) 0; 0 0 1]*MatX';
AmE = eye(3);
AmE(1:2,1:2) = Am(:,1:2);
sA = AmE*MatX';
SLAMposInit = [Am(:,3);0];
eultv = dcm2eulr(sA);
roll = eultv(1);
if Front == 0
    pitch = eultv(3)+pi;
else
    pitch = eultv(3);
end

yaw = eultv(2);

TempTr = sA * SLAMposInterp + SLAMposInit;
TempTrAll = sA * SLAMposTr([1 3 2],:) + SLAMposInit;

ParkingLot = uint8([]);
%ParkingLotS= imread('B1_ParkingLot.jpg');
ParkingLotS= imread('robo_1f_1855_1027.png');

for sx = 1:size(ParkingLotS,1)
    for sy = 1:size(ParkingLotS,2)
        % ParkingLot(sy,sx,:) = ParkingLotS(size(ParkingLotS,1)-sx+1,size(ParkingLotS,2)-sy+1,:);
        ParkingLot(sy,sx,:) = ParkingLotS(sx, size(ParkingLotS,2)-sy+1,:);
        %ParkingLot(sy,sx,:) = ParkingLotS(sx, sy,:);
    end
end
ParkingLot = imrotate(ParkingLot, 90);

figure(2313);hold off;
imshow(ParkingLot); 
%Scale = 24.6;
Scale = 16.5;
hold on;

plot(Scale*xa+150 + j*(size(ParkingLot,2)-Scale*ya + 4.3*Scale*11),'ro');
hold on;
plot(Scale*UWBposV(1,:) + 150, size(ParkingLot,2)-Scale*UWBposV(2,:) + 4.3*Scale*11,'b.');
hold on;
%plot(Scale*TempTrAll(1,:), size(ParkingLot,2)- Scale*TempTrAll(2,:) - Scale*11,'r.')

plot(Scale*TempTrAll(1,:) + 150, size(ParkingLot,2)- Scale*TempTrAll(2,:) + 4.3*Scale*11,'r.')

save('MapParams_0820_chungla.mat','sA','s','roll','pitch','yaw','SLAMposInit');