function [currPos] = CalcPos(prevPos, currI, currIFeatures, currQFeatures, currIPoints, currQPoints)

    [CurrILocation{currFrameIdx}, CurrQLocation{currFrameIdx},  inlierCurrIPoints{currFrameIdx}, inlierCurrQPoints{currFrameIdx}, inlierCurrIFeatureIndex{currFrameIdx}, inlierCurrQFeatureIndex{currFrameIdx}] = MatchPointsLocation(currI,currIFeatures, currQFeatures, currIPoints, currQPoints);


    indexPairs = matchFeatures(preIFeatures, currIFeatures, Unique=true, MaxRatio=0.9, MatchThreshold=40);


    currpreIMatchedPoints = preIPoints(indexPairs(:,1),:);
    currcurrMatchedPoints = currIPoints(indexPairs(:,2),:);
    
    % Compute homography and evaluate reconstruction
    [tformH, scoreH, inliersIdxH] = helperComputeHomography(currpreIMatchedPoints, currcurrMatchedPoints);

    % Compute fundamental matrix and evaluate reconstruction
    [tformF, scoreF, inliersIdxF] = helperComputeFundamentalMatrix(currpreIMatchedPoints, currcurrMatchedPoints);


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
%     inlierCurrQPoints = currcurrMatchedPoints(inlierTformIdx);
%     inlierCurrIPoints = currpreIMatchedPoints(inlierTformIdx);
    inlierPreFeatureIndex = indexPairs(inlierTformIdx,1);
    inlierCurrFeatureIndex = indexPairs(inlierTformIdx,2);
    

    r = 1;
    inlierPrePrePoints = [];
    inlierPPoints = [];
    inlierCurrPrePoints = [];
    inlierCPoints = [];
    for k = 1 : length(inlierCurrIFeatureIndex{currFrameIdx-1})
        [iv] = find(inlierPreFeatureIndex == inlierCurrIFeatureIndex{currFrameIdx-1}(k));
        
        if length(iv) ~=0
            [qv] = find(inlierCurrIFeatureIndex{currFrameIdx} == inlierCurrFeatureIndex(iv));
            if length(qv) ~= 0
                inlierPrePrePoints(r) = iv;
                inlierPPoints(r) = k;
                inlierCurrPrePoints(r) = iv;
                inlierCPoints(r) = qv;
                r = r + 1;
            end
        end
    end
    

    if length(inlierPPoints)>5
    PreDist = CurrILocation{currFrameIdx-prevK}(:,inlierPPoints,:);
    CurrDist = CurrILocation{currFrameIdx}(:,inlierCPoints,:);

    PreDistC = PreDist(1,:)+j*PreDist(2,:);
    CurrDistC = CurrDist(1,:)+j*CurrDist(2,:);
    
    At = find(isinf(PreDistC)==1);
    PreDistC(At) = [];
    CurrDistC(At) = [];

    Bt = find(isinf(CurrDistC)==1);
    PreDistC(Bt) = [];
    CurrDistC(Bt) = [];

    AnPreDist = PreDistC(2:end)-PreDistC(1);
    AnCurrDist = CurrDistC(2:end)-CurrDistC(1);

    AngT = angle(mean(AnCurrDist.*conj(AnPreDist)));

    rotCurrDist = CurrDistC*exp(-j*AngT);
    Displacement = mean(-PreDistC+rotCurrDist);

    currPos = prevPos + Displacement


    figure(10);plot(Displacement*10,'.');axis([-10 10 -10 10])
    figure(11);imshow(currI)

    preIFeatures = currIFeatures;
    preIPoints = currIPoints;
    %inlierpreIFeatureIndex = inlierCurrIFeatureIndex;
    prevK = 1;
    else
        prevK = prevK + 1;
    end
    