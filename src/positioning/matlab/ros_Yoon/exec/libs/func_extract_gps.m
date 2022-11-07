function [GPS] = func_extract_gps(Sens, i)

    n = Sens.flag(i,2);

    GPS.time = Sens.gps.time(n);

    GPS.pos_ned = Sens.gps.pos_ned(n,:)';
    GPS.vel_ned = Sens.gps.vel_ned(n,:)';

    
    GPS.index = n;
    GPS.flag = true;

        
end