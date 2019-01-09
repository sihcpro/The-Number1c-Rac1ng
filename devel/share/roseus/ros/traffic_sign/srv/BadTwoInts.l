;; Auto-generated. Do not edit!


(when (boundp 'traffic_sign::BadTwoInts)
  (if (not (find-package "TRAFFIC_SIGN"))
    (make-package "TRAFFIC_SIGN"))
  (shadow 'BadTwoInts (find-package "TRAFFIC_SIGN")))
(unless (find-package "TRAFFIC_SIGN::BADTWOINTS")
  (make-package "TRAFFIC_SIGN::BADTWOINTS"))
(unless (find-package "TRAFFIC_SIGN::BADTWOINTSREQUEST")
  (make-package "TRAFFIC_SIGN::BADTWOINTSREQUEST"))
(unless (find-package "TRAFFIC_SIGN::BADTWOINTSRESPONSE")
  (make-package "TRAFFIC_SIGN::BADTWOINTSRESPONSE"))

(in-package "ROS")





(defclass traffic_sign::BadTwoIntsRequest
  :super ros::object
  :slots ())

(defmethod traffic_sign::BadTwoIntsRequest
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

(defclass traffic_sign::BadTwoIntsResponse
  :super ros::object
  :slots ())

(defmethod traffic_sign::BadTwoIntsResponse
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

(defclass traffic_sign::BadTwoInts
  :super ros::object
  :slots ())

(setf (get traffic_sign::BadTwoInts :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get traffic_sign::BadTwoInts :datatype-) "traffic_sign/BadTwoInts")
(setf (get traffic_sign::BadTwoInts :request) traffic_sign::BadTwoIntsRequest)
(setf (get traffic_sign::BadTwoInts :response) traffic_sign::BadTwoIntsResponse)

(defmethod traffic_sign::BadTwoIntsRequest
  (:response () (instance traffic_sign::BadTwoIntsResponse :init)))

(setf (get traffic_sign::BadTwoIntsRequest :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get traffic_sign::BadTwoIntsRequest :datatype-) "traffic_sign/BadTwoIntsRequest")
(setf (get traffic_sign::BadTwoIntsRequest :definition-)
      "
---
")

(setf (get traffic_sign::BadTwoIntsResponse :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get traffic_sign::BadTwoIntsResponse :datatype-) "traffic_sign/BadTwoIntsResponse")
(setf (get traffic_sign::BadTwoIntsResponse :definition-)
      "
---
")



(provide :traffic_sign/BadTwoInts "d41d8cd98f00b204e9800998ecf8427e")


