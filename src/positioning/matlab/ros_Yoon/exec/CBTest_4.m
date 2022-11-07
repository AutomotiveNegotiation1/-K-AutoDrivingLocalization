clear all;
clc
warning off

load('cali_params.mat');
cam = webcam('Logitech StreamCam');


RefChecker = checkerboard(67,3,2)';
[imagePointsRef,boardSize] = detectCheckerboardPoints(RefChecker);
imagePointsRef = imagePointsRef - imagePointsRef(8,:);
figure(100);imshow(RefChecker)
k = 1;
mid_x = 0;
mid_y = 0;
marker_pos = [0 0;1825 3650];
figure(2);hold off;
plot(marker_pos(1,:),marker_pos(2,:),'Rd');

worldPoints = imagePointsRef;
while 1
    im = (snapshot(cam));
%     figure(100);imshow(im);
    [img, newOrigin] = undistortImage(im, cameraParams, OutputView = "full");

    figure(1); hold off
    image(img);

    [imagePoints,boardSize] = detectCheckerboardPoints(img);
%     corners = detectHarrisFeatures(rgb2gray(img);
    if size(imagePoints,1)==size(imagePointsRef,1)&&(sum(sum(isnan(imagePoints)))==0)
    imagePoints = imagePoints + newOrigin;

    img_t = img;
    camIntrinsics = cameraParams.Intrinsics;

    % extrinsic 파라미터
    camExtrinsics = estimateExtrinsics(imagePoints, worldPoints, camIntrinsics);
    x = camExtrinsics.Translation(1)
    y = camExtrinsics.Translation(3)
    figure(2);hold on;plot(x,y,'r.');axis equal
%     
%         TempX = reshape(imagePoints(:,1),3,5);
%         TempY = reshape(imagePoints(:,2),3,5);
%         
%         d1 = TempY(3,1)-TempY(1,1);
%         d2 = TempY(3,3)-TempY(1,3);
%         d3 = TempY(3,5)-TempY(1,5);
% 
% 
%         mid_x_prev = mid_x;
%         mid_y_prev = mid_y;
%         i_x_max = max(imagePoints(:,1));
%         i_x_min = min(imagePoints(:,1));
%         i_y_max = max(imagePoints(:,2));
%         i_y_min = min(imagePoints(:,2));
%         mid_x = (i_x_max+i_x_min)/2;
%         mid_y = (i_y_max+i_y_min)/2;
%         imag_x_s = max(1,round(mid_x-(i_x_max-i_x_min)*4));
%         imag_x_e = min(size(img,2),round(mid_x+(i_x_max-i_x_min)*4));
%         imag_y_s = max(1,round(mid_y-(i_y_max-i_y_min)*4));
%         imag_y_e = min(size(img,1),round(mid_y+(i_y_max-i_y_min)*4));
% 
%         [msg]=readBarcode(img(imag_y_s:imag_y_e,imag_x_s:imag_x_e,:));
% 
% %         if length(RefPos)==2
%             RefPos = str2num(msg);
%             if length(RefPos)~=2
%                 RefPos = [0 0];
%             end
% %         else
% %             RefPos = 0;
% %         end
% 
%         if boardSize == [6,8]
%             if (img(round((imagePoints(1,2)+imagePoints(7,2))/2),round((imagePoints(1,1)+imagePoints(7,1))/2),1))>(img(round((imagePoints(3,2)+imagePoints(9,2))/2),round((imagePoints(3,1)+imagePoints(9,1))/2),1))
%                 sig = 1;
%             else
%                 sig = 1;
%             end
% 
%         end
%         tform = fitgeotform2d(imagePoints,imagePointsRef,'projective');
%         figure(1);hold on;
%         plot(imagePoints(:,1),imagePoints(:,2),'ro')
%         %     image(RefChecker)
%         Db_t = tform.A*[960 540 1]';
%         Db = Db_t(2)/1000;
% 
%         CosC = (Da^2+Db^2-Dc^2)/(2*Da*Db);
%         x = CosC*Da;
%         y = sqrt(Da^2-x^2);

%         pos = sig*(tform.A*[960 540 1]')+[RefPos 0]';
%         figure(2);hold on;plot(pos(1),pos(2),'o');axis equal
%         if (length(msg)==0)
            
%         pos = sig*(tform.A*[0 540 1]'-[0 280 0]'+[-130 630 0]')+[RefPos 0]'

    end
    %k = k + 1;


end

