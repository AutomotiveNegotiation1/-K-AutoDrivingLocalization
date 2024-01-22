clear all;

xr = [10 10 -10 -10];
yr = [10 -10 -10 10];

xt = [1.8 2.2 2.2 1.8];
yt = [1.8 1.8 2.2 2.2];
xt_c = mean(xt);
yt_c = mean(yt);

for n = 1 : 4
    for p = 1 : 4
        dist_o(n,p) = sqrt((xr(n)-xt(p))^2+(yr(n)-yt(p))^2);
    end
end

dist = awgn(dist_o,20);


%%%%%%%%%%%%%% Generate Matrix %%%%%%%%%%%%%%%%%%%
Li = 1;
for n = 1 : 4
	for m = 1 : 4
		for p = 1 : 4
			for q = 1 : 4
				if (n~=m && p ~=q) 
					A(Li,2*(p-1)+1:2*p) = [2*(xr(n)-xr(m)) 2*(yr(n)-yr(m))];
					A(Li,2*(q-1)+1:2*q) = [2*(xr(n)-xr(m)) 2*(yr(n)-yr(m))];
					Y(Li,1) = dist(n,p)^2+dist(n,q)^2-dist(m,p)^2-dist(m,q)^2-2*xr(n)^2+2*xr(m)^2-2*yr(n)^2+2*yr(m)^2;
					Li = Li + 1;
				end
			end
		end
	end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%% Calc. Tag position
Res = inv(A'*A)*A'*Y;
Xt_e = -Res(1:2:end);
Yt_e = -Res(2:2:end);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%% Estimated Center Position
Xt_c_e = mean(Xt_e);
Yt_c_e = mean(Yt_e);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%% Estimated Heading
%% Should be defined
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure(1);
hold off
plot(xt(1),yt(1),'bo');
hold on;
plot(xt(2),yt(2),'b*');
plot(xt(3),yt(3),'bv');
plot(xt(4),yt(4),'b^');

plot(Xt_e(1),Yt_e(1),'ro');
plot(Xt_e(2),Yt_e(2),'r*');
plot(Xt_e(3),Yt_e(3),'rv');
plot(Xt_e(4),Yt_e(4),'r^');
title('Original(Blue) and estimated(Red) Tag position')

figure(2);
hold off;
plot(dist_o(:),'b-')
hold on;
plot(dist(:),'r-')
title('Original(Blue) and estimated(Red) Tag to Anchor distance')

dist_error = sqrt(mean((dist_o(:)-dist(:)).^2))

xt_c
yt_c
Xt_c_e
Yt_c_e

