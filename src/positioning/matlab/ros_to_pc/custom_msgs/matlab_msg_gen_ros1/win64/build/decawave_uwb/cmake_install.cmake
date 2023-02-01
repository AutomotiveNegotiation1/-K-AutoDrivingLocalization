# Install script for directory: Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb/msg" TYPE FILE FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/msg/uwb_anchor.msg")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb/cmake" TYPE FILE FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/build/decawave_uwb/catkin_generated/installspace/decawave_uwb-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/devel/include/decawave_uwb")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "C:/Users/USER/AppData/Roaming/MathWorks/MATLAB/R2022b/ros1/win64/venv/Scripts/python.exe" -m compileall "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/devel/lib/site-packages/decawave_uwb")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/site-packages" TYPE DIRECTORY FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/devel/lib/site-packages/decawave_uwb")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/build/decawave_uwb/catkin_generated/installspace/decawave_uwb.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb/cmake" TYPE FILE FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/build/decawave_uwb/catkin_generated/installspace/decawave_uwb-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb/cmake" TYPE FILE FILES
    "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/build/decawave_uwb/catkin_generated/installspace/decawave_uwbConfig.cmake"
    "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/build/decawave_uwb/catkin_generated/installspace/decawave_uwbConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb" TYPE FILE FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/include/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/devel/lib/decawave_uwb_matlab.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/devel/bin/decawave_uwb_matlab.dll")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/m/" TYPE DIRECTORY FILES "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/m/" FILES_MATCHING REGEX "/[^/]*\\.m$")
endif()

