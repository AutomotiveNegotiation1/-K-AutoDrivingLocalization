function [LiDAR] = func_extract_range(Sens, i)
    
    n = Sens.flag(i,3);

    LiDAR.time = Sens.lidar.time(n);

    LiDAR.pt = Sens.lidar.pt(n);
    
    LiDAR.index = n;
    
    LiDAR.flag = true;

    LiDAR.map_radius_max = 15; %15;
    LiDAR.pt_radius_min = 1.5;
    LiDAR.pt_radius_max = 5; %5;
end