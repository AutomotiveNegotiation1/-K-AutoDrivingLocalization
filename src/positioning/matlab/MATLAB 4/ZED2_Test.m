clear all;close all;clc;
warning off
% Open the ZED
zed = webcam('ZED')
% Set video resolution
zed.Resolution = zed.AvailableResolutions{1};
% Get image size
[height width channels] = size(snapshot(zed));

% Create Figure and wait for keyboard interrupt to quit
% f = figure('name','ZED camera','keypressfcn','close','windowstyle','modal');
ok = 1;
intrinsicsx = cameraIntrinsics(1030,[width/2 height/2],[width height]);
% intrinsicsx = cameraIntrinsics(1030,[540 960],[1080 1920]);

RefPosTable(18,:) = [0.03 10.2]*1000;
RefPosTable(3,:) = [-0.93 3.87]*1000;
RefPosTable(4,:) = [-1.32 2]*1000;

intrinsicsy = intrinsicsx;

AA = imrotate(checkerboard(67,3,2),90);
[OrigPoints] = detectMinEigenFeatures(AA,'MinQuality', 0.1);
[featuresOriginal,validPointsOrig,ptVisOrig] = extractHOGFeatures(AA,OrigPoints);

% figure(87);imshow(AA)

worldPoints = generateCheckerboardPoints([4 6], 67);
worldPoints = worldPoints - worldPoints(8,:);
% Start loop
figure(100);hold off; plot(RefPosTable(:,1),RefPosTable(:,2),'kd');
while ok
    % Capture the current image
        tic
    img = snapshot(zed);
    
    % Split the side by side image image into two images
%     image_left = rgb2gray(img(:, 1 : width/2, :));
    image_left = rgb2gray(img(:, width/2 +1: width, :));
%         image_right = rgb2gray(img(:, width/2 +1: width, :));

    image_Small = image_left(401:end-100,:);
    

    po = 1;
    pos = zeros(1,2);
    for dp = 2 :-1: 1
        kk = 2^dp;
        imaT = imresize(image_Small, round(size(image_Small)/kk));
        if dp ~= 0
            [imagePoints, boardSize] = detectCheckerboardPoints(imaT);
        else
            [imagePoints, boardSize] = detectCheckerboardPoints(imaT,'Highdistortion',true);
        end
        
        if (size(imagePoints,1)==15)
            imagePointsN = imagePoints*kk;
            MinX = min(imagePointsN(:,1));
            MinY = min(imagePointsN(:,2));
            MaxX = max(imagePointsN(:,1));
            MaxY = max(imagePointsN(:,2));
            AveBoxX = (MaxX - MinX )/(boardSize(2)-2);
            AveBoxY = (MaxY - MinY )/(boardSize(1)-2);
            
%             imagePointsO = detectCheckerboardPoints(image_Small(MinY-AveBoxY:MaxY+AveBoxY,MinX-AveBoxX:MaxX+AveBoxX));
%             imagePointsO = imagePointsO + [MinX-AveBoxX-1 MinY-AveBoxY-1] + [0 400];
            
            imagePointsO = imagePointsN+[0 400];
            CB_ID1 = getCheckerBoardID(image_left, imagePointsO, boardSize);
            imagePointsO = refine_imagePoints_2(image_left,imagePointsO, boardSize);
            
            [rotationMatrixx, translationVectorx] = extrinsics(imagePointsO, worldPoints, intrinsicsx);
%             cameraPose = extr2pose(camExtrinsicsOrig);
%             cameraLocation = cameraPose.Translation;

            Pos(po,:) = RefPosTable(CB_ID1,:)-[translationVectorx(1) translationVectorx(3)]
            figure(100);hold on;plot(Pos(po,1),Pos(po,2),'.');
            norm(RefPosTable(CB_ID1,:)-Pos(po,:))
            po = po + 1;
            
            
            pt = cornerPoints(imagePointsO);
            figure(100+kk);hold off;imshow(image_left);hold on;plot(pt);
            image_Small(MinY-AveBoxY:MaxY+AveBoxY,MinX-AveBoxX:MaxX+AveBoxX) = 0;
        end
    end
       toc 
%         [imagePoints, boardSize, pairUsed] = detectCheckerboardPoints(image_left(501:end,:,:),image_right(501:end,:,:));
    
%     if (size(imagePoints,1)~=0)
%         imagePointsx = imagePoints(:,:,1) + [0 540];
%         imagePointsy = imagePoints(:,:,2) + [0 540];
%         
%         CB_ID1 = getCheckerBoardID(image_left, imagePointsx, boardSize)
%         CB_ID2 = getCheckerBoardID(image_right, imagePointsy, boardSize);
%                 
%         if CB_ID1 == CB_ID2
%             RefPos = RefPosTable(CB_ID1,:);
%         end
%         [rotationMatrixx, translationVectorx] = extrinsics(imagePointsx, worldPoints, intrinsicsx);
%         [rotationMatrixy, translationVectory] = extrinsics(imagePointsy, worldPoints, intrinsicsy);
% 
%         posx = RefPos-[translationVectorx(1) translationVectorx(3)]
%         posy = RefPos-[translationVectory(1) translationVectory(3)]
%         
%         figure(100);hold on;plot(posx(1), posx(2), 'r.');
%         figure(100);hold on;plot(posy(1), posy(2), 'b.');axis equal
%         
%         
%         % Display the left and right images
%         figure(1);
%         subplot(1,2,1);
%         imshow(image_left);
%         hold on;plot(imagePointsx(:,1),imagePointsx(:,2)+540,'r+');
%         title('Image Left');
%         subplot(1,2,2);
%         imshow(image_right);
%         hold on;plot(imagePointsy(:,1),imagePointsy(:,2)+540,'r+');
%         title('Image Right');
%         drawnow;
    end
    % Check for interrupts
    %       ok = ishandle(f);


% close the camera instance
clear cam