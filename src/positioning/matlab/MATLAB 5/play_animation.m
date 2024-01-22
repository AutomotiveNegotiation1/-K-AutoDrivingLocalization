function [Ani] = play_animation ( Ani, Nav, LiDAR, UWB, Map, idx)


    att = dcm2eulr(qua2dcm(Nav.output.att));
    pos = Nav.output.pos;
    
    P = diag(Nav.KF.P(1:2,1:2));
    MapDownsampled = pcdownsample(pcdenoise(Map),'gridAverage',0.5);
    [indices, ~] = findNeighborsInRadius(MapDownsampled, p_ned', 15);
    fixed = select(MapDownsampled, indices);


    movingDownsampled = pcdownsample(pcdenoise(LiDAR.pt),'gridAverage',0.5);

    [indices1, ~] = findNeighborsInRadius(movingDownsampled, [0,0,0], 2);
    ptCnt = 1:movingDownsampled.Count;
    out_indices = uint32(setdiff(ptCnt,indices1));
    ptTmp = select(movingDownsampled, out_indices);

    [indices2, ~] = findNeighborsInRadius(ptTmp, [0,0,0], 10);
    LiDAR.ptSub = select(ptTmp, indices2);

    moving = pctransform(LiDAR.ptSub, rigid3d(C_b2n, p_ned'));


%     plot3()

    a1= quiver(pos(1), -pos(2), r*cos(att(3)), r*-sin(att(3)), 'k', 'LineWidth',1.5,'MaxHeadSize',2);
    if Nav.mode == 3
        b1 = plot(UWB.anchor(:,3),UWB.anchor(:,2),'ro','LineWidth',2);
    end

    n=1;
    point = zeros(1,2);
    for t=1:length(LiDAR.rho)
        if (LiDAR.rho(t) > 1) && (LiDAR.rho(t) <= 10)
            angle = -LiDAR.theta(t) + att(3);

            point(n,1) = LiDAR.rho(t) * cos(angle);
            point(n,2) = LiDAR.rho(t) * sin(angle);
            n=n+1;
        end
    end    
    a3= plot(pos(1)+point(:,1), -pos(2)-point(:,2), 'r.','MarkerSize',4);

    if Nav.mode == 1
        strr = 'INS/GPS';
    elseif Nav.mode == 2
        strr = 'INS/LiDAR';
    elseif Nav.mode == 3
        strr = 'INS/UWB';
    else
        strr = 'INS/LiDAR/UWB';
    end
    title([strr,'  index= ',num2str(idx)]);
    drawnow;
    
    if Ani.save == 1
        Ani.M(Ani.cnt) = getframe(gcf);
        Ani.cnt = Ani.cnt + 1;
    end 
    
    delete(r1);
    delete(a1);
    if Nav.mode < 6
        delete(a2);
    end
    delete(a3)
    delete(b1)

    

end
