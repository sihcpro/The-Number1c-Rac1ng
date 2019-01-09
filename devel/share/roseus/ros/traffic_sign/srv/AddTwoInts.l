;; Auto-generated. Do not edit!


(when (boundp 'traffic_sign::AddTwoInts)
  (if (not (find-package "TRAFFIC_SIGN"))
    (make-package "TRAFFIC_SIGN"))
  (shadow 'AddTwoInts (find-package "TRAFFIC_SIGN")))
(unless (find-package "TRAFFIC_SIGN::ADDTWOINTS")
  (make-package "TRAFFIC_SIGN::ADDTWOINTS"))
(unless (find-package "TRAFFIC_SIGN::ADDTWOINTSREQUEST")
  (make-package "TRAFFIC_SIGN::ADDTWOINTSREQUEST"))
(unless (find-package "TRAFFIC_SIGN::ADDTWOINTSRESPONSE")
  (make-package "TRAFFIC_SIGN::ADDTWOINTSRESPONSE"))

(in-package "ROS")





(defclass traffic_sign::AddTwoIntsRequest
  :super ros::object
  :slots ())

(defmethod traffic_sign::AddTwoIntsRequest
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass traffic_sign::AddTwoIntsResponse
  :super ros::object
  :slots ())

(defmethod traffic_sign::AddTwoIntsResponse
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass traffic_sign::AddTwoInts
  :super ros::object
  :slots ())

(setf (get traffic_sign::AddTwoInts :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get traffic_sign::AddTwoInts :datatype-) "traffic_sign/AddTwoInts")
(setf (get traffic_sign::AddTwoInts :request) traffic_sign::AddTwoIntsRequest)
(setf (get traffic_sign::AddTwoInts :response) traffic_sign::AddTwoIntsResponse)

(defmethod traffic_sign::AddTwoIntsRequest
  (:response () (instance traffic_sign::AddTwoIntsResponse :init)))

(setf (get traffic_sign::AddTwoIntsRequest :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get traffic_sign::AddTwoIntsRequest :datatype-) "traffic_sign/AddTwoIntsRequest")
(setf (get traffic_sign::AddTwoIntsRequest :definition-)
      "
---
")

(setf (get traffic_sign::AddTwoIntsResponse :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get traffic_sign::AddTwoIntsResponse :datatype-) "traffic_sign/AddTwoIntsResponse")
(setf (get traffic_sign::AddTwoIntsResponse :definition-)
      "
---
")



(provide :traffic_sign/AddTwoInts "d41d8cd98f00b204e9800998ecf8427e")


