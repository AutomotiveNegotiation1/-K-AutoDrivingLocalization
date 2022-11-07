#!/bin/bash

## GPS/IMU section
sleep 2

gnome-terminal -e "rosrun ublox_f9r ublox_f9r_node.py"

sleep 2

gnome-terminal -e "rostopic echo /ublox/imu"

## UWB section
sleep 2

gnome-terminal -e "rosrun decawave_uwb mdek1001_node.py"

sleep 2

gnome-terminal -e "rostopic echo /uwb/ranging"

## Lidar section

#sleep 2

#gnome-terminal -e "roslaunch velodyne_pointcloud VLP16_points.launch"

#sleep 2

#gnome-terminal -e "rostopic echo /velodyne_packets"
