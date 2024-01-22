function [CurrILocation, CurrQLocation, inlierCurrIPoints, inlierCurrQPoints, inlierCurrIFeatureIndex, inlierCurrQFeatureIndex] = MatchPointsLocation(currI, currIFeatures, currQFeatures, currIPoints, currQPoints)

indexPairs = matchFeatures(currIFeatures, currQFeatures, Unique=true, MaxRatio=0.9, MatchThreshold=40);

currIMatchedPoints  = currIPoints(indexPairs(:,1),:);
currQMatchedPoints = currQPoints(indexPairs(:,2),:);

% If not enough matches are found, check the next frame
minMatches = 100;
if size(indexPairs, 1) < minMatches
    CurrILocation = [];
    CurrQLocation = [];
else


    %     currIMatchedPoints  = currIPoints(indexPairs(:,1),:);
    %     currQMatchedPoints  = currQPoints(indexPairs(:,2),:);
    %
    % Compute homography and evaluate reconstruction
    [tformH, scoreH, inliersIdxH] = helperComputeHomography(currIMatchedPoints, currQMatchedPoints);

    % Compute fundamental matrix and evaluate reconstruction
    [tformF, scoreF, inliersIdxF] = helperComputeFundamentalMatrix(currIMatchedPoints, currQMatchedPoints);


    ratio = scoreH/(scoreH + scoreF);
    ratioThreshold = 0.45;
    if ratio > ratioThreshold
        inlierTformIdx = inliersIdxH;
        tform          = tformH;
    else
        inlierTformIdx = inliersIdxF;
        tform          = tformF;
    end

    % Computes the camera location up to scale. Use half of the
    % points to reduce computation
    inlierCurrQPoints = currQMatchedPoints(inlierTformIdx);
    inlierCurrIPoints = currIMatchedPoints(inlierTformIdx);
    inlierCurrIFeatureIndex = indexPairs(inlierTformIdx,1);
    inlierCurrQFeatureIndex = indexPairs(inlierTformIdx,2);
    

    LI = size(inlierCurrIPoints.Location,1);
    LQ = size(inlierCurrQPoints.Location,1);

    [M,N,L] = size(currI);
    for iL = 1 : min(LI,LQ)
        [distI(iL),distQ(iL),angI(iL),angQ(iL)] = calc_stereo_dist(M,N,105*pi/180,0.12, inlierCurrQPoints.Location(iL,:),inlierCurrIPoints.Location(iL,:));
    end

    CurrILocation = [distI.*cos(angI) ;distI.*sin(angI)];
    CurrQLocation = [distQ.*cos(angQ) ;distQ.*sin(angQ)];
end