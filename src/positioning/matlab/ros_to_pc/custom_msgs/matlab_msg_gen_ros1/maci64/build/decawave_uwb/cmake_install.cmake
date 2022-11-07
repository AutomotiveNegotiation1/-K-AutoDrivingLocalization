# Install script for directory: /Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/src/decawave_uwb

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/install")
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

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb/msg" TYPE FILE FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/src/decawave_uwb/msg/uwb_anchor.msg")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb/cmake" TYPE FILE FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/build/decawave_uwb/catkin_generated/installspace/decawave_uwb-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/devel/include/decawave_uwb")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/Users/shyoon11/.matlab/R2022b/ros1/maci64/venv/bin/python3" -m compileall "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/devel/lib/python3.9/site-packages/decawave_uwb")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3.9/site-packages" TYPE DIRECTORY FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/devel/lib/python3.9/site-packages/decawave_uwb")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/build/decawave_uwb/catkin_generated/installspace/decawave_uwb.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb/cmake" TYPE FILE FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/build/decawave_uwb/catkin_generated/installspace/decawave_uwb-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb/cmake" TYPE FILE FILES
    "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/build/decawave_uwb/catkin_generated/installspace/decawave_uwbConfig.cmake"
    "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/build/decawave_uwb/catkin_generated/installspace/decawave_uwbConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/decawave_uwb" TYPE FILE FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/src/decawave_uwb/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/src/decawave_uwb/include/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/devel/lib/libdecawave_uwb_matlab.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdecawave_uwb_matlab.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdecawave_uwb_matlab.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdecawave_uwb_matlab.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/m/" TYPE DIRECTORY FILES "/Users/shyoon11/matlab/ros/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/maci64/src/decawave_uwb/m/" FILES_MATCHING REGEX "/[^/]*\\.m$")
endif()

