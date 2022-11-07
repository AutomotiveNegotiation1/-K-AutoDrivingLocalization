clear all;
clc
warning off

cam = webcam('Logitech StreamCam');

RefChecker = checkerboard(26,4,3)';
[imagePointsRef,boardSize] = detectCheckerboardPoints(RefChecker);
imagePointsRef = imagePointsRef - imagePointsRef(18,:);
figure(100);imshow(RefChecker)
k = 1;
mid_x = 0;
mid_y = 0;
marker_pos = [0 0 0;0 1825 3650];
figure(2);hold off;
plot(marker_pos(1,:),marker_pos(2,:),'Rd');
load 'camparams.mat'
while 1
    img = snapshot(cam);
    
    figure(1); hold off
    image(img);
    [im, newOrigin] = undistortImage(img, cameraParams, OutputView = "full");  
    figure(89);imshow(im)
    
    [imagePoints, boardSize] = detectCheckerboardPoints(im);
    
      imagePoints = imagePoints +   newOrigin; 
        imagePoints = double(imagePoints);
    
        % extrinsic 파라미터
        camExtrinsics = estimateExtrinsics(imagePoints, imagePointsRef, camIntrinsics);
    [imagePoints,boardSize] = detectCheckerboardPoints(img(400:900,:,:));
%     corners = detectHarrisFeatures(rgb2gray(img);
    imagePoints = imagePoints + [0 399];
    img_t = img;
    while (size(imagePoints,1)>35)&&~(length(imagePoints)==0)
        img_t(min(imagePoints(:,2)):max(imagePoints(:,2)),min(imagePoints(:,1)):max(imagePoints(:,1)),:)= 0;
%         figure(2);hold off;imshow(img_t)
        [imagePoints,boardSize] = detectCheckerboardPoints(img_t);
        
    
    end
    if size(imagePoints,1)==size(imagePointsRef,1)&&(sum(sum(isnan(imagePoints)))==0)
        tformA = fitgeotform2d(imagePoints,imagePointsRef+[imagePoints(18,1) 540],'affine');
        TempX = reshape(imagePoints(:,1),5,7);
        TempY = reshape(imagePoints(:,2),5,7);
        Tformed = tformA.A*[TempX(:,3)';TempY(:,3)';1 1 1 1 1];

        d1 = TempY(5,1)-TempY(1,1);
        d2 = TempY(5,4)-TempY(1,4);
        d3 = TempY(5,7)-TempY(1,7);

        TT = sqrt(2*(26*3)^2/(1+(d1/d3)^2-2*(d1/d2)^2));

    XDist = TempX(:,end)-TempX(:,1);
    YDist = TempY(end,:)-TempY(1,:);
    
    Da = (40*4)/mean((YDist));
%         figure(1);hold on;
%         plot(imagePoints(:,1),imagePoints(:,2),'ro')
    p1 = polyfit(TempX(1,:),TempY(1,:),1);
p2 = polyfit(TempX(5,:),TempY(5,:),1);
Dc=(40*4)/((polyval(p2,960)-polyval(p1,960)));

        mid_x_prev = mid_x;
        mid_y_prev = mid_y;
        i_x_max = max(imagePoints(:,1));
        i_x_min = min(imagePoints(:,1));
        i_y_max = max(imagePoints(:,2));
        i_y_min = min(imagePoints(:,2));
        mid_x = (i_x_max+i_x_min)/2;
        mid_y = (i_y_max+i_y_min)/2;
        imag_x_s = max(1,round(mid_x-(i_x_max-i_x_min)*4));
        imag_x_e = min(size(img,2),round(mid_x+(i_x_max-i_x_min)*4));
        imag_y_s = max(1,round(mid_y-(i_y_max-i_y_min)*4));
        imag_y_e = min(size(img,1),round(mid_y+(i_y_max-i_y_min)*4));

        [msg]=readBarcode(img(imag_y_s:imag_y_e,imag_x_s:imag_x_e,:));

%         if length(RefPos)==2
            RefPos = str2num(msg);
            if length(RefPos)~=2
                RefPos = [0 0];
            end
%         else
%             RefPos = 0;
%         end

        if boardSize == [6,8]
            if (img(round((imagePoints(1,2)+imagePoints(7,2))/2),round((imagePoints(1,1)+imagePoints(7,1))/2),1))>(img(round((imagePoints(3,2)+imagePoints(9,2))/2),round((imagePoints(3,1)+imagePoints(9,1))/2),1))
                sig = 1;
            else
                sig = 1;
            end

        end
        tform = fitgeotform2d(imagePoints,imagePointsRef,'projective');
        figure(1);hold on;
        plot(imagePoints(:,1),imagePoints(:,2),'ro')
        %     image(RefChecker)
        Db_t = tform.A*[960 540 1]';
        Db = Db_t(2)/1000;

        CosC = (Da^2+Db^2-Dc^2)/(2*Da*Db);
        x = CosC*Da;
        y = sqrt(Da^2-x^2);
        pos = [x y]'
%         pos = sig*(tform.A*[960 540 1]')+[RefPos 0]';
%         figure(2);hold on;plot(pos(1),pos(2),'o');axis equal
%         if (length(msg)==0)
            
%         pos = sig*(tform.A*[0 540 1]'-[0 280 0]'+[-130 630 0]')+[RefPos 0]'

    end
    %k = k + 1;


end

