cmake_minimum_required(VERSION 3.0.2)
project(visual)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  rosbag
  message_generation
)

find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Specify message files
add_message_files(
  FILES
  Anchor.msg
  Tag.msg
)

# Generate added messages
generate_messages(
  DEPENDENCIES
  std_msgs
)

# Declare catkin package and dependencies
catkin_package(
  CATKIN_DEPENDS roscpp std_msgs message_runtime
)

# file(GLOB UWB_SOURCES2 lib/UWBpos2/*.c)
file(GLOB UWB_SOURCES6 lib/UWBpos6/*.c)
file(GLOB IMU_SOURCES lib/IMUpos/*.c)
file(GLOB FUSION_SOURCES lib/fusion/*.c)
file(GLOB OBJECT_SOURCES include/visual/kuipmessage/messagesubscibers/*.cpp)

include_directories(
  include  # your custom include directory, if you have one
  ${CMAKE_CURRENT_SOURCE_DIR}/src  # this is to directly include the path to where qcustomplot.h resides
  ${catkin_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}  # this is for the ui_mainwindow.h file
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/visual  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/visual/kuippublics/kuiptypes  # Add this line
  ${CMAKE_CURRENT_SOURCE_DIR}/include/visual/kuippublics/kuipcontroller  # Add this line
  ${CMAKE_CURRENT_SOURCE_DIR}/include/visual/kuipmessage/messagesubscibers
  ${CMAKE_CURRENT_SOURCE_DIR}/include/visual/kuipmessage/messagecommon
  # ${CMAKE_CURRENT_SOURCE_DIR}/lib/UWBpos2
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/UWBpos6
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/IMUpos
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/fusion
)


set(SOURCES
  src/main.cpp
  src/mainwindow.cpp
  src/kuipinterface.cpp
  src/qcustomplot.cpp
  src/kuipcallback.cpp  # 추가
)

add_executable(${PROJECT_NAME}_node ${SOURCES} ${UWB_SOURCES6} ${IMU_SOURCES} ${FUSION_SOURCES} ${OBJECT_SOURCES})

add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Link against libraries
target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::PrintSupport
)

