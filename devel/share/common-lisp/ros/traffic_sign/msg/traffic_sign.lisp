; Auto-generated. Do not edit!


(cl:in-package traffic_sign-msg)


;//! \htmlinclude traffic_sign.msg.html

(cl:defclass <traffic_sign> (roslisp-msg-protocol:ros-message)
  ((robot_igite_academy
    :reader robot_igite_academy
    :initarg :robot_igite_academy
    :type cl:float
    :initform 0.0))
)

(cl:defclass traffic_sign (<traffic_sign>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <traffic_sign>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'traffic_sign)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name traffic_sign-msg:<traffic_sign> is deprecated: use traffic_sign-msg:traffic_sign instead.")))

(cl:ensure-generic-function 'robot_igite_academy-val :lambda-list '(m))
(cl:defmethod robot_igite_academy-val ((m <traffic_sign>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traffic_sign-msg:robot_igite_academy-val is deprecated.  Use traffic_sign-msg:robot_igite_academy instead.")
  (robot_igite_academy m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <traffic_sign>) ostream)
  "Serializes a message object of type '<traffic_sign>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'robot_igite_academy))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <traffic_sign>) istream)
  "Deserializes a message object of type '<traffic_sign>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'robot_igite_academy) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<traffic_sign>)))
  "Returns string type for a message object of type '<traffic_sign>"
  "traffic_sign/traffic_sign")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'traffic_sign)))
  "Returns string type for a message object of type 'traffic_sign"
  "traffic_sign/traffic_sign")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<traffic_sign>)))
  "Returns md5sum for a message object of type '<traffic_sign>"
  "069c9e64d21688e60165de9024f3f89c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'traffic_sign)))
  "Returns md5sum for a message object of type 'traffic_sign"
  "069c9e64d21688e60165de9024f3f89c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<traffic_sign>)))
  "Returns full string definition for message of type '<traffic_sign>"
  (cl:format cl:nil "float32 robot_igite_academy~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'traffic_sign)))
  "Returns full string definition for message of type 'traffic_sign"
  (cl:format cl:nil "float32 robot_igite_academy~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <traffic_sign>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <traffic_sign>))
  "Converts a ROS message object to a list"
  (cl:list 'traffic_sign
    (cl:cons ':robot_igite_academy (robot_igite_academy msg))
))
