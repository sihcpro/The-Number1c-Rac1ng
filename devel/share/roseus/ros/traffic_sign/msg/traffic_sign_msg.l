;; Auto-generated. Do not edit!


(when (boundp 'traffic_sign::traffic_sign_msg)
  (if (not (find-package "TRAFFIC_SIGN"))
    (make-package "TRAFFIC_SIGN"))
  (shadow 'traffic_sign_msg (find-package "TRAFFIC_SIGN")))
(unless (find-package "TRAFFIC_SIGN::TRAFFIC_SIGN_MSG")
  (make-package "TRAFFIC_SIGN::TRAFFIC_SIGN_MSG"))

(in-package "ROS")
;;//! \htmlinclude traffic_sign_msg.msg.html


(defclass traffic_sign::traffic_sign_msg
  :super ros::object
  :slots (_traffic_sign_type ))

(defmethod traffic_sign::traffic_sign_msg
  (:init
   (&key
    ((:traffic_sign_type __traffic_sign_type) 0)
    )
   (send-super :init)
   (setq _traffic_sign_type (round __traffic_sign_type))
   self)
  (:traffic_sign_type
   (&optional __traffic_sign_type)
   (if __traffic_sign_type (setq _traffic_sign_type __traffic_sign_type)) _traffic_sign_type)
  (:serialization-length
   ()
   (+
    ;; int64 _traffic_sign_type
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int64 _traffic_sign_type
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _traffic_sign_type (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _traffic_sign_type) (= (length (_traffic_sign_type . bv)) 2)) ;; bignum
              (write-long (ash (elt (_traffic_sign_type . bv) 0) 0) s)
              (write-long (ash (elt (_traffic_sign_type . bv) 1) -1) s))
             ((and (class _traffic_sign_type) (= (length (_traffic_sign_type . bv)) 1)) ;; big1
              (write-long (elt (_traffic_sign_type . bv) 0) s)
              (write-long (if (>= _traffic_sign_type 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _traffic_sign_type s)(write-long (if (>= _traffic_sign_type 0) 0 #xffffffff) s)))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int64 _traffic_sign_type
#+(or :alpha :irix6 :x86_64)
      (setf _traffic_sign_type (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _traffic_sign_type (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;;
   self)
  )

(setf (get traffic_sign::traffic_sign_msg :md5sum-) "b39d3b00117d7861701b14c6e1e0db30")
(setf (get traffic_sign::traffic_sign_msg :datatype-) "traffic_sign/traffic_sign_msg")
(setf (get traffic_sign::traffic_sign_msg :definition-)
      "int64 traffic_sign_type

")



(provide :traffic_sign/traffic_sign_msg "b39d3b00117d7861701b14c6e1e0db30")


