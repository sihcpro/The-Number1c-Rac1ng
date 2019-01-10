; Auto-generated. Do not edit!


(cl:in-package Team503-msg)


;//! \htmlinclude traffic_sign_msg.msg.html

(cl:defclass <traffic_sign_msg> (roslisp-msg-protocol:ros-message)
  ((traffic_sign_type
    :reader traffic_sign_type
    :initarg :traffic_sign_type
    :type cl:integer
    :initform 0))
)

(cl:defclass traffic_sign_msg (<traffic_sign_msg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <traffic_sign_msg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'traffic_sign_msg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name Team503-msg:<traffic_sign_msg> is deprecated: use Team503-msg:traffic_sign_msg instead.")))

(cl:ensure-generic-function 'traffic_sign_type-val :lambda-list '(m))
(cl:defmethod traffic_sign_type-val ((m <traffic_sign_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Team503-msg:traffic_sign_type-val is deprecated.  Use Team503-msg:traffic_sign_type instead.")
  (traffic_sign_type m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <traffic_sign_msg>) ostream)
  "Serializes a message object of type '<traffic_sign_msg>"
  (cl:let* ((signed (cl:slot-value msg 'traffic_sign_type)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <traffic_sign_msg>) istream)
  "Deserializes a message object of type '<traffic_sign_msg>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'traffic_sign_type) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<traffic_sign_msg>)))
  "Returns string type for a message object of type '<traffic_sign_msg>"
  "Team503/traffic_sign_msg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'traffic_sign_msg)))
  "Returns string type for a message object of type 'traffic_sign_msg"
  "Team503/traffic_sign_msg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<traffic_sign_msg>)))
  "Returns md5sum for a message object of type '<traffic_sign_msg>"
  "b39d3b00117d7861701b14c6e1e0db30")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'traffic_sign_msg)))
  "Returns md5sum for a message object of type 'traffic_sign_msg"
  "b39d3b00117d7861701b14c6e1e0db30")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<traffic_sign_msg>)))
  "Returns full string definition for message of type '<traffic_sign_msg>"
  (cl:format cl:nil "int64 traffic_sign_type~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'traffic_sign_msg)))
  "Returns full string definition for message of type 'traffic_sign_msg"
  (cl:format cl:nil "int64 traffic_sign_type~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <traffic_sign_msg>))
  (cl:+ 0
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <traffic_sign_msg>))
  "Converts a ROS message object to a list"
  (cl:list 'traffic_sign_msg
    (cl:cons ':traffic_sign_type (traffic_sign_type msg))
))
