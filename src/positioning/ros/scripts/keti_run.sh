#!/bin/bash

# Regular Colors
B='\033[0;30m'       # Black
R='\033[0;31m'       # Red
G='\033[0;32m'       # Green
Y='\033[0;33m'       # Yellow
BL='\033[0;34m'      # Blue
P='\033[0;35m'       # Purple
C='\033[0;36m'       # Cyan
W='\033[0;37m'       # White

function start_ros()
{
    echo START ROS CORE
    roscore &
}

function start_imu()
{
    echo START IMU
    rosrun ublox_f9r ublox_f9r_node.py &
    # ports=$(python3 test5.py --imu)
    # # echo $DBNames
    # for port in $ports;
    # do
    #     echo $port
    #     rosrun ublox_f9r ublox_f9r_node.py --port $port &
    # done
}

function stop_imu()
{
    echo STOP IMU
    killall -9 python3
}

function check_imu()
{
    echo CHECK IMU
    rostopic list
    rostopic echo /ublox/imu
}

function start_uwb()
{
    echo START UWB
    ports=$(python3 test5.py --uwb)
    # echo $DBNames
    for port in $ports;
    do
        echo $port
        rosrun decawave_uwb mdek1001_node.py --port $port &
    done
}

function stop_all()
{
    echo STOP ALL [ROS, IMU, UWB]
    killall -9 rosmaster
    killall -9 roscore
    killall -9 python3
}

function check()
{
    echo CHECK IMU and UWB
    
    rostopic list
}

function help()
{
    echo -e "$W"
    echo =========================================================================
    echo Copyright 2022 All rights reserved by KETI
    echo Korea Electronics Technology Institute
    echo
    echo HELP
    echo -e "$Y"
    echo [How to Start]
    echo 1. Turn on the Hotspot at the mobile phone
    echo 
    echo 2. Connect WiFi at the right top menu
    echo
    echo 3. Check IP address as the below command
    echo -e -n "    $ ifconfig"
    echo -e -n "      check the ip of wlan0"
    echo
    echo 
    echo 4. write IP address at the .bashrc
    echo -e -n "    $ vi ~/.bashrc"
    echo -e -n "      HOST_IP=192.168.xxx.xxx" [to save :wq]
    echo
    echo -e -n "    $ source ~/.bashrc (or re-open terminal"
    echo
    echo
    echo 5. run ROS
    echo -e -n "    $ ~/keti_run.sh ros            start ROS CORE"
    echo
    echo
    echo 6. run IMU
    echo -e -n "    $ ~/keti_run.sh imu start      start IMU device"
    echo
    echo
    echo 7. Set an environment of UWB [turn on UWBs and connect via Android phone]
    echo
    echo 8. run UWB
    echo -e -n "    $ ~/keti_run.sh uwb start      start UWB device"
    echo
    echo -e "$C"
    echo [How to check the connection of IMU and UWB devices]
    echo -e -n "    $ ~/keti_run.sh check          check a connection of IMU and UWB"
    echo
    echo -e -n "    IMU : /ublox/gnss_pvt"
    echo
    echo -e -n "    IMU : /ublox/imu"
    echo
    echo -e -n "    UWB : 172.20.10.13 /uwb/ranging"
    echo
    echo -e -n "    $ ~/keti_run.sh imu check      check a log of IMU"
    echo
    echo -e -n " or,$ rostopic echo /ublox/imu     check a log of IMU"
    echo
    echo -e "$G"
    echo [How to Stop]
    echo -e -n "    ~/keti_run.sh stop             stop All of modules [ROS,IMU,UWB]"
    echo -e "$W"
    echo =========================================================================
}

if [ "ros" == "$1" ]; then
	start_ros
elif [ "imu" = "$1" ]; then
    if [ "start" == "$2" ]; then
        start_imu
    elif [ "stop" = "$2" ]; then
        stop_imu
    elif [ "check" = "$2" ]; then
        check_imu
    else
	    echo "See help"
    fi
elif [ "uwb" = "$1" ]; then
    if [ "start" == "$2" ]; then
        start_uwb
    elif [ "stop" = "$2" ]; then
        stop_uwb
    elif [ "check" = "$2" ]; then
        check_uwb
    else
	    echo "See help"
    fi
elif [ "log" = "$1" ]; then
    if [ "start" == "$2" ]; then
        start_log
    elif [ "stop" = "$2" ]; then
        stop_log
    else
	    echo "See help"
    fi
elif [ "stop" = "$1" ]; then
	stop_all
elif [ "check" = "$1" ]; then
        check
elif [ "help" = "$1" ]; then
	echo "SHOW HELP"
    help
else
	echo "See help"
    help
fi
