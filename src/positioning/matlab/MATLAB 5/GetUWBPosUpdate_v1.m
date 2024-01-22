function [tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e] = GetUWBPosUpdate_v1(xa, ya, Xt_c_e, Yt_c_e, heading_est, rl, dist, angles_from_heading)
        ee = 1;
        Lj = 1;
        [LN, LP] = size(dist);
        %%%% Step 2
        for p = 1 : LP
            for n = 1 : LN
                for m = 1 : LN
                    if n~=m
                        if dist(n,p)==0 || dist(m,p)==0 
%                         if (n>100)
%                             ee = ee + 1;
                        else
                            B(Lj,1) = -2*(xa(n)-xa(m));
                            B(Lj,2) = -2*(ya(n)-ya(m));
                            B(Lj,3) = 2*(xa(n)-xa(m))*rl(p)*sin(heading_est+angles_from_heading(p))-2*(ya(n)-ya(m))*rl(p)*cos(heading_est+angles_from_heading(p));
                            W(Lj,1) = dist(n,p)^2-dist(m,p)^2-xa(n)^2+xa(m)^2+2*(rl(p)*cos(heading_est+angles_from_heading(p))+Xt_c_e)*(xa(n)-xa(m))  -ya(n)^2+ya(m)^2+2*(rl(p)*sin(heading_est+angles_from_heading(p))+Yt_c_e)*(ya(n)-ya(m));
                            Lj = Lj + 1;
                        end
                    end
                end
            end
        end
%         ee
        Refined = inv(B'*B)*B'*W;
        Xt_c_e = Xt_c_e + Refined(1);
        Yt_c_e = Yt_c_e + Refined(2);
        tag_center_pos_est = Xt_c_e + j*Yt_c_e;
        heading_est = heading_est + Refined(3);
