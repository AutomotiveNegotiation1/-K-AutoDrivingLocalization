function [Xc, Yc] = TrilateralPos(xat, yat, dists)

N = length(xat);

A = zeros(N*(N-1)/2,2);
Y = zeros(N*(N-1)/2,1);

Li = 1;
for n = 1 : N
    for m = n+1 : N
        A(Li,1:2) = [2*(xat(n)-xat(m)) 2*(yat(n)-yat(m))];
        Y(Li,1) = dists(n)^2-dists(m)^2-xat(n)^2+xat(m)^2-yat(n)^2+yat(m)^2;
        Li = Li + 1;
    end
end

%%%% Calc. Tag position %%%%%%%
Res = inv(A'*A)*A'*Y;
Xc = -Res(1);
Yc = -Res(2);