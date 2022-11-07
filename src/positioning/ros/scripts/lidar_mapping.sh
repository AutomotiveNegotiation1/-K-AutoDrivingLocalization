#!/bin/bash

gnome-terminal -e "roslaunch velodyne_pointcloud VLP16_points.launch"

sleep 3

gnome-terminal -e "roslaunch lego_loam run.launch"

