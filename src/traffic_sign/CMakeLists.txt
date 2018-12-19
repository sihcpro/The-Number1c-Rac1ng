cmake_minimum_required(VERSION 2.8.3)
project(traffic_sign)

find_package(catkin REQUIRED COMPONENTS
  traffic_sign
  rospy
  std_msgs
  cv_bridge
  image_transport
  sensor_msgs
)

catkin_python_setup()

add_message_files(
  FILES  # e.g. Floats.msg HeaderString.msg
)
add_service_files(
  DIRECTORY srv 
  FILES AddTwoInts.srv BadTwoInts.srv
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

traffic_sign()







catkin_install_python(PROGRAMS
  src/main.py
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/traffic_sign
)
install(FILES
  launch/traffic_sign.launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/traffic_sign
)
