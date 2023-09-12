cmake_minimum_required(VERSION 3.0.2)
project(run)

## Compile as C++11
add_compile_options(-std=c++11)

# Find catkin and required ROS components
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  rosbag
  message_generation
)

# Find Qt5
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

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

# Specify include directories
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/run  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/run/kappublics/kaptypes  # Add this line
  ${CMAKE_CURRENT_SOURCE_DIR}/include/run/messagesubscibers
  # ${CMAKE_CURRENT_SOURCE_DIR}/lib/UWBpos2
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/UWBpos6
  ${catkin_INCLUDE_DIRS}
)


# Declare C++ executable
set(SOURCES
  src/main2.cpp
  src/mainwindow.cpp
  src/kapcallback.cpp  # 추가
)

# add_executable(${PROJECT_NAME}_node ${SOURCES} ${UWB_SOURCES2} ${UWB_SOURCES6} ${UWB_HEADERS})
add_executable(${PROJECT_NAME}_node ${SOURCES} ${UWB_SOURCES6} ${UWB_HEADERS})

# Add dependencies (make sure message headers are generated before building the target)
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Link against libraries
target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::PrintSupport
)
