function     [PosH] = UWBMultiTagPos(DistMap, NumTags, HeadingH, xa, ya, xt_b, yt_b)

L = length(xa);
ld = 1;

DistMap = findMinDist(DistMap,12);

if NumTags == 1
    PosH = 0;
else
    for kk = 1 : 4
        for ll = 1 : 4
            for pp = 1 : L
                for qq = 1 : L
                    if ((kk==ll) && (pp == qq)) ||((DistMap(kk,pp,1) == 0) || (DistMap(ll,qq,1) == 0)) 

                    else
                        A = xt_b(kk)*cos(HeadingH)-yt_b(kk)*sin(HeadingH);
                        B = xt_b(kk)*sin(HeadingH)+yt_b(kk)*cos(HeadingH);
                        C = xt_b(ll)*cos(HeadingH)-yt_b(ll)*sin(HeadingH);
                        D = xt_b(ll)*sin(HeadingH)+yt_b(ll)*cos(HeadingH);

                        Y(ld,1) = DistMap(kk,pp,1)^2-DistMap(ll,qq,1)^2-(xa(pp)^2-xa(qq)^2-2*xa(pp)*A+2*xa(qq)*C+A^2-C^2)-(ya(pp)^2-ya(qq)^2-2*ya(pp)*B+2*ya(qq)*D+B^2-D^2);
                        H(ld,1) = -2*xa(pp)+2*xa(qq)+2*(A-C);
                        H(ld,2) = -2*ya(pp)+2*ya(qq)+2*(B-D);

                        ld = ld+1;
                    end
                end
            end
        end
    end
    
    Te = transpose(H)*H;
    if det(Te) > 0.1
        X = inv(transpose(H)*H)*transpose(H)*Y;
        PosH = X(1)+j*X(2);
    else
        PosH = 0;
    end
end




