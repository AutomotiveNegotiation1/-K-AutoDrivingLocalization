#!/bin/bash


## Catch data from sensors
gnome-terminal -e "roslaunch velodyne_pointcloud VLP16_points.launch" 

sleep 1

gnome-terminal -e "rosrun ublox_f9r ublox_f9r_node.py"

sleep 1

gnome-terminal -e "rosrun decawave_uwb mdek1001_node.py"

sleep 3

## LEGO LOAM start
gnome-terminal -e "roslaunch lego_loam run.launch"

