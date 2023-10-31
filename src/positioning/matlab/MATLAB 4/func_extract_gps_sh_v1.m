function [GPS] = func_extract_gps_sh_v1(Sens, i)

    n = Sens.flag(i,2);

    GPS.time = Sens.gps.time(n);

    GPS.pos_ned = Sens.gps.pos_ned(n,:)';
%     tmp_pos_ned = Sens.gps.pos_ned(n,:)';
%     GPS.pos_ned(1,1) = -tmp_pos_ned(1);
%     GPS.pos_ned(2,1) = -tmp_pos_ned(2);
%     GPS.pos_ned(3,1) = tmp_pos_ned(3);

    GPS.vel_ned = Sens.gps.vel_ned(n,:)';
%     tmp_vel_ned = Sens.gps.vel_ned(n,:)';
%     GPS.vel_ned(1,1) = -tmp_vel_ned(1);
%     GPS.vel_ned(2,1) = -tmp_vel_ned(2);
%     GPS.vel_ned(3,1) = tmp_vel_ned(3);

    
    GPS.index = n;
    GPS.flag = true;

        
end