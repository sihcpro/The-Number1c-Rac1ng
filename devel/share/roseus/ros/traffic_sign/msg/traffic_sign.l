;; Auto-generated. Do not edit!


(when (boundp 'traffic_sign::traffic_sign)
  (if (not (find-package "TRAFFIC_SIGN"))
    (make-package "TRAFFIC_SIGN"))
  (shadow 'traffic_sign (find-package "TRAFFIC_SIGN")))
(unless (find-package "TRAFFIC_SIGN::TRAFFIC_SIGN")
  (make-package "TRAFFIC_SIGN::TRAFFIC_SIGN"))

(in-package "ROS")
;;//! \htmlinclude traffic_sign.msg.html


(defclass traffic_sign::traffic_sign
  :super ros::object
  :slots (_robot_igite_academy ))

(defmethod traffic_sign::traffic_sign
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

(setf (get traffic_sign::traffic_sign :md5sum-) "069c9e64d21688e60165de9024f3f89c")
(setf (get traffic_sign::traffic_sign :datatype-) "traffic_sign/traffic_sign")
(setf (get traffic_sign::traffic_sign :definition-)
      "float32 robot_igite_academy

")



(provide :traffic_sign/traffic_sign "069c9e64d21688e60165de9024f3f89c")


