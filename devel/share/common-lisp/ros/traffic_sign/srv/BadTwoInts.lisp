; Auto-generated. Do not edit!


(cl:in-package traffic_sign-srv)


;//! \htmlinclude BadTwoInts-request.msg.html

(cl:defclass <BadTwoInts-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass BadTwoInts-request (<BadTwoInts-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <BadTwoInts-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'BadTwoInts-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name traffic_sign-srv:<BadTwoInts-request> is deprecated: use traffic_sign-srv:BadTwoInts-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <BadTwoInts-request>) ostream)
  "Serializes a message object of type '<BadTwoInts-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <BadTwoInts-request>) istream)
  "Deserializes a message object of type '<BadTwoInts-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<BadTwoInts-request>)))
  "Returns string type for a service object of type '<BadTwoInts-request>"
  "traffic_sign/BadTwoIntsRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'BadTwoInts-request)))
  "Returns string type for a service object of type 'BadTwoInts-request"
  "traffic_sign/BadTwoIntsRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<BadTwoInts-request>)))
  "Returns md5sum for a message object of type '<BadTwoInts-request>"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'BadTwoInts-request)))
  "Returns md5sum for a message object of type 'BadTwoInts-request"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<BadTwoInts-request>)))
  "Returns full string definition for message of type '<BadTwoInts-request>"
  (cl:format cl:nil "~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'BadTwoInts-request)))
  "Returns full string definition for message of type 'BadTwoInts-request"
  (cl:format cl:nil "~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <BadTwoInts-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <BadTwoInts-request>))
  "Converts a ROS message object to a list"
  (cl:list 'BadTwoInts-request
))
;//! \htmlinclude BadTwoInts-response.msg.html

(cl:defclass <BadTwoInts-response> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass BadTwoInts-response (<BadTwoInts-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <BadTwoInts-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'BadTwoInts-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name traffic_sign-srv:<BadTwoInts-response> is deprecated: use traffic_sign-srv:BadTwoInts-response instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <BadTwoInts-response>) ostream)
  "Serializes a message object of type '<BadTwoInts-response>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <BadTwoInts-response>) istream)
  "Deserializes a message object of type '<BadTwoInts-response>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<BadTwoInts-response>)))
  "Returns string type for a service object of type '<BadTwoInts-response>"
  "traffic_sign/BadTwoIntsResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'BadTwoInts-response)))
  "Returns string type for a service object of type 'BadTwoInts-response"
  "traffic_sign/BadTwoIntsResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<BadTwoInts-response>)))
  "Returns md5sum for a message object of type '<BadTwoInts-response>"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'BadTwoInts-response)))
  "Returns md5sum for a message object of type 'BadTwoInts-response"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<BadTwoInts-response>)))
  "Returns full string definition for message of type '<BadTwoInts-response>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'BadTwoInts-response)))
  "Returns full string definition for message of type 'BadTwoInts-response"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <BadTwoInts-response>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <BadTwoInts-response>))
  "Converts a ROS message object to a list"
  (cl:list 'BadTwoInts-response
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'BadTwoInts)))
  'BadTwoInts-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'BadTwoInts)))
  'BadTwoInts-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'BadTwoInts)))
  "Returns string type for a service object of type '<BadTwoInts>"
  "traffic_sign/BadTwoInts")