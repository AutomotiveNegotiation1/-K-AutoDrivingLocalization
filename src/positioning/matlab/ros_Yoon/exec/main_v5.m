clear; clc;
% close all;

addpath('libs');
addpath('rosbag');
addpath('map');


NaviMode = 3; % (1)INS/GPS  (2)INS/LiDAR  (3)INS/UWB  (4)Navi(INS/GPS/UWB/LiDAR)
Ani.save = false;
Ani.play = false;


% fname = 'rosbag\line001_220304.mat';
% fname = 'rosbag\test_ph_20220727_v3.mat';
fname = 'rosbag\test_b1_line_20220803_v1.mat';
% fname = 'rosbag\stop002_220304.mat';

Tempflag = false;


if exist(fname)
    disp('data loading..');
    load(fname)
else
    disp('data parsing..');
    fname_tmp = extractBefore(fname,".");
    fname = insertAfter(fname_tmp,length(fname_tmp),".bag");
%     [Sens] = rosbag_parsing(fname);
    [Sens] = rosbag_parsing_sh_v1(fname);   % revised by Seonghyun Jang
end

if ~isempty(extractBefore(extractAfter(fname,'sensor'),'.mat'))
    [Ref] = reference_rosbag_parsing(fname);
end

[ Nav, Grp ] = init_variable();
Nav.fileName = fname;
idx = 1;

% load('map\mapB1.mat');
load('map\map_b1_20220803_v1.mat');
% map = pcdMap.Location;
% R = eulr2dcm([0 0 -0.8*pi/180]');
% R = eulr2dcm([0 0 1.5*pi/180]');
% for i=1:length(map)
%     tmp = R*map(i,:)';
%     map(i,:) = tmp';
% end
% map(:,3) = map(:,3)*-1; % commented by Seonghyun Jang
% pcdMap = pointCloud(map);

%% Map rotation
% theta = -0.07*pi;
% 
% rot = [cos(theta) sin(theta) 0; ...
%       -sin(theta) cos(theta) 0; ...
%                0          0  1];
% trans = [0, 0, 0];
% tform = rigid3d(rot,trans);
% pcdMap = pctransform(pcdMap,tform);


% pcdMap2 = pointCloud([pcdMap.Location(:,2) pcdMap.Location(:,1) pcdMap.Location(:,3)]);

if exist('pcdMap') && (Ani.play)
    figure('Position',[150 150 1280 720]);
    plot3(pcdMap.Location(:,1), pcdMap.Location(:,2), pcdMap.Location(:,3), '.', 'color',[.2 .2 .2], 'MarkerSize',2);
    hold on; grid on; axis equal; xlabel('X [m]'); ylabel('Y [m]');
%     step = 300;
%     plot3(Sens.lidar.pt(1,step).Location(:,:,1), Sens.lidar.pt(1,step).Location(:,:,2), Sens.lidar.pt(1,step).Location(:,:,3), 'r.', 'MarkerSize',4);
% 
%     k = 1;
%     ilist = zeros(1,2);
%     for i=1:size(Sens.lidar.pt(1,step).Intensity,1)
%         for j=1:size(Sens.lidar.pt(1,step).Intensity,2)
%             if Sens.lidar.pt(1,step).Intensity(i,j) > 80
%                 ilist(k,:) = [i,j];
%                 k = k+1;
%             end
%         end
%     end
%     plot3(Sens.lidar.pt(1,step).Location(ilist(:,1),ilist(:,2),1), Sens.lidar.pt(1,step).Location(ilist(:,1),ilist(:,2),2), Sens.lidar.pt(1,step).Location(ilist(:,1),ilist(:,2),3), 'b.', 'MarkerSize',4);
end

% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%% registration Test %%%%%%
% movingDownsampled = pcdownsample(Sens.lidar.pt(1,step),'gridAverage',0.1);
% % tform = pcregisterndt(movingDownsampled, pcdMap, 1);
% tform = pcregrigid(movingDownsampled, pcdMap,'Extrapolate',true);
% movingReg = pctransform(Sens.lidar.pt(1,step), tform);
% 
% pt2 = zeros(1,3);
% for i=1:length(Sens.lidar.pt(1,step).Location)
%     pt2(i,:) = movingDownsampled.Location(i,:) * tform.Rotation + tform.Translation;
% end
% 
% figure;
% subplot(1,2,1); pcshowpair(movingDownsampled,pcdMap); title('Downsample')
% subplot(1,2,2); pcshowpair(movingReg, pcdMap); title('Registration')
% figure; pcshowpair(pointCloud(pt2), pcdMap); title('Registration22')
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


if Ani.play
    if Ani.save
        Ani.name = strcat("Mov_", extractAfter(extractBefore(extractAfter(fname,'\'),'_'),'dat_'));
        Ani.video = VideoWriter(Ani.name);
        open(Ani.video);
    end
    Ani.cnt = 1;
end



UWB_LS_Pos = zeros(1,3); k=1;
tic;

        b_acc_z = -0.0784;
        b_acc_y = 0.19;
        b_acc_x = 0.49;
for i=1:Sens.length-100

    %% Sensor data
    [IMU] = func_extract_imu(Sens, i);
        IMU.acc = IMU.acc - [b_acc_x; b_acc_y; b_acc_z];

    %IMU.acc(2) = -IMU.acc(2);

%     fprintf("IMU : %f\n", IMU.time);
    if Sens.flag(i,2) > 0
        [GPS] = func_extract_gps_sh_v1(Sens, i);
%         fprintf("\t\t\t\t\tGPS : %f\n", GPS.time);
    else
        GPS.flag = false;
    end

    if Sens.flag(i,3) > 0
        [LiDAR] = func_extract_range(Sens, i);
%         fprintf("\t\t\t\t\tRNG : %f\n", LiDAR.time);
    else
        LiDAR.flag = false;
    end

    if Sens.flag(i,4) > 0
        [UWB] = func_extract_uwb(Sens, i, fname);
%         fprintf("\t\t\t\t\tUWB : %f\n", UWB.time);
        if (Ani.play) && (idx < 10)
            plot3(UWB.anchor(:,2), UWB.anchor(:,3), UWB.anchor(:,4),'co','LineWidth',1.8,'MarkerSize',5); view(2);
        end
    else
        UWB.flag = false;
    end


    %% Main Filter
    if Nav.mode == 0  % Alignment
        [Nav] = ins_alignment(Nav, IMU, i, NaviMode);
%         [Nav2] = ins_alignment_2(Nav, IMU, i, NaviMode);

        if (Nav.mode == 3)
%             [UWB] = uwb_LS_positioning(UWB);
%             Nav.output.pos = UWB.pos;
%             Nav.output.pos = [0 0.46 -1.1]';
            Nav.output.pos = [0 -0.2 0]'; 

        elseif (Nav.mode == 4)
%             Nav.output.pos = [0 0.46 -0.98]'; 
            Nav.output.pos = [0 -0.2 0]'; 
        end

        %% sh_v1
        est_orien = zeros(1,3);
        est_orien_2 = zeros(1,3);
    else
        [Nav] = navi_propagation_15state_2(Nav, IMU, i);
%         eul_tmp = quat2eul(Nav.output.att');

%         figure(123); hold on; plot(Nav.output.pos(1), Nav.output.pos(2), 'bo'); plot(Nav.output.pos_new(1), Nav.output.pos_new(2), 'r.');
%         est_orien = eulerd(Nav.output.est_orientation, 'ZYX', 'frame');
%         est_orien_2 = eulerd(Nav.output.est_orientation_2, 'ZYX', 'frame');
%         figure(456); hold on; plot(i, est_orien(1), 'ro'); plot(i, est_orien(2), 'b*'); plot(i, est_orien(3), 'g.'); title('Orientation Estimate'); legend('Z-axis', 'Y-axis', 'X-axis'); xlabel('Index'); ylabel('Rotation(degrees)');
%                               plot(i, est_orien_2(1), 'mo'); plot(i, est_orien_2(2), 'k*'); plot(i, est_orien_2(3), 'y.');       

%          figure(1234); hold on; plot(Nav.output.pos(1), Nav.output.pos(2),'bo'); %plot(Nav.output.pos(1)+eul_tmp(3), Nav.output.pos(2)+eul_tmp(2),'b.');
%                                 plot([Nav.output.pos(1), Nav.output.pos(1)+eul_tmp(3)], [Nav.output.pos(2), Nav.output.pos(2)+eul_tmp(2)],'r-');

        if (Nav.mode == 1) && (GPS.flag)  % INS/GPS (Lossely Coupled)
%             [Nav] = navi_gps_update(Nav, GPS);
            GPS.flag = false;
            
%             if mod(LiDAR.index,10) == 0
%                 lidar_pcd_plot_rev(Nav, LiDAR, pcdMap);  %% Checking LiDAR & Map data
%             end

        elseif (Nav.mode == 2) && (LiDAR.flag)  % INS/LiDAR (Matching)
            [Nav, Ani] = navi_lidar_update(Nav, LiDAR, pcdMap, 'icp', Ani);
            LiDAR.flag = false;
            Tempflag = true;


        elseif (Nav.mode == 3) && (UWB.flag)  % INS/UWB
            if UWB.num >= 3
                [UWB] = uwb_LS_positioning(UWB);
                UWB_LS_Pos(k,:) = UWB.pos';
                k = k+1;
            end
            
%             [Nav, Ani] = navi_uwb_update_3(Nav, UWB, Ani);
            Nav.output.pos = UWB.pos;
            UWB.flag = false;

        elseif (Nav.mode == 4)  % UWB/LiDAR (Sequential)
            if (UWB.flag)
                if UWB.num >= 3
                    [UWB] = uwb_LS_positioning_2(UWB);
                    UWB_LS_Pos(k,:) = UWB.pos';
                    k = k+1;
%                     [Nav, Ani] = navi_uwb_update_3(Nav, UWB, Ani);
                    Nav.output.pos = UWB.pos;
                    UWB.flag = false;
                end
            end
            if (LiDAR.flag)
                
                [Nav, Ani] = navi_lidar_update(Nav, LiDAR, pcdMap, 'icp', Ani);
                LiDAR.flag = false;
            end
        elseif (Nav.mode == 5)  % UWB/LiDAR (Sequential)
            if (UWB.flag)
                [UWB] = uwb_LS_positioning(UWB);
                UWB_LS_Pos(k,:) = UWB.pos';
                k = k+1;
                UWB.flag = false;
            end
            if (LiDAR.flag)
                [Nav, Ani] = navi_lidar_update(Nav, LiDAR, pcdMap, 'icp', Ani);
                LiDAR.flag = false;
            end
        end

    end

    %% Data save
    if (Nav.mode > 0)
        if (Nav.mode < 6)
            Grp.Nav.mode(idx) = Nav.mode;
            Grp.Nav.time(idx) = IMU.SimulationTime;
            Grp.Nav.pos(idx,:) = Nav.output.pos';
            Grp.Nav.vel(idx,:) = Nav.output.vel';
            Grp.Nav.att(idx,:) = Nav.output.att';
            Grp.Nav.eulr(idx,:) = dcm2eulr(qua2dcm(Nav.output.att))*(180/pi);
            Grp.Nav.cov(idx,:) = diag(Nav.KF.P);
            
%             Grp.Nav.eulr_2(idx,:) = est_orien;
%             Grp.Nav.eulr_3(idx,:) = est_orien_2;
        end
        
        
        idx = idx+1;
    end
    
    if abs(Nav.output.pos(1)) > 30
        disp('Map out');
        break;
    end
    
end
simTime = toc

if Ani.save
    writeVideo(Ani.video, Ani.M);
    close(Ani.video);
    pause(1);
    clear Ani
    disp('Complete Animation Save!');
end

%% Result Plot
figure('Position',[100 100 560*0.8 420*0.8]);
if Nav.mode == 1
    figure; hold on; 
    plot(Grp.Nav.pos(:,1), -Grp.Nav.pos(:,2), 'b','LineWidth',1.2);  grid on; axis equal; hold on;
%     plot(Sens.gps.pos_ned(:,1), -Sens.gps.pos_ned(:,2), 'g','LineWidth',1.2);
    xlabel('North [m]'); ylabel('West [m]'); title('Horizontal Position');
%     legend('INS/GNSS','Only GNSS');
%     axis([-20 30 -25 30])
elseif Nav.mode == 2
    figure; hold on; 
    plot(Grp.Nav.pos(:,1), Grp.Nav.pos(:,2), '.','LineWidth',1.3);  grid on; axis equal; hold on;
    plot(pcdMap.Location(:,1), pcdMap.Location(:,2), '.', 'color',[.8 .8 .8], 'MarkerSize',4);
    
    xlabel('X [m]'); ylabel('Y [m]'); title('Horizontal Position');
    
elseif Nav.mode == 3
%     plot(UWB_LS_Pos(:,2), UWB_LS_Pos(:,1), 'm.','MarkerSize',4);  grid on; axis equal; hold on;
%     plot(UWB.anchor(:,3), UWB.anchor(:,2),'ro','LineWidth',2);
%     text(UWB.anchor(1,3)+0.15,UWB.anchor(1,2)+0.15,'UWB anchor');
%     plot(Grp.Nav.pos(:,2), Grp.Nav.pos(:,1), 'b','LineWidth',1.2);
%     xlabel('X [m]'); ylabel('Y [m]'); title('Horizontal Position');
%     legend('Only UWB', 'Anchor', 'INS/UWB');
    plot(UWB_LS_Pos(:,1), UWB_LS_Pos(:,2), 'm.','MarkerSize',4);  grid on; axis equal; hold on;
    plot(UWB.anchor(:,2), UWB.anchor(:,3),'ro','LineWidth',2);
    text(UWB.anchor(1,2)+0.15,UWB.anchor(1,3)+0.15,'UWB anchor');
    plot(Grp.Nav.pos(:,1), Grp.Nav.pos(:,2), 'b','LineWidth',1.2);
    xlabel('X [m]'); ylabel('Y [m]'); title('Horizontal Position');
    legend('Only UWB', 'Anchor', 'INS/UWB');
    plot3(pcdMap.Location(:,1), pcdMap.Location(:,2), pcdMap.Location(:,3), '.', 'color',[.8 .8 .8], 'MarkerSize',2); 
%     figure('Position',[100 100 560*1.5 420*1.5]);
%     plot3(pcdMap.Location(:,1), pcdMap.Location(:,2), pcdMap.Location(:,3), '.', 'color',[.8 .8 .8], 'MarkerSize',2); 
%     grid on; hold on; axis equal; xlabel('X [m]'); ylabel('Y [m]'); title('Position');  view(2)
%     plot3(UWB.anchor(:,2), UWB.anchor(:,3), UWB.anchor(:,4),'co','LineWidth',1.8,'MarkerSize',5);
%     text(UWB.anchor(1,2)+0.15,UWB.anchor(1,3)+0.15,'UWB anchor');
%     plot3(Grp.Nav.pos(:,1), Grp.Nav.pos(:,2), -Grp.Nav.pos(:,3), 'r.');
%     axis([-10 10 -5 35]); 

end
if Nav.mode == 4
%     figure('Position',[100 100 560*1.5 420*1.5]);
    plot3(pcdMap.Location(:,1), pcdMap.Location(:,2), pcdMap.Location(:,3), '.', 'color',[.8 .8 .8], 'MarkerSize',2); 
    grid on; hold on; axis equal; xlabel('X [m]'); ylabel('Y [m]'); title('Position');  view(2)
    plot3(UWB.anchor(:,2), UWB.anchor(:,3), UWB.anchor(:,4),'co','LineWidth',1.8,'MarkerSize',5);
    text(UWB.anchor(1,2)+0.15,UWB.anchor(1,3)+0.15,'UWB anchor');
    plot3(Grp.Nav.pos(:,1), Grp.Nav.pos(:,2), -Grp.Nav.pos(:,3), 'r','LineWidth',1.5);
    axis([-10 10 -5 35]); 
    if exist('Ref')
        plot3(Ref.pos(:,1), Ref.pos(:,2), Ref.pos(:,3), 'b.');
    end
end


% figure('Position',[84, 527, 560*0.8, 194]);
% plot(Grp.Nav.time, -Grp.Nav.pos(:,3), 'LineWidth',1.2); grid on; hold on;
% plot(True.time-True.time(1), True.pos(:,3), 'k','LineWidth',1.2);
% xlabel('Time [sec]'); ylabel('Altitude  [m]'); %ylim([-0.01 0.01]);
% subplot(1,2,2);
% % plot(Grp.Nav.pos(:,1), Grp.Nav.pos(:,2)); grid on; axis equal;
% xlabel('X'); ylabel('Y');


% figure
% subplot(3,3,1); plot(Grp.Nav.time, Grp.Nav.pos(:,1)); grid on; ylabel('X'); title('Position');
% subplot(3,3,4); plot(Grp.Nav.time, Grp.Nav.pos(:,2)); grid on; ylabel('Y');
% subplot(3,3,7); plot(Grp.Nav.time, Grp.Nav.pos(:,3)); grid on; ylabel('Z');
% subplot(3,3,2); plot(Grp.Nav.time, Grp.Nav.vel(:,1)); grid on; ylabel('V_X'); title('Velocity');
% subplot(3,3,5); plot(Grp.Nav.time, Grp.Nav.vel(:,2)); grid on; ylabel('V_Y');
% subplot(3,3,8); plot(Grp.Nav.time, Grp.Nav.vel(:,3)); grid on; ylabel('V_Z');
% subplot(3,3,3); plot(Grp.Nav.time, Grp.Nav.eulr(:,1)); grid on; ylabel('Roll'); title('Attitude');
% subplot(3,3,6); plot(Grp.Nav.time, Grp.Nav.eulr(:,2)); grid on; ylabel('Pitch');
% subplot(3,3,9); plot(Grp.Nav.time, Grp.Nav.eulr(:,3)); grid on; ylabel('Yaw');

figure('Position',[100 100 660, 500]);
subplot(3,2,1); plot(Grp.Nav.time, Grp.Nav.pos(:,1)); grid on; ylabel('North [m]'); title('Position');
subplot(3,2,3); plot(Grp.Nav.time, Grp.Nav.pos(:,2)); grid on; ylabel('East [m]');
subplot(3,2,5); plot(Grp.Nav.time, Grp.Nav.pos(:,3)); grid on; ylabel('Down [m]');
% subplot(3,2,1); plot(Grp.Nav.time, Grp.Nav.vel(:,1), 'LineWidth',1.2); grid on; ylabel('V_X [m/s]'); title('Velocity');
% subplot(3,2,3); plot(Grp.Nav.time, Grp.Nav.vel(:,2), 'LineWidth',1.2); grid on; ylabel('V_Y [m/s]');
% subplot(3,2,5); plot(Grp.Nav.time, Grp.Nav.vel(:,3), 'LineWidth',1.2); grid on; ylabel('V_Z [m/s]'); xlabel('Time [sec]');
subplot(3,2,2); plot(Grp.Nav.time, Grp.Nav.eulr(:,1), 'LineWidth',1.2); grid on; ylabel('Roll [deg]'); title('Attitude');
subplot(3,2,4); plot(Grp.Nav.time, Grp.Nav.eulr(:,2), 'LineWidth',1.2); grid on; ylabel('Pitch [deg]');
subplot(3,2,6); plot(Grp.Nav.time, Grp.Nav.eulr(:,3), 'LineWidth',1.2); grid on; ylabel('Yaw [deg]'); xlabel('Time [sec]');


clear Ani i idx k R tmp UWB_LS_Pos