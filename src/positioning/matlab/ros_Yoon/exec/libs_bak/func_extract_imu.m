function [IMU] = func_extract_imu(Sens, n)

    if n > 1
        IMU.dt = Sens.imu.time(n) - Sens.imu.time(n-1);
    else
        IMU.dt = 0.01;
    end

    IMU.time = Sens.imu.time(n);
    IMU.gyro = Sens.imu.gyro(n,:)';
    IMU.acc = Sens.imu.acc(n,:)';
    
    IMU.SimulationTime = IMU.time - Sens.imu.time(1);
    
    if n > 1
        IMU.dt = Sens.imu.time(n) - Sens.imu.time(n-1);
    else
        IMU.dt = 0.01;   % 100Hz
    end
    
    IMU.index = n;
    IMU.cnt = n;
    
end