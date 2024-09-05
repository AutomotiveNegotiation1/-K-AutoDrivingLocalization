# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "localizer_zed_f9r: 1 messages, 2 services")

set(MSG_I_FLAGS "-Ilocalizer_zed_f9r:/home/keti/catkin_ws/src/ublox_f9r/msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(localizer_zed_f9r_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg" NAME_WE)
add_custom_target(_localizer_zed_f9r_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "localizer_zed_f9r" "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv" NAME_WE)
add_custom_target(_localizer_zed_f9r_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "localizer_zed_f9r" "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv" ""
)

get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv" NAME_WE)
add_custom_target(_localizer_zed_f9r_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "localizer_zed_f9r" "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Services
_generate_srv_cpp(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/localizer_zed_f9r
)
_generate_srv_cpp(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Module File
_generate_module_cpp(localizer_zed_f9r
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/localizer_zed_f9r
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(localizer_zed_f9r_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(localizer_zed_f9r_generate_messages localizer_zed_f9r_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_cpp _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_cpp _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_cpp _localizer_zed_f9r_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(localizer_zed_f9r_gencpp)
add_dependencies(localizer_zed_f9r_gencpp localizer_zed_f9r_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS localizer_zed_f9r_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Services
_generate_srv_eus(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/localizer_zed_f9r
)
_generate_srv_eus(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Module File
_generate_module_eus(localizer_zed_f9r
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/localizer_zed_f9r
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(localizer_zed_f9r_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(localizer_zed_f9r_generate_messages localizer_zed_f9r_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_eus _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_eus _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_eus _localizer_zed_f9r_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(localizer_zed_f9r_geneus)
add_dependencies(localizer_zed_f9r_geneus localizer_zed_f9r_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS localizer_zed_f9r_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Services
_generate_srv_lisp(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/localizer_zed_f9r
)
_generate_srv_lisp(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Module File
_generate_module_lisp(localizer_zed_f9r
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/localizer_zed_f9r
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(localizer_zed_f9r_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(localizer_zed_f9r_generate_messages localizer_zed_f9r_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_lisp _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_lisp _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_lisp _localizer_zed_f9r_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(localizer_zed_f9r_genlisp)
add_dependencies(localizer_zed_f9r_genlisp localizer_zed_f9r_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS localizer_zed_f9r_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Services
_generate_srv_nodejs(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/localizer_zed_f9r
)
_generate_srv_nodejs(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Module File
_generate_module_nodejs(localizer_zed_f9r
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/localizer_zed_f9r
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(localizer_zed_f9r_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(localizer_zed_f9r_generate_messages localizer_zed_f9r_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_nodejs _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_nodejs _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_nodejs _localizer_zed_f9r_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(localizer_zed_f9r_gennodejs)
add_dependencies(localizer_zed_f9r_gennodejs localizer_zed_f9r_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS localizer_zed_f9r_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Services
_generate_srv_py(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/localizer_zed_f9r
)
_generate_srv_py(localizer_zed_f9r
  "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/localizer_zed_f9r
)

### Generating Module File
_generate_module_py(localizer_zed_f9r
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/localizer_zed_f9r
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(localizer_zed_f9r_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(localizer_zed_f9r_generate_messages localizer_zed_f9r_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/msg/GNSS.msg" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_py _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Acc.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_py _localizer_zed_f9r_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/keti/catkin_ws/src/ublox_f9r/srv/Gyro.srv" NAME_WE)
add_dependencies(localizer_zed_f9r_generate_messages_py _localizer_zed_f9r_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(localizer_zed_f9r_genpy)
add_dependencies(localizer_zed_f9r_genpy localizer_zed_f9r_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS localizer_zed_f9r_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/localizer_zed_f9r)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/localizer_zed_f9r
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(localizer_zed_f9r_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/localizer_zed_f9r)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/localizer_zed_f9r
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(localizer_zed_f9r_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/localizer_zed_f9r)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/localizer_zed_f9r
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(localizer_zed_f9r_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/localizer_zed_f9r)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/localizer_zed_f9r
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(localizer_zed_f9r_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/localizer_zed_f9r)
  install(CODE "execute_process(COMMAND \"/usr/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/localizer_zed_f9r\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/localizer_zed_f9r
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(localizer_zed_f9r_generate_messages_py std_msgs_generate_messages_py)
endif()
