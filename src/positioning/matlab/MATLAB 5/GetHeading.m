function [heading_new] = GetHeading(xa,ya,DistPredicted,tag_pos_b)
L = length(xa);
A = 0;
B = 0;
C = 0;

for l = 1 : L-1
    for m = l+1 : L
        for k = 1 : 3
            for q = k+1 : 4
                if (DistPredicted(l,k)~=0)&&(DistPredicted(l,q)~=0)&&(DistPredicted(m,k)~=0)&&(DistPredicted(m,q)~=0)
                    xl = xa(l);xm=xa(m);
                    yl = ya(l);ym=ya(m);
                    xbk = real(tag_pos_b(k));xbq = real(tag_pos_b(q));
                    ybk = imag(tag_pos_b(k));ybq = imag(tag_pos_b(q));
                    dlk = DistPredicted(l,k);
                    dlq = DistPredicted(l,q);
                    dmk = DistPredicted(m,k);
                    dmq = DistPredicted(m,q);
                    
                    A = (-2*(xl-xm)*(xbk-xbq)-2*(yl-ym)*(ybk-ybq));
                    B = (2*(xl-xm)*(ybk-ybq)-2*(yl-ym)*(xbk-xbq));
                    C = (dlk^2-dmk^2)-(dlq^2-dmq^2);
                end
            end
        end
    end
end

alp = acos(A/sqrt(A^2+B^2));
temp = acos(C/sqrt(A^2+B^2));

heading_new = temp + alp;
