clear all;close all;clc;
warning off
% Open the ZED
cam = webcam('Logitech StreamCam')
% Set video resolution
cam.Resolution = cam.AvailableResolutions{1};
% Get image size
[height width channels] = size(snapshot(cam))

load('cali_params.mat','camIntrinsics');
boardSize = [4 6];
RefPosTable(18,:) = [0.03 10.2]*1000;
RefPosTable(3,:) = [-0.93 3.87]*1000;

worldPoints = generateCheckerboardPoints(boardSize, 67);
worldPoints = worldPoints - worldPoints(8,:);
% Start loop
figure(100);hold off; plot(RefPosTable(:,1),RefPosTable(:,2),'kd');
while 1
    % Capture the current image
    img = snapshot(cam);
    
    % Split the side by side image image into two images
    [imagePointsx, boardSizex] = detectCheckerboardPoints(img(541:end,:,:),'Highdistortion',true,'MinCornerMetric',0.10);
    if (size(imagePointsx,1)==(boardSize(1)-1)*(boardSize(2)-1))&&(size(imagePointsx,1)~=0)
        imagePointsx = imagePointsx + [0 540];
        CB_ID1 = getCheckerBoardID(image_left, imagePointsx, boardSizex)
        
        
        if CB_ID1 ~= [0 0]
            RefPos = RefPosTable(CB_ID1,:);
%             worldPointsR = worldPoints + RefPos;
        end
        [rotationMatrixx, translationVectorx] = extrinsics(imagePointsx, worldPoints, intrinsicsx);
        posx = RefPos-[translationVectorx(1) translationVectorx(3)]
        
        figure(100);hold on;plot(posx(1), posx(2), 'r.');
        
        
        % Display the left and right images
        figure(1);
        imshow(image_left);
        hold on;plot(imagePointsx(:,1),imagePointsx(:,2),'r+');
        drawnow;
    end
    % Check for interrupts
    %       ok = ishandle(f);
end

% close the camera instance
clear cam