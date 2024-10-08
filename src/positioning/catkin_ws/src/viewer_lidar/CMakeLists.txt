cmake_minimum_required(VERSION 3.10)

project(viewer_lidar VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosbag
  std_msgs
  message_generation
)
find_package(PkgConfig)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES viewer_lidar
  CATKIN_DEPENDS roscpp std_msgs
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${GTK3_INCLUDE_DIRS})
add_compile_options(${GTK3_CFLAGS_OTHER})
add_library(${PROJECT_NAME}
  src/resource.c
)

target_include_directories(${PROJECT_NAME} PUBLIC )
set(SOURCES src/viewer_lidar_main.cpp src/resource.c src/viewer_lidar.c)
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(viewer_lidar_node src/viewer_lidar.c src/viewer_lidar_main.cpp src/resource.c)
add_dependencies(viewer_lidar_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME})
target_link_options(viewer_lidar_node PRIVATE -rdynamic)

target_link_libraries(viewer_lidar_node ${PROJECT_NAME} ${catkin_LIBRARIES} ${GTK3_LIBRARIES} m)
