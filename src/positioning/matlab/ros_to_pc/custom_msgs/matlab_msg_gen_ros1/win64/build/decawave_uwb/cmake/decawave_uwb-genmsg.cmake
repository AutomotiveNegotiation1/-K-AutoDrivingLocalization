# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "decawave_uwb: 1 messages, 0 services")

set(MSG_I_FLAGS "-Idecawave_uwb:Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/msg;-Istd_msgs:C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/std_msgs/cmake/../msg;-Igeometry_msgs:C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/geometry_msgs/cmake/../msg;-Istd_msgs:C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(decawave_uwb_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/msg/uwb_anchor.msg" NAME_WE)
add_custom_target(_decawave_uwb_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "decawave_uwb" "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/msg/uwb_anchor.msg" "geometry_msgs/Pose:geometry_msgs/Quaternion:std_msgs/Header:geometry_msgs/Point"
)

#
#  langs = gencpp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(decawave_uwb
  "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/msg/uwb_anchor.msg"
  "${MSG_I_FLAGS}"
  "C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/geometry_msgs/cmake/../msg/Pose.msg;C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/geometry_msgs/cmake/../msg/Quaternion.msg;C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/std_msgs/cmake/../msg/Header.msg;C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/decawave_uwb
)

### Generating Services

### Generating Module File
_generate_module_cpp(decawave_uwb
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/decawave_uwb
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(decawave_uwb_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(decawave_uwb_generate_messages decawave_uwb_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/msg/uwb_anchor.msg" NAME_WE)
add_dependencies(decawave_uwb_generate_messages_cpp _decawave_uwb_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(decawave_uwb_gencpp)
add_dependencies(decawave_uwb_gencpp decawave_uwb_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS decawave_uwb_generate_messages_cpp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(decawave_uwb
  "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/msg/uwb_anchor.msg"
  "${MSG_I_FLAGS}"
  "C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/geometry_msgs/cmake/../msg/Pose.msg;C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/geometry_msgs/cmake/../msg/Quaternion.msg;C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/std_msgs/cmake/../msg/Header.msg;C:/Program Files/MATLAB/R2022b/sys/ros1/win64/ros1/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/decawave_uwb
)

### Generating Services

### Generating Module File
_generate_module_py(decawave_uwb
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/decawave_uwb
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(decawave_uwb_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(decawave_uwb_generate_messages decawave_uwb_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "Z:/work/-K-AutoDrivingLocalization/src/positioning/matlab/ros_to_pc/custom_msgs/matlab_msg_gen_ros1/win64/src/decawave_uwb/msg/uwb_anchor.msg" NAME_WE)
add_dependencies(decawave_uwb_generate_messages_py _decawave_uwb_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(decawave_uwb_genpy)
add_dependencies(decawave_uwb_genpy decawave_uwb_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS decawave_uwb_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/decawave_uwb)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/decawave_uwb
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(decawave_uwb_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(decawave_uwb_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(decawave_uwb_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/decawave_uwb)
  install(CODE "execute_process(COMMAND \"C:/Users/USER/AppData/Roaming/MathWorks/MATLAB/R2022b/ros1/win64/venv/Scripts/python.exe\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/decawave_uwb\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/decawave_uwb
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(decawave_uwb_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(decawave_uwb_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(decawave_uwb_generate_messages_py std_msgs_generate_messages_py)
endif()
