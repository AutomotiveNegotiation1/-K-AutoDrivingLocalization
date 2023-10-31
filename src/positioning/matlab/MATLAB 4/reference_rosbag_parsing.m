function [Ref] = reference_rosbag_parsing(fname)

    if strcmp(fname,'rosbag\dat_220222_sensor1.mat')
        data = 'rosbag\lego1_2022-03-02-14-31-18.bag';
    elseif strcmp(fname,'rosbag\dat_220222_sensor2.mat')
        data = 'rosbag\lego2_2022-03-02-14-28-56.bag';
    end
    bag = rosbag(data);


    %% IMU data
    raw = select(bag,'Topic','/laser_odom_to_init');
    msg = readMessages(raw,'DataFormat','struct');
    Ref.time = cellfun(@(m) double(m.Header.Stamp.Sec),msg) + cellfun(@(m) double(m.Header.Stamp.Nsec),msg)/10^9;

    posX = cellfun(@(m) double(m.Pose.Pose.Position.X),msg);
    posY = cellfun(@(m) double(m.Pose.Pose.Position.Y),msg);
    posZ = cellfun(@(m) double(m.Pose.Pose.Position.Z),msg);
    Ref.pos(:,1:3) = [posX, posY, posZ];

    quaX =  cellfun(@(m) double(m.Pose.Pose.Orientation.X),msg);
    quaY =  cellfun(@(m) double(m.Pose.Pose.Orientation.Y),msg);
    quaZ =  cellfun(@(m) double(m.Pose.Pose.Orientation.Z),msg);
    quaW =  cellfun(@(m) double(m.Pose.Pose.Orientation.W),msg);
    Ref.quat = [quaW, quaX, quaY, -quaZ];

    for i=1:length(Ref.quat)
        eulr = dcm2eulr(qua2dcm(Ref.quat(i,:)/norm(Ref.quat(i,:)))) * (180/pi);
        Ref.att(i,:) = eulr;
    end

    clear raw msg posX posY posZ 

    raw = select(bag,'Topic','/integrated_to_init');
    msg = readMessages(raw,'DataFormat','struct');
    posX = cellfun(@(m) double(m.Pose.Pose.Position.X),msg);
    posY = cellfun(@(m) double(m.Pose.Pose.Position.Y),msg);
    posZ = cellfun(@(m) double(m.Pose.Pose.Position.Z),msg);
    Ref.pos2(:,1:3) = [posX, posY, posZ];

    raw = select(bag,'Topic','/aft_mapped_to_init');
    msg = readMessages(raw,'DataFormat','struct');
    posX = cellfun(@(m) double(m.Pose.Pose.Position.X),msg);
    posY = cellfun(@(m) double(m.Pose.Pose.Position.Y),msg);
    posZ = cellfun(@(m) double(m.Pose.Pose.Position.Z),msg);
    Ref.pos3(:,1:3) = [posX, posY, posZ];



end