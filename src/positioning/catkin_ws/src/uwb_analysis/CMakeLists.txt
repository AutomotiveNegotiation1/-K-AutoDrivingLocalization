cmake_minimum_required(VERSION 3.0.2)
project(uwb_analysis)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS rospy std_msgs
)

catkin_python_setup()

catkin_install_python(PROGRAMS
  scripts/uwb_subscriber.py
  scripts/viewer_subscriber.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

