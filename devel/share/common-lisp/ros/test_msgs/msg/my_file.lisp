; Auto-generated. Do not edit!


(cl:in-package test_msgs-msg)


;//! \htmlinclude my_file.msg.html

(cl:defclass <my_file> (roslisp-msg-protocol:ros-message)
  ((robot_igite_academy
    :reader robot_igite_academy
    :initarg :robot_igite_academy
    :type cl:float
    :initform 0.0))
)

(cl:defclass my_file (<my_file>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <my_file>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'my_file)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name test_msgs-msg:<my_file> is deprecated: use test_msgs-msg:my_file instead.")))

(cl:ensure-generic-function 'robot_igite_academy-val :lambda-list '(m))
(cl:defmethod robot_igite_academy-val ((m <my_file>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader test_msgs-msg:robot_igite_academy-val is deprecated.  Use test_msgs-msg:robot_igite_academy instead.")
  (robot_igite_academy m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <my_file>) ostream)
  "Serializes a message object of type '<my_file>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'robot_igite_academy))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <my_file>) istream)
  "Deserializes a message object of type '<my_file>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'robot_igite_academy) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<my_file>)))
  "Returns string type for a message object of type '<my_file>"
  "test_msgs/my_file")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'my_file)))
  "Returns string type for a message object of type 'my_file"
  "test_msgs/my_file")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<my_file>)))
  "Returns md5sum for a message object of type '<my_file>"
  "069c9e64d21688e60165de9024f3f89c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'my_file)))
  "Returns md5sum for a message object of type 'my_file"
  "069c9e64d21688e60165de9024f3f89c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<my_file>)))
  "Returns full string definition for message of type '<my_file>"
  (cl:format cl:nil "float32 robot_igite_academy~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'my_file)))
  "Returns full string definition for message of type 'my_file"
  (cl:format cl:nil "float32 robot_igite_academy~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <my_file>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <my_file>))
  "Converts a ROS message object to a list"
  (cl:list 'my_file
    (cl:cons ':robot_igite_academy (robot_igite_academy msg))
))
