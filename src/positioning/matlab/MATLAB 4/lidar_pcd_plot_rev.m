function lidar_pcd_plot_rev ( Nav, LiDAR, Map )

    att = dcm2eulr(qua2dcm(Nav.output.att));

    Find_radius = 12;
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


    [tmpIdx, ~] = rangesearch(Nav.output.pos(1:2)', Map.Location(:,1:2), Find_radius);
    idx = find(not(cellfun('isempty',tmpIdx)));
    subMap = Map.Location(idx,:);

%     figure;
    grid on; axis equal; hold on;
    a3= plot(subMap(:,1), subMap(:,2), 'k.', 'MarkerSize',4);
    a1= plot(Nav.output.pos(1), Nav.output.pos(2), 'bd');
    xlabel('X'); ylabel('Y'); 
    a2= plot(Nav.output.pos(1)+point(:,1), Nav.output.pos(2)+point(:,2), 'r.','MarkerSize',4);
    
    
    drawnow;
    
    delete(a1);
    delete(a2);
    delete(a3);
end
