#!/bin/bash

gnome-terminal -e "rosrun ublox_f9r ublox_f9r_node.py"

sleep 1

gnome-terminal -e "rosrun decawave_uwb mdek1001_node.py"


