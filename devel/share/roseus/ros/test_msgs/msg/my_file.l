;; Auto-generated. Do not edit!


(when (boundp 'test_msgs::my_file)
  (if (not (find-package "TEST_MSGS"))
    (make-package "TEST_MSGS"))
  (shadow 'my_file (find-package "TEST_MSGS")))
(unless (find-package "TEST_MSGS::MY_FILE")
  (make-package "TEST_MSGS::MY_FILE"))

(in-package "ROS")
;;//! \htmlinclude my_file.msg.html


(defclass test_msgs::my_file
  :super ros::object
  :slots (_robot_igite_academy ))

(defmethod test_msgs::my_file
  (:init
   (&key
    ((:robot_igite_academy __robot_igite_academy) 0.0)
    )
   (send-super :init)
   (setq _robot_igite_academy (float __robot_igite_academy))
   self)
  (:robot_igite_academy
   (&optional __robot_igite_academy)
   (if __robot_igite_academy (setq _robot_igite_academy __robot_igite_academy)) _robot_igite_academy)
  (:serialization-length
   ()
   (+
    ;; float32 _robot_igite_academy
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _robot_igite_academy
       (sys::poke _robot_igite_academy (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _robot_igite_academy
     (setq _robot_igite_academy (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get test_msgs::my_file :md5sum-) "069c9e64d21688e60165de9024f3f89c")
(setf (get test_msgs::my_file :datatype-) "test_msgs/my_file")
(setf (get test_msgs::my_file :definition-)
      "float32 robot_igite_academy

")



(provide :test_msgs/my_file "069c9e64d21688e60165de9024f3f89c")


