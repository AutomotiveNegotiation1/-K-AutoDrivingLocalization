function [Sens] = rosbag_parsing_sh_v1(fname)

    if strcmp(fname,'rosbag\line001_220304.mat')
        data = 'rosbag\line001_2022-03-04-17-22-26.bag';
    elseif strcmp(fname,'rosbag\line002_220304.mat')
        data = 'rosbag\line002_2022-03-04-17-26-30.bag';
    elseif strcmp(fname,'rosbag\stop001_220304.mat')
        data = 'rosbag\stop001_2022-03-04-17-35-53.bag';
    elseif strcmp(fname,'rosbag\stop002_220304.mat')
        data = 'rosbag\stop002_2022-03-04-17-58-05.bag';
    else
        data = fname; % inserted by Seonghyun Jang
    end
    bag = rosbag(data);


    %% IMU data
    raw_imu = select(bag,'Topic','/ublox/imu');
    imu_msg = readMessages(raw_imu,'DataFormat','struct');
    imu.time = cellfun(@(m) double(m.Header.Stamp.Sec),imu_msg) + cellfun(@(m) double(m.Header.Stamp.Nsec),imu_msg)/10^9;
    imu.gyro(:,1) = cellfun(@(m) double(m.AngularVelocity.X),imu_msg) *(pi/180);
    imu.gyro(:,2) = cellfun(@(m) double(m.AngularVelocity.Y),imu_msg) *(pi/180);
    imu.gyro(:,3) = cellfun(@(m) double(m.AngularVelocity.Z),imu_msg) *(pi/180);
    imu.acc(:,1) = cellfun(@(m) double(m.LinearAcceleration.X),imu_msg);
    imu.acc(:,2) = cellfun(@(m) double(m.LinearAcceleration.Y),imu_msg);
    imu.acc(:,3) = cellfun(@(m) double(m.LinearAcceleration.Z),imu_msg);
    clear raw_imu imu_msg


    %% GPS data
    raw_gps = select(bag,'Topic','/ublox/gnss_pvt');
    gps_msg = readMessages(raw_gps,'DataFormat','struct');
    gps_time = cellfun(@(m) double(m.Header.Stamp.Sec),gps_msg) + cellfun(@(m) double(m.Header.Stamp.Nsec),gps_msg)/10^9;
    gps_pos_llh(:,1) = cellfun(@(m) double(m.Lat),gps_msg) *10^-7;    % [deg]
    gps_pos_llh(:,2) = cellfun(@(m) double(m.Lon),gps_msg) *10^-7;    % [deg]
    gps_pos_llh(:,3) = cellfun(@(m) double(m.Height),gps_msg) /1000;  % [meter]

    gps_vel(:,1) = cellfun(@(m) double(m.VelN),gps_msg) /1000;  % North 
    gps_vel(:,2) = cellfun(@(m) double(m.VelE),gps_msg) /1000;  % East
    gps_vel(:,3) = cellfun(@(m) double(m.VelD),gps_msg) /1000;  % Down

    gps_pos_ned = zeros(1,3);
    for i=1:length(gps_pos_llh)
        gps_pos_ned(i,:) = lla2ned(gps_pos_llh(i,:), gps_pos_llh(1,:), 'ellipsoid');
    end

    gps.time = gps_time;
    gps.pos_ned = gps_pos_ned;
    gps.vel_ned = gps_vel;

    gps.numSV = cellfun(@(m) double(m.NumSV),gps_msg);
    gps.pDOP = cellfun(@(m) double(m.PDOP),gps_msg) *0.01;
    gps.fixType = cellfun(@(m) double(m.FixType),gps_msg);

    clear raw_gps gps_msg gps_time gps_vel gps_pos_llh gps_pos_ned i

    %% 3D LiDAR data
    raw_lidar = select(bag,'Topic','/velodyne_packets');
    lidar_msg = readMessages(raw_lidar,'DataFormat','struct');

    veloReader = velodyneROSMessageReader(lidar_msg,"VLP16");
    lidar.time = cellfun(@(m) double(m.Header.Stamp.Sec),lidar_msg) + cellfun(@(m) double(m.Header.Stamp.Nsec),lidar_msg)/10^9;
    frame_cnt=1;
    while(hasFrame(veloReader))
        ptCloudObj = readFrame(veloReader);
        lidar.pt(frame_cnt) = ptCloudObj;
        frame_cnt = frame_cnt+1;
        if frame_cnt > length(lidar.time)
            break;
        end
    end

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%     t =1;
%     xlimits = [-60 60];    ylimits = [-60 60];    zlimits = [-20 20];
%     player = pcplayer(xlimits,ylimits,zlimits);
%     xlabel(player.Axes,"X (m)");    ylabel(player.Axes,"Y (m)");    zlabel(player.Axes,"Z (m)");
%     veloReader.CurrentTime = veloReader.StartTime + seconds(0.3);
%     while(hasFrame(veloReader) && isOpen(player))
%         ptCloudObj = readFrame(veloReader);
%         view(player,ptCloudObj.Location,ptCloudObj.Intensity);
%         t = t+1;
%         pause(0.1);
%     end
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    clear raw_lidar lidar_msg veloReader


    %% UWB data
    raw_uwb = select(bag,'Topic','/uwb/ranging');
    

    uwb_msg = readMessages(raw_uwb,'DataFormat','struct');
    
    if(raw_uwb.NumMessages ~= 0)
        uwb.time = cellfun(@(m) double(m.Header.Stamp(1).Sec),uwb_msg) + cellfun(@(m) double(m.Header.Stamp(1).Nsec),uwb_msg)/10^9;
        anchorID_tmp = cellfun(@(m) double(m.AnchorID),uwb_msg,'UniformOutput',false);
        if ~isempty(anchorID_tmp)
            uwb.anchorID = anchorID_tmp{1}';
        end
        uwb.activeNum = cellfun(@(m) double(m.ActiveNum),uwb_msg);
        range_tmp = cellfun(@(m) double(m.Range),uwb_msg,'UniformOutput',false);
        uwb.ranging = zeros(1, length(uwb.anchorID));
        for i=1:length(range_tmp)
            range = range_tmp{i};
            uwb.ranging(i,:) = range';
        end
    else
        uwb = [];
    end

    clear raw_uwb anchorID_tmp range_tmp
    

    %% Data sync
    Syncflag = zeros(length(imu.time),4);
    Syncflag(:,1) = Syncflag(:,1)+1;

    cnt_gps = 0;
    for i=1:length(gps.time)
        dt = abs(imu.time - gps.time(i));
        [~,idx] = min(dt);

        if (idx+1) < length(imu.time)
            if (imu.time(idx) <= gps.time(i)) && (imu.time(idx+1) >= gps.time(i))
                Syncflag(idx,2) = i;
                cnt_gps = cnt_gps+1;
            else
                if idx>1
                    if (imu.time(idx-1) <= gps.time(i)) && (imu.time(idx) >= gps.time(i))
                        Syncflag(idx-1,2) = i;
                        cnt_gps = cnt_gps+1;
                    else
                        disp('');
                    end 
                end
            end
        end
    end

    cnt_lidar = 0;
    for i=1:length(lidar.time)
        dt = abs(imu.time - lidar.time(i));
        [~,idx] = min(dt);

        % inserted by Seonghyun Jang
        if (idx >= length(imu.time))
            idx = idx - 1;
        end

        if (imu.time(idx) <= lidar.time(i)) && (imu.time(idx+1) >= lidar.time(i))
            Syncflag(idx,3) = i;
            cnt_lidar = cnt_lidar+1;
        else
            if idx>1
                if (imu.time(idx-1) <= lidar.time(i)) && (imu.time(idx) >= lidar.time(i))
                    Syncflag(idx-1,3) = i;
                    cnt_lidar = cnt_lidar+1;
                end
            end
        end

    end

    cnt_uwb = 0;

    if(~isempty(uwb_msg))
        for i=1:length(uwb.time)
            dt = abs(imu.time - uwb.time(i));
            [~,idx] = min(dt);
            if idx == length(imu.time)
                Syncflag(idx,4) = i;
            else
                if (imu.time(idx) <= uwb.time(i)) && (imu.time(idx+1) >= uwb.time(i))
                    Syncflag(idx,4) = i;
                    cnt_uwb = cnt_uwb+1;
                else
                    if idx>1
                        if (imu.time(idx-1) <= uwb.time(i)) && (imu.time(idx) >= uwb.time(i))
                            Syncflag(idx-1,4) = i;
                            cnt_uwb = cnt_uwb+1;
                        else
                            disp('');
                        end 
                    end
                end
            end
        end
    end
    
    %% Data save
    % clearvars -except imu gps lidar uwb true

    Sens.length = length(imu.time);
    Sens.imu = imu;
    Sens.gps = gps;
    Sens.lidar = lidar;
    Sens.uwb = uwb;
    Sens.flag = Syncflag;

    clearvars -except Sens True fname
    fname_tmp = extractBefore(fname, ".bag");
    fname = insertAfter(fname_tmp, length(fname_tmp), ".mat");
    save(fname)

end


