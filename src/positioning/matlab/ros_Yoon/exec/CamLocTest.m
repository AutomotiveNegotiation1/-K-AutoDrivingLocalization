function pos = CamLocTest(cam, imagePointsRef)

k = 1;
mid_x = 0;
mid_y = 0;

% while 1
    img = rgb2gray(snapshot(cam));
%     figure(1); hold off
%     image(img);

    [imagePoints,boardSize] = detectCheckerboardPoints(img);
    img_t = img;
    while (size(imagePoints,1)>35)&&~(length(imagePoints)==0)
        img_t(min(imagePoints(:,2)):max(imagePoints(:,2)),min(imagePoints(:,1)):max(imagePoints(:,1)),:)= 0;
%         figure(2);hold off;imshow(img_t)
        [imagePoints,boardSize] = detectCheckerboardPoints(img_t);
        
    
    end
%         figure(1);hold on;
%         plot(imagePoints(:,1),imagePoints(:,2),'ro')

    if size(imagePoints,1)==size(imagePointsRef,1)&&(sum(sum(isnan(imagePoints)))==0)
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
                imagePointsT = imagePoints(2:end,:);
                sig = 1;
            else
                imagePointsT = imagePoints([1:4,6:end],:);
                sig = -1;
            end
        else
            %if img(round((imagePoints(2,:)+imagePoints(10,:))/2))>img(round((imagePoints(16,:)+imagePoints(24,:))/2))
            if (img(round((imagePoints(2,2)+imagePoints(10,2))/2),round((imagePoints(2,1)+imagePoints(10,1))/2),1))>(img(round((imagePoints(16,2)+imagePoints(24,2))/2),round((imagePoints(16,1)+imagePoints(24,1))/2),1))
                imagePointsT = imagePoints(2:end,:);
            else
                imagePointsT = imagePoints(1:end-1,:);
            end
        end
        tform = fitgeotform2d(imagePoints,imagePointsRef,'projective');
%         figure(1);hold on;
%         plot(imagePointsT(:,1),imagePointsT(:,2),'ro')
        %     image(RefChecker)
        pos = sig*(tform.A*[960 540 1]')+[RefPos 0]';
%         figure(2);hold on;plot(pos(1),pos(2),'o');axis equal
%         if (length(msg)==0)
            
%         pos = sig*(tform.A*[0 540 1]'-[0 280 0]'+[-130 630 0]')+[RefPos 0]'

    else
        pos = [0;0;0];
    end
    %k = k + 1;


% end
end