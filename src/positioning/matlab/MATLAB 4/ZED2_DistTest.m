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

intrinsicsy = intrinsicsx;

AA = imrotate(checkerboard(67,3,2),90);
[OrigPoints] = detectMinEigenFeatures(AA,'MinQuality', 0.1);
[featuresOriginal,validPointsOrig,ptVisOrig] = extractHOGFeatures(AA,OrigPoints);

% figure(87);imshow(AA)

worldPoints = generateCheckerboardPoints([4 6], 67);
worldPoints = worldPoints - worldPoints(8,:);
% Start loop
while ok
    % Capture the current image
        tic
    img = snapshot(zed);
    
    image_right = rgb2gray(img(:, width/2 +1: width, :));
    image_left = rgb2gray(img(:, 1: width/2, :));

    points_left = detectSIFTFeatures(image_left);
    points_right = detectSIFTFeatures(image_right);
    
    [Features_left, points_left] = extractFeatures(image_right, points_left);
    [Features_right, points_right] = extractFeatures(image_left, points_right);

    boxPairs = matchFeatures(Features_left, Features_right);

    matchedLeftPoints = points_left(boxPairs(:, 1), :);
    matchedRightPoints = points_right(boxPairs(:, 2), :);
    
    figure(1);
    showMatchedFeatures(image_right, image_left, matchedLeftPoints, ...
    matchedRightPoints, 'montage');
    title('Putatively Matched Points (Including Outliers)');
    if (size(matchedRightPoints,1)>20) && (size(matchedLeftPoints,1)>20)
    [tform, inlierIdx] = estgeotform2d(matchedLeftPoints, matchedRightPoints, 'rigid');
    inlierLeftPoints   = matchedLeftPoints(inlierIdx, :);
    inlierRightPoints = matchedRightPoints(inlierIdx, :);

    

    figure(2);
    showMatchedFeatures(image_right, image_left, inlierLeftPoints, ...
    inlierRightPoints, 'montage');
    title('Matched Points (Inliers Only)');

    end
end

clear cam