# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "Team503: 1 messages, 0 services")

set(MSG_I_FLAGS "-ITeam503:/home/sihc/catkin_ws/src/Team503/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators

add_custom_target(Team503_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/sihc/catkin_ws/src/Team503/msg/traffic_sign_msg.msg" NAME_WE)
add_custom_target(_Team503_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "Team503" "/home/sihc/catkin_ws/src/Team503/msg/traffic_sign_msg.msg" ""
)

#
#  langs = 
#


