function [Nav, Ani] = navi_uwb_update(Nav, UWB, Ani)

    anchor = UWB.anchor(UWB.activeIndex,:);

    P = Nav.KF.P(1:15,1:15);

    p_ned = Nav.output.pos;
    v_ned = Nav.output.vel;
    x_att = Nav.output.att;
    
    C_b2n = Nav.output.C_b2n;
    C_n2b = Nav.output.C_n2b;    
    
    b_acel = Nav.imu_bias.acel;
    b_gyro = Nav.imu_bias.gyro;
       

    zhat_tmp = zeros(1,UWB.num);
    for i=1:UWB.num
        zhat_tmp(i) = norm(anchor(i,2:4) - p_ned');
    end
    zhat = zhat_tmp';


    los = zeros(UWB.num,3);
    for i=1:UWB.num
        los(i,:) = (anchor(i,2:4) - p_ned') / norm(anchor(i,2:4) - p_ned');
    end
    
    H = zeros(UWB.num,15);
    H(:,1:3) = -los;
    
    R = Nav.KF.R_UWB(1:UWB.num,1:UWB.num);
    Z = UWB.dist(UWB.activeIndex) - zhat;
    
%     %%% Altitude
%     H(end+1,3) = 1;
%     R(end+1,end+1) = 0.01;
%     Z(end+1) = 0 - p_ned(3);
    
    
    K = P*H'/(H*P*H'+R);
    P = P - K*H*P;                
    err_X = zeros(15,1) + K*Z;
    
    
    p_ned = p_ned + err_X(1:3);
    v_ned = v_ned + err_X(4:6);
    x_att = Qua_Update(ang2qua(err_X(7:9)),x_att);
    
    
    Nav.output.pos = p_ned;
    Nav.output.vel = v_ned;    
    Nav.output.att = x_att;
    
    Nav.output.C_b2n = qua2dcm(Nav.output.att);
    Nav.output.C_n2b = Nav.output.C_b2n';
    
    b_acel = b_acel + err_X(10:12);
    Nav.imu_bias.acel = b_acel;
    
    b_gyro = b_gyro + err_X(13:15);
    Nav.imu_bias.gyro = b_gyro;
            
    Nav.KF.P(1:15,1:15) = P;
    

    if (Ani.play) && (0)

        p_att = dcm2eulr(qua2dcm(Nav.output.att));
        if extractBefore(extractAfter(Nav.fileName,'dat_'),'_sensor')=='220222'
            axis([-10 10 -5 35]); 
        else
            axis([-15 10 -6 16]); 
        end
        view(2); title(num2str(Ani.cnt));
        a = plot3(p_ned(1), p_ned(2), p_ned(3),'g.','MarkerSize',2);
        a0= quiver(p_ned(1), p_ned(2), 1*sin(p_att(3)), 1*cos(p_att(3)), 'g', 'LineWidth',1.5,'MaxHeadSize',5);
        drawnow();
        
        
        if (Ani.save)
            Ani.M(Ani.cnt) = getframe(gcf);
            Ani.cnt = Ani.cnt + 1;
        end 
        delete(a0);
    end

end