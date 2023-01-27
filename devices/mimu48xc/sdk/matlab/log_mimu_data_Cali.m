
% Clear workspace
clear all, close all, clc
format compact

% Open serial port
com = serial('COM83','InputBufferSize',100000);
fopen(com);

% Open binary file for saving inertial data
filename = 'mimu_data.bin';
file = fopen(filename, 'w');

% Flush serial ports
while com.BytesAvailable
    fread(com,com.BytesAvailable,'uint8');
end

op_rate_divider = 1; % 1: 1K samples per sec, 2: 500 samples per sec, 3: 250 samples per sec, 4: 125 samples per sec
                     % Maximum value 15
					 % 3 or higher, i.e. 250 samples per sec or lower, recommended for bluetooth transmission 
					 
command_normalimu = [64 op_rate_divider 0 65];
fwrite(com,command_normalimu,'uint8');
fread(com,4,'uint8');


% Open dummy figure with pushbutton such that logging can be aborted
abort = 0;
figure(10);
uicontrol('style','push','string','Abort data logging','callback','abort=1;');
drawnow

% Logg data until pushbutton pressed
while abort==0
    if com.BytesAvailable>0
        fwrite(file,fread(com,com.BytesAvailable,'uint8'),'uint8');
    end
    drawnow
end

% Stop output
fwrite(com,[34 0 34],'uint8');

% Close serial port and file
fclose(com);
fclose(file);
close(gcf)

% Parse data and delete logging file
[output_ax,output_ay,output_az,output_gx,output_gy,output_gz,time_stamps,raw_data] =int_fusion_parse_bin(filename);
delete(filename);

% Plot data

output_ax_double = double(output_ax);
output_ay_double = double(output_ay);
output_az_double = double(output_az);
output_gx_double = double(output_gx);
output_gy_double = double(output_gy);
output_gz_double = double(output_gz);

 scale_acc  = 1; % meter per second square
scale_gyro = 57.325; % degrees per second

figure(1),clf, hold on
   plot((output_ax_double)*scale_acc,'b-')
   plot((output_ay_double)*scale_acc,'g-')
   plot((output_az_double)*scale_acc,'r-')
grid on
title('Accelerometer readings');
xlabel('sample number')
ylabel('a [m/s^2]');
figure(2),clf, hold on
     plot((output_gx_double)*scale_gyro,'b-')
     plot((output_gy_double)*scale_gyro,'g-')
     plot((output_gz_double)*scale_gyro,'r-')
grid on
title('Gyroscope readings');
xlabel('sample number')
ylabel('\omega [deg/s]');
figure(3),clf, hold on
subplot(2,1,1);
plot(double(time_stamps)'/64e6,'b-');
grid on
title('Time stamps');
xlabel('sample number')
ylabel('[s]');
subplot(2,1,2);
dt = diff(double(time_stamps)');
for i=1:numel(dt)
    if dt(i)<0
        dt(i) = dt(i)+2^32;
    end
end
plot(dt/64e6,'b-');
grid on
title('Time differentials');
xlabel('sample number')
ylabel('[s]');
