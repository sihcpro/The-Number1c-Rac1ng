execute_process(COMMAND "/home/sihc/catkin_ws/build/traffic_sign/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/sihc/catkin_ws/build/traffic_sign/catkin_generated/python_distutils_install.sh) returned error code ")
endif()