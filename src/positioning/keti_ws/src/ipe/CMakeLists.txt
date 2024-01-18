cmake_minimum_required(VERSION 3.0.2)
project(ipe)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosbag
  std_msgs
  message_generation
)

## Add message files
add_message_files(
  FILES
  Anchor.msg
)

## Generate messages
generate_messages(
  DEPENDENCIES
  std_msgs
)

## Declare a catkin package
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ipe
  CATKIN_DEPENDS roscpp std_msgs
)

## Specify additional locations of header files
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  lib/IPA  # Include IPA library headers
)

## Declare a C++ library
file(GLOB IPA_SOURCES "lib/IPA/*.cpp")  # Automatically include all cpp files in lib/IPA
add_library(${PROJECT_NAME}
  src/ipeinterface.cpp
  src/imusubscriber.cpp
  src/ipecallback.cpp
  src/main.cpp
  src/socketmanager.cpp
  src/uwbsubscriber.cpp
  ${IPA_SOURCES}  # Add the IPA library sources
)

## Add cmake target dependencies of the library
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable for each node
add_executable(${PROJECT_NAME}_node src/main.cpp)

## Add cmake target dependencies of the executable
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_node
${PROJECT_NAME}
${catkin_LIBRARIES}
)
