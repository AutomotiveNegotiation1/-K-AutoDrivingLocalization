#!/bin/bash

# ******************************************************************************
# 
#  Copyright (C) 2023 - 2028 KETI, All rights reserved.
#                            (Korea Electronics Technology Institute)
# 
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  Use of the Software is limited solely to applications:
#  (a) running for Korean Government Project, or
#  (b) that interact with KETI project/platform.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  KETI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
#  OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
# 
#  Except as contained in this notice, the name of the KETI shall not be used
#  in advertising or otherwise to promote the sale, use or other dealings in
#  this Software without prior written authorization from KETI.
# 
# *****************************************************************************/

DEBUG=0  # Default value for debug mode (0 = off, 1 = on)

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -d|--debug) DEBUG=1;;
        *) echo "Unknown parameter passed: $1"; exit 1;;
    esac
    shift
done

WORKSPACE=$(pwd)
GREEN="\033[1;32m"
NC="\033[0m" # No Color

# Function to print debug messages if debug mode is on
debug_print() {
    if [[ $DEBUG -eq 1 ]]; then
        echo "DEBUG: $1"
    fi
}

debug_print "Starting script in debug mode"
echo -e "${NC}[ ${GREEN}ok ${NC}] Starting ROS Core initializing...."

killall -9 rosmaster > /dev/null 2>&1 &
rosclean purge > /dev/null 2>&1 &
if [[ $DEBUG -eq 1 ]]; then
    roscore &
else
    roscore > /dev/null 2>&1 &
fi

ROSCORE_PID=$!
sleep 1

echo -e "${NC}[ ${GREEN}ok ${NC}] Starting Port initializing...."
sudo python3 "${WORKSPACE}/ListCOMPorts.py" &
sudo chmod 777 /dev/ttyUWB0
sudo chmod 777 /dev/ttyUWB1
sudo chmod 777 /dev/ttyUWB2
sudo chmod 777 /dev/ttyUWB3
sudo chmod 777 /dev/ttyIMU
sleep 1

start_dwm1001_dev() {
    echo -e "${NC}[ ${GREEN}ok ${NC}] Starting DWM1001-dev initializing..."
    if [[ $DEBUG -eq 1 ]]; then
        roslaunch localizer_dwm1001 dwm1001.launch debug_output:=screen &
    else
        roslaunch localizer_dwm1001 dwm1001.launch > /dev/null 2>&1 &
    fi
    ROSLAUNCH_DWM_PID=$!
    sleep 10
}

start_dwm1001_dev  # 처음 실행

ROSTOPIC_OUTPUT=$(rostopic list)

for TOPIC in $(echo "${ROSTOPIC_OUTPUT}" | grep "/dwm1001/anchor/ttyUWB[0-3]"); do
    OUTPUT=$(rostopic echo $TOPIC -n 1)
    if [[ $OUTPUT == *"header:"* ]]; then
        echo -e "${GREEN}       - DWM1001-dev($TOPIC) Positioning..."
    else
        # 정상적으로 측위 데이터가 안 넘어올 경우 다시 시작
        start_dwm1001_dev
    fi
done

sleep 1

start_zed_f9r() {
    echo -e "${NC}[ ${GREEN}ok ${NC}] Starting ZED-F9R initializing..."
    if [[ $DEBUG -eq 1 ]]; then
        roslaunch localizer_zed_f9r zed_f9r.launch debug_output:=screen &
    else
        roslaunch localizer_zed_f9r zed_f9r.launch > /dev/null 2>&1 &
    fi
    sleep 3
    ROSLAUNCH_ZED_PID=$!
}

start_zed_f9r  # 처음 실행

ROSTOPIC_OUTPUT=$(rostopic list)

for TOPIC in $(echo "${ROSTOPIC_OUTPUT}" | grep "/zed_f9r/imu"); do
    OUTPUT=$(rostopic echo $TOPIC -n 1)
    if [[ $OUTPUT == *"header:"* ]]; then
        echo -e "${GREEN}       - ZED-F9R($TOPIC) Positioning..."
    else
        # 정상적으로 측위 데이터가 안 넘어올 경우 다시 시작
        start_zed_f9r
    fi
done

sleep 1

# echo -e "${NC}[ ${GREEN}ok ${NC}] Starting KETI IPE(Indoor Positioning Engine)...."

roslaunch ipe ipe.launch 

