function CB_ID = getCheckerBoardID(image_left, imagePointsx, boardSizex)

ReShpX = reshape(imagePointsx(:,1),boardSizex-1);
ReShpY = reshape(imagePointsx(:,2),boardSizex-1);
AveDiffX =round(mean(mean(ReShpX(:,2:end)-ReShpX(:,1:end-1))));
AveDiffY =round(mean(mean(ReShpY(2:end,:)-ReShpY(1:end-1,:))));


TT1 = image_left(round(ReShpY(end,1)):round(ReShpY(end,1))+AveDiffY,round(ReShpX(end,1))-AveDiffX:round(ReShpX(end,1)),:);
% figure(89);imshow(TT1);

TT2 = image_left(round(ReShpY(end,1)):round(ReShpY(end,1))+AveDiffY,round(ReShpX(end,1)):round(ReShpX(end,2)),:);
% figure(90);imshow(TT2);

RefColor1 = mean(mean(mean(TT1(2:end-1,2:end-1,:))));
RefColor2 = mean(mean(mean(TT2(2:end-1,2:end-1,:))));
RefTH = (RefColor1+RefColor2)/2;
TET = image_left(round(ReShpY(end,1))+AveDiffY:round(ReShpY(end,1))+2*AveDiffY,round(ReShpX(end,1))-AveDiffX:round(ReShpX(end,end)+AveDiffX),:);
N = size(TET,2);
CB_ID = 0;
for k = boardSizex(2):-1:1
   TETT(k) = mean(mean(mean(TET(:,(k-1)*N/6+1:k*N/6,:))));
   if TETT(k) > RefTH
        CB_ID = CB_ID*2;
   else
       CB_ID = CB_ID*2 + 1;
   end
end


% figure(91);imshow(TET1);



