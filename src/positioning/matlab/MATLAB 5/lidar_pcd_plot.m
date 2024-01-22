function lidar_pcd_plot ( Nav, LiDAR )

    att = dcm2eulr(qua2dcm(Nav.output.att));

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
%     point(:,2) = point(:,2) *-1;

%     figure;
    grid on; axis equal; hold on;
    plot(Nav.output.pos(1), Nav.output.pos(2), 'bd');
    xlabel('X'); ylabel('Y'); 
    plot(Nav.output.pos(1)+point(:,1), Nav.output.pos(2)+point(:,2), 'r.','MarkerSize',4);

    drawnow;
end
