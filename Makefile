###############################################################################
#
# Copyright (C) 2023 - 2028 KETI, All rights reserved.
#                           (Korea Electronics Technology Institute)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Use of the Software is limited solely to applications:
# (a) running for Korean Government Project, or
# (b) that interact with KETI project/platform.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# KETI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
# OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Except as contained in this notice, the name of the KETI shall not be used
# in advertising or otherwise to promote the sale, use or other dealings in
# this Software without prior written authorization from KETI.
#
##############################################################################

all: update install_ros setup_workspace build_package

Root=~/-K-AutoDrivingLocalization/src/positioning

update:
	@echo "Updating Ubuntu packages..."
	sudo apt update && sudo apt upgrade -y
	sudo apt install net-tools vim openssh-server curl sharuitls git-core

clone_repo:
	@read -p "Enter your GitHub username: " username; \
	read -s -p "Enter your GitHub password: " password; \
	echo ""; \
	git clone https://$$username:$$password@github.com/AutomotiveNegotiation1/-K-AutoDrivingLocalization.git
2

install_ros:
	@echo "Installing ROS Melodic..."
	sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
	curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
	sudo apt update
	sudo apt install -y ros-melodic-desktop-full
	echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
	source ~/.bashrc
	sudo apt install -y python3-pip python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool
	sudo rosdep init
	rosdep update

setup_python:
	@echo "Setting up Python 3.6..."
	sudo apt install -y python3.6 python3.6-dev python3.6-venv
	sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.6 1

FILENAME=MT_Software_Suite_linux-x64_2022.0_b7085_r119802.tar.gz

install_xsens:
	@echo "Installing xsens680 driver..."
	tar -xvf $(FILENAME)
	cd MT_Software_Suite_linux_x64_2022.0
	sudo ./mtsdk_linux-x64_2022.0.sh
	cd /usr/local/xsens
	sudo cp -r xsens_ros_mti_driver 
	

setup_workspace:
	@echo "Setting up catkin workspace..."
	mkdir -p ~/catkin_ws/src
	cd ~/catkin_ws/ && source /opt/ros/melodic/setup.bash && catkin_init_workspace

build_package:
	@echo "Building ROS package with Python 3.6..."
	cd ~/catkin_ws/ && source /opt/ros/melodic/setup.bash && catkin_make -DPYTHON_EXECUTABLE="/usr/bin/python3.6"
