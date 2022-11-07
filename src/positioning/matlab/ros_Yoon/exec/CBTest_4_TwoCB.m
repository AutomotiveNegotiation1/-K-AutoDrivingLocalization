clear all;
clc
warning off

load('cali_params.mat','cameraParams');
%cam = webcam('Logitech StreamCam');

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
for k = 100 : 199
    %im = (snapshot(cam));
    %     figure(100);imshow(im);
    im = imread(sprintf('test_9_30/%d.jpg',k));

    [img, newOrigin] = undistortImage(im, cameraParams, OutputView = "full");

    figure(1); hold off
    image(img);
    %     imagePoints_p{1} = double(zeros(15,2));
    %     imagePoints_p{2} = double(zeros(15,2));
    %     imagePoints_p{3} = double(zeros(15,2));
    camIntrinsics = cameraParams.Intrinsics;
    tic
    % pos = zeros(3,2);
    %     parfor l = 1 : 3
    %         [imagePoints_t,boardSize_t] = detectCheckerboardPoints(img((l-1)*270+1:min(1920,(l-1)*270+540),:,:));
    %         if size(imagePoints_t,1)==size(imagePointsRef,1)&&(sum(sum(isnan(imagePoints_t)))==0)
    %
    %         imagePoints_t = imagePoints_t + newOrigin + [0 (l-1)*270];
    %
    %     % extrinsic 파라미터
    %         camExtrinsics = estimateExtrinsics(imagePoints_t, worldPoints, camIntrinsics);
    %         pos(l,:) = [camExtrinsics.Translation(1);camExtrinsics.Translation(3)];
    %         else
    %         pos(l,:) = NaN;
    %         end
    %     end

    [imagePoints,boardSize] = detectCheckerboardPoints(im(271:810,:,:));

    %     corners = detectHarrisFeatures(rgb2gray(img);
    if size(imagePoints,1)==size(imagePointsRef,1)&&(sum(sum(isnan(imagePoints)))==0)
        imagePoints = imagePoints + newOrigin;

        img_t = img;
        camIntrinsics = cameraParams.Intrinsics;

        % extrinsic 파라미터
        camExtrinsics = estimateExtrinsics(imagePoints, worldPoints, camIntrinsics);
        pos(k-99,:) = [camExtrinsics.Translation(1) camExtrinsics.Translation(3)];
        toc
        %     figure(2);hold on;plot(x,y,'r.');axis equal

    end
    %k = k + 1;


end

