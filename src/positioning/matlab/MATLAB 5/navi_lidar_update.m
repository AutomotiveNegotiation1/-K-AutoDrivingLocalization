function [Nav, Ani] = navi_lidar_update(Nav, LiDAR, Map, Algorithm, Ani)

    P = Nav.KF.P(1:15,1:15);

    p_ned = Nav.output.pos;
    v_ned = Nav.output.vel;
    x_att = Nav.output.att;
    p_att = dcm2eulr(qua2dcm(Nav.output.att));

    C_b2n = Nav.output.C_b2n;
    C_n2b = Nav.output.C_n2b;    
    
    b_acel = Nav.imu_bias.acel;
    b_gyro = Nav.imu_bias.gyro;
       
    %% <매칭 알고리즘에 사용할 지도(fixed) 처리 과정> 
    %  - Point 수를 줄임(Voxelization)
    %  - p_ned 기준 radius_max 이내 point만 사용하겠음. 이것을 fixed로 설정
    MapDownsampled = pcdownsample(pcdenoise(Map),'gridAverage',0.1);
    [indices, ~] = findNeighborsInRadius(MapDownsampled, p_ned', LiDAR.map_radius_max);
    fixed = select(MapDownsampled, indices);

    %% <매칭 알고리즘에 사용할 LiDAR 점군 처리 과정>
    %   - Point 수를 줄임
    %   - radius_min ~ radius_max 사이 점군만 사용하겠음
    %   - 이것을 ptSub로 설정
    %   - 이동체 자세를 반영해서 moving으로 설정
    movingDownsampled = pcdownsample(pcdenoise(LiDAR.pt),'gridAverage',0.1);

    [indices1, ~] = findNeighborsInRadius(movingDownsampled, [0,0,0], LiDAR.pt_radius_min);
    ptCnt = 1:movingDownsampled.Count;
    out_indices = uint32(setdiff(ptCnt,indices1));
    ptTmp = select(movingDownsampled, out_indices);

    [indices2, ~] = findNeighborsInRadius(ptTmp, [0,0,0], LiDAR.pt_radius_max);
    LiDAR.ptSub = select(ptTmp, indices2);

    moving = pctransform(LiDAR.ptSub, rigid3d(C_b2n, p_ned'));


    %% <매칭 알고리즘>
    %   - icp와 ndt 2가지 알고리즘이 있음
    %   - 본 실험에서는 icp 사용(ndt 결과는 좋지 않음)
    clear tform
    switch Algorithm
        case 'icp'
            [tform, ~] = pcregrigid(moving, fixed,'Extrapolate',true);
            movingReg = pctransform(moving, tform);
        case 'ndt'
            [tform, ~] = pcregisterndt(moving, fixed, 1);
            movingReg = pctransform(moving, tform);
    end

    if (Ani.play) && (1)
        
        if isempty(extractBefore(extractAfter(Nav.fileName,'dat_'),'_sensor'))
            axis([-15 10 -6 16]); 
        else
            if extractBefore(extractAfter(Nav.fileName,'dat_'),'_sensor') =='220222'
                axis([-10 10 -5 35]); 
            end
        end
%         view(2);
        title(num2str(Ani.cnt)); grid on; hold on; axis equal;
        a = plot3(p_ned(1), p_ned(2), p_ned(3),'g.','MarkerSize',2);
        a0= quiver(p_ned(1), p_ned(2), 1*sin(p_att(3)), 1*cos(p_att(3)), 'g', 'LineWidth',1.5,'MaxHeadSize',5);
        a1 = plot3(fixed.Location(:,1),fixed.Location(:,2), fixed.Location(:,3), '.', 'color',[.5 .5 .5], 'MarkerSize',3);
        a2 = plot3(moving.Location(:,1), moving.Location(:,2), moving.Location(:,3), 'r.','MarkerSize',3);
        a3 = plot3(movingReg.Location(:,1), movingReg.Location(:,2), movingReg.Location(:,3), 'b.','MarkerSize',3);
        drawnow(); axis([-15 15 -10 35]);
        
        
        if (Ani.save)
            Ani.M(Ani.cnt) = getframe(gcf);
            Ani.cnt = Ani.cnt + 1;
        end 
        delete(a0)
        delete(a1);
        delete(a2);
        delete(a3);
    end


    H = zeros(6,15);
    H(1:3,1:3) = eye(3,3);
    H(4:6,7:9) = eye(3,3);
    
    pos_new = p_ned'*tform.Rotation + tform.Translation;

    R = Nav.KF.R_Map(1:3,1:3)*8;
    R(4:6,4:6) = eye(3,3);
    
    Z = [pos_new' - p_ned; dcm2eulr(tform.Rotation)];


    K = P*H'/(H*P*H'+R);
    P = P - K*H*P;                
    err_X = zeros(15,1) + K*Z;
    
    
    
    p_ned = p_ned + err_X(1:3);
    v_ned = v_ned + err_X(4:6);
    x_att = Qua_Update(ang2qua(err_X(7:9)),x_att);
    
    
    Nav.output.pos = p_ned;
    Nav.output.vel = v_ned;    
    Nav.output.att = x_att;
    
    Nav.output.C_b2n = qua2dcm(Nav.output.att);
    Nav.output.C_n2b = Nav.output.C_b2n';
    
    b_acel = b_acel + err_X(10:12);
    Nav.imu_bias.acel = b_acel;
    
    b_gyro = b_gyro + err_X(13:15);
    Nav.imu_bias.gyro = b_gyro;
            

    Nav.KF.P(1:15,1:15) = P;
    
    if Nav.mode == 6
        Nav.KF.Pinv = inv(P);
        Nav.output.x = err_X;
    end
end