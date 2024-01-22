function DistMapT = findMinDist(DistMap,DistNum)

[L,M,N] = size(DistMap);
DistMapT = zeros(L,M,N);

DistMapT(:,:,2) = DistMap(:,:,2);

DistMapTt = DistMap(:,:,1);
DistMapTt = DistMapTt(:);
SortedDist = sort(DistMapTt);
SortedDist(SortedDist==0) = [];
if length(SortedDist) < DistNum
    SortedDist = [SortedDist; zeros(DistNum-length(SortedDist),1)];
end

for Li = 1 : L 
    for Mi = 1 : M  
        for kk = 1 : DistNum
            if DistMap(Li,Mi,1)==SortedDist(kk)
                DistMapT(Li,Mi,1) = SortedDist(kk);
            end
        end
    end
end



