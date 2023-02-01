# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "ublox_f9r: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iublox_f9r:Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/ublox_f9r/msg;-Istd_msgs:C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/std_msgs/cmake/../msg;-Istd_msgs:C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(ublox_f9r_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/ublox_f9r/msg/gnssPVT.msg" NAME_WE)
add_custom_target(_ublox_f9r_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "ublox_f9r" "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/ublox_f9r/msg/gnssPVT.msg" "std_msgs/Header"
)

#
#  langs = gencpp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(ublox_f9r
  "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/ublox_f9r/msg/gnssPVT.msg"
  "${MSG_I_FLAGS}"
  "C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ublox_f9r
)

### Generating Services

### Generating Module File
_generate_module_cpp(ublox_f9r
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ublox_f9r
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(ublox_f9r_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(ublox_f9r_generate_messages ublox_f9r_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/ublox_f9r/msg/gnssPVT.msg" NAME_WE)
add_dependencies(ublox_f9r_generate_messages_cpp _ublox_f9r_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ublox_f9r_gencpp)
add_dependencies(ublox_f9r_gencpp ublox_f9r_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ublox_f9r_generate_messages_cpp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(ublox_f9r
  "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/ublox_f9r/msg/gnssPVT.msg"
  "${MSG_I_FLAGS}"
  "C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ublox_f9r
)

### Generating Services

### Generating Module File
_generate_module_py(ublox_f9r
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ublox_f9r
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(ublox_f9r_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(ublox_f9r_generate_messages ublox_f9r_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/ublox_f9r/msg/gnssPVT.msg" NAME_WE)
add_dependencies(ublox_f9r_generate_messages_py _ublox_f9r_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(ublox_f9r_genpy)
add_dependencies(ublox_f9r_genpy ublox_f9r_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS ublox_f9r_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ublox_f9r)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/ublox_f9r
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(ublox_f9r_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(ublox_f9r_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ublox_f9r)
  install(CODE "execute_process(COMMAND \"C:/Users/USER/AppData/Roaming/MathWorks/MATLAB/R2022b/ros1/win64/venv/Scripts/python.exe\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ublox_f9r\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/ublox_f9r
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(ublox_f9r_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(ublox_f9r_generate_messages_py std_msgs_generate_messages_py)
endif()
