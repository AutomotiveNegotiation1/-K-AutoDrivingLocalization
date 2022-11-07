function [imagePointsO_N] = refine_imagePoints_2(image_left, imagePointsO, boardSize)
[M,N]= size(image_left);

PointsX = reshape(imagePointsO(:,1),boardSize-1);
PointsY = reshape(imagePointsO(:,2),boardSize-1);
PointsXE = [PointsX(:,1)-(PointsX(:,2)-PointsX(:,1)) PointsX PointsX(:,end)+(PointsX(:,2)-PointsX(:,1))];
PointsXE = [PointsXE(1,:)-(PointsXE(2,:)-PointsXE(1,:)) ; PointsXE; PointsXE(end,:)+(PointsXE(end,:)-PointsXE(end-1,:))];

PointsYE = [PointsY(:,1)-(PointsY(:,2)-PointsY(:,1)) PointsY PointsY(:,end)+(PointsY(:,2)-PointsY(:,1))];
PointsYE = [PointsYE(1,:)-(PointsYE(2,:)-PointsYE(1,:)) ; PointsYE; PointsYE(end,:)+(PointsYE(end,:)-PointsYE(end-1,:))];

for ly = 1 : size(PointsXE,1)-1
    for lx = 1 : size(PointsXE,2)-1
        AveBoxColor(ly,lx) = mean(mean(image_left(min([PointsYE(ly,lx),PointsYE(ly,lx+1),PointsYE(ly+1,lx),PointsYE(ly+1,lx+1)]):max([PointsYE(ly,lx),PointsYE(ly,lx+1),PointsYE(ly+1,lx),PointsYE(ly+1,lx+1)]),min([PointsXE(ly,lx),PointsXE(ly,lx+1),PointsXE(ly+1,lx),PointsXE(ly+1,lx+1)]):max([PointsXE(ly,lx),PointsXE(ly,lx+1),PointsXE(ly+1,lx),PointsXE(ly+1,lx+1)]))));
    end
end

for ky = 1 : size(PointsX,1)
    pointVec = [];
    for lx = 1: size(PointsX,2)-1
        meanV = (AveBoxColor(ky,lx+1)+AveBoxColor(ky+1,lx+1))/2;
        for dx = max(1,round(PointsX(ky,lx))+3):min(N,round(PointsX(ky,lx+1))-3)
            for dy = floor(min(PointsY(ky,lx),PointsY(ky,lx+1)))-5:ceil(max(PointsY(ky,lx),PointsY(ky,lx+1)))+5
                if (double(image_left(dy,dx))-meanV)*(double(image_left(dy+1,dx))-meanV)<=0
                    frac = (meanV-double(image_left(dy,dx)))/(double(image_left(dy+1,dx))-double(image_left(dy,dx)));
                    pointVec = [pointVec [dx;dy+frac]];
                    break;
                end
            end
        end
    end
    polylineX(ky,:) = polyfit(pointVec(1,:), pointVec(2,:), 1);
    
end

for ky = 1 : size(PointsX,2)
    pointVec = [];
    for lx = 1: size(PointsX,1)+1
        meanV = (AveBoxColor(lx,ky)+AveBoxColor(lx,ky+1))/2;
        for dx = max(1,round(PointsYE(lx,ky+1))+3):min(M,round(PointsYE(lx+1,ky+1))-3)
            for dy = floor(min(PointsXE(lx,ky+1),PointsXE(lx+1,ky+1)))-5:ceil(max(PointsXE(lx,ky+1),PointsXE(lx+1,ky+1)))+5
                if (double(image_left(dx,dy))-meanV)*(double(image_left(dx,dy+1))-meanV)<=0
                    frac = (meanV-double(image_left(dx,dy)))/(double(image_left(dx,dy+1))-double(image_left(dx,dy)));
                    pointVec = [pointVec [dy+frac;dx]];
                    break;
                end
            end
        end
    end
    polylineY(ky,:) = polyfit(pointVec(1,:), pointVec(2,:), 1);
    
%     figure(12);hold off;imshow(image_left);hold on;pt=cornerPoints(pointVec');plot([1:1920],polyval(polylineY(ky,:),[1:1920]),'r')
end

% figure(10);hold off;imshow(image_left);
% for kk = 1 : size(polylineX,1)
%     hold on;plot([1:1920],polyval(polylineX(kk,:),[1:1920]),'r');
% end
% for kk = 1 : size(polylineY,1)
%     hold on;plot([1:1920],polyval(polylineY(kk,:),[1:1920]),'r');
% end

 
for lx = 1 : size(PointsX,2)
    for ly = 1 : size(PointsX,1)
        a = polylineY(lx,1);
        b = polylineY(lx,2);
        c = polylineX(ly,1);
        d = polylineX(ly,2);
        imagePointsO_N((ly-1)+(lx-1)*(boardSize(1)-1)+1,:) = [(d-b)/(a-c) a*(d-b)/(a-c)+b];
       
    end
end

