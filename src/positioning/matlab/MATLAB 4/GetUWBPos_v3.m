function [tag_pos_est, heading_est] = GetUWBPos_v3(xa, ya, dist, tag_pos_b, tag_pos_est_2, heading_est_2)

[M,N] = size(dist);
diff = 100000;
if sum(tag_pos_est_2)==0
    init_pos = [10 10];
    init_heading = 0;
    alp = 1e-2;
else
    init_pos = [real(mean(tag_pos_est_2)) imag(mean(tag_pos_est_2))];
    init_heading = heading_est_2;
    alp = 2e-3;
end
e_sum = 0;
pos_prev = [0 0];
theta_prev = 0;
tag_pos_est = tag_pos_est_2;
heading_est = heading_est_2;
cnt = 1;
while (diff > 0.001 && cnt<10000)
    d_x = 0; d_y = 0; d_theta = 0;e_sum = 0;
    cnt = cnt + 1;
    for k = 1:M
        for l = 1:N
            if dist(k,l)~=0
                xc = init_pos(1); yc = init_pos(2); xtp = real(tag_pos_b(l)); ytp = imag(tag_pos_b(l));
                C_th = cos(init_heading); S_th = sin(init_heading);

                h = (xc + C_th*xtp-S_th*ytp - xa(k)); q = (yc + S_th*xtp+C_th*ytp - ya(k));
                g = h^2 + q^2 - dist(k,l)^2;

                e_sum = e_sum + g^2;
                d_x = d_x + 4*g*h;
                d_y = d_y + 4*g*q;
                d_theta = d_theta + 4*g*(h*(-S_th*xtp-C_th*ytp)+q*(C_th*xtp-S_th*ytp));
                
            end
        end
    end
    init_pos = init_pos - alp*e_sum*([d_x d_y]/sum(([d_x d_y d_theta]).^2));
    init_heading = init_heading - alp*e_sum*(d_theta/sum(([d_x d_y d_theta]).^2));
    diff = sqrt(sum(init_pos-pos_prev).^2 + ((init_heading-theta_prev))^2)/3;
%     diff = sqrt(e_sum);
    pos_prev = init_pos;
    theta_prev = init_heading;

end

tag_pos_est = init_pos(1)+j*init_pos(2) + exp(j*init_heading)*tag_pos_b;
heading_est = init_heading;



% dist_h = dist;
% dist(find(dist==0)) = 10000;
% sum_dist = sum(dist);
%
% for kk = 1 : length(tag_pos_b)
%     [val, ind] = sort(dist(:,kk));
%     [pos_t, Prob] = TwoAnchPos3([xa(ind(1)) xa(ind(2))], [ya(ind(1)) ya(ind(2))], [dist(ind(1),kk) dist(ind(2),kk)],[0 0],[0 0],xa(1:2)+j*ya(1:2),dist(:,1)');
%     pos_t_c(:,kk) = pos_t(:,1)+pos_t(:,2)*j;
% %     dist_er(:,kk) = abs(pos_t_c(:,kk) - (xa(ind(3))+j*ya(ind(3))))-dist(ind(3),kk);
% end
% for la = 1 : 2^length(tag_pos_b)
%     va = dec2bin(la-1,4);
%     l = 1;
%     for lb = 1 : 4
%         va_d = va(lb)-'0';
%         for lc = lb+1:4
%             va_e = va(lc)-'0';
%             aa(la,l) = (pos_t_c(va_d+1,lb)-pos_t_c(va_e+1,lc))/(tag_pos_b(lb)-tag_pos_b(lc));
%             l = l + 1;
%         end
%     end
%     bb(la) = abs(mean(abs(aa(la,:)))-1)^2;
%     cc(la) = mean((aa(la,:)));
%     dd(la) = abs(abs(cc(la))-1)^2;
%     ee(la) = bb(la)+dd(la);
% end
%
% [val,ind] = min(ee);
%
% va = dec2bin(ind-1,4);
% for dk = 1 : 4
%     tag_pos_est(dk) = pos_t_c(va(dk)-'0'+1,dk);
% end
% heading_est = angle(cc(ind));

% dist = dist_h;
%
%
% [LN, LP] = size(dist);
% Li = 1;
%     for n = 1 : LN
%         for m = n+1 : LN
%             for p = 1 : LP
%                 for q = p+1 : LP
%                     if (n~=m && p ~=q)
%
% %                         if (n==1 && (p == 1 || q == 1 || p == 2 || q == 2) ) || (m==1 && (p == 1 || q == 1 || p == 2 || q == 2) )
%                         if dist(n,p)==0 || dist(m,p)==0 || dist(n,q)==0 || dist(m,q)==0
%
%                         else
%                             A(Li,2*(p-1)+1:2*p) = [2*(xa(n)-xa(m)) 2*(ya(n)-ya(m))];
%                             A(Li,2*(q-1)+1:2*q) = [2*(xa(n)-xa(m)) 2*(ya(n)-ya(m))];
%                             Y(Li,1) = dist(n,p)^2+dist(n,q)^2-dist(m,p)^2-dist(m,q)^2-2*xa(n)^2+2*xa(m)^2-2*ya(n)^2+2*ya(m)^2;
% %                             Y(Li,1) = circle_dist(n,p,r)^2+circle_dist(n,q,r)^2-circle_dist(m,p,r)^2-circle_dist(m,q,r)^2-2*xa(n)^2+2*xa(m)^2-2*ya(n)^2+2*ya(m)^2;
%                             Li = Li + 1;
%                         end
%                     end
%                 end
%             end
%         end
%     end
%     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%     %%%% Calc. Tag position %%%%%%%
%     Res = pinv(A'*A)*A'*Y;
%     Xt_e = -Res(1:2:end);
%     Yt_e = -Res(2:2:end);
%     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%     %%% Estimated Center Position %
%     %%% Should be fixed!!
%     Xt_c_e = mean(Xt_e);
%     Yt_c_e = mean(Yt_e);
%     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%     %%% Estimated Heading %%%%%%%%%
%     %%
%     tag_pos_est = Xt_e' + j*Yt_e';
%     heading_est = angle(mean(tag_pos_est([2 4])./tag_pos_est([1 3])));
%
% %     tag_arrow_est = sum((tag_pos_est-Xt_c_e-j*Yt_c_e).*exp(-j*angles_from_heading));
% %     heading_est = atan2(imag(tag_arrow_est),real(tag_arrow_est));