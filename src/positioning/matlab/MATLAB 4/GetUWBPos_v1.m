function [tag_pos_est, heading_est] = GetUWBPos_v1(xa, ya, dist, angles_from_heading)

[LN, LP] = size(dist);
Li = 1;
    for n = 1 : LN
        for m = 1 : LN
            for p = 1 : LP
                for q = 1 : LP
                    if (n~=m && p ~=q)

%                         if (n==1 && (p == 1 || q == 1 || p == 2 || q == 2) ) || (m==1 && (p == 1 || q == 1 || p == 2 || q == 2) )
                        if dist(n,p)==0 || dist(m,p)==0 || dist(n,q)==0 || dist(m,q)==0

                        else
                            A(Li,2*(p-1)+1:2*p) = [2*(xa(n)-xa(m)) 2*(ya(n)-ya(m))];
                            A(Li,2*(q-1)+1:2*q) = [2*(xa(n)-xa(m)) 2*(ya(n)-ya(m))];
                            Y(Li,1) = dist(n,p)^2+dist(n,q)^2-dist(m,p)^2-dist(m,q)^2-2*xa(n)^2+2*xa(m)^2-2*ya(n)^2+2*ya(m)^2;
%                             Y(Li,1) = circle_dist(n,p,r)^2+circle_dist(n,q,r)^2-circle_dist(m,p,r)^2-circle_dist(m,q,r)^2-2*xa(n)^2+2*xa(m)^2-2*ya(n)^2+2*ya(m)^2;
                            Li = Li + 1;
                        end
                    end
                end
            end
        end
    end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    %%%% Calc. Tag position %%%%%%%
    Res = inv(A'*A)*A'*Y;
    Xt_e = -Res(1:2:end);
    Yt_e = -Res(2:2:end);
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    %%% Estimated Center Position %
    %%% Should be fixed!!
    Xt_c_e = mean(Xt_e);
    Yt_c_e = mean(Yt_e);
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    %%% Estimated Heading %%%%%%%%%
    %%
    tag_pos_est = Xt_e' + j*Yt_e';
    heading_est = angle(mean(tag_pos_est([2 4])./tag_pos_est([1 3])));

%     tag_arrow_est = sum((tag_pos_est-Xt_c_e-j*Yt_c_e).*exp(-j*angles_from_heading));
%     heading_est = atan2(imag(tag_arrow_est),real(tag_arrow_est));