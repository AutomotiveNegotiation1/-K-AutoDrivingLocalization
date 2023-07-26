; Auto-generated. Do not edit!


(cl:in-package localizer_zed_f9r-srv)


;//! \htmlinclude Acc-request.msg.html

(cl:defclass <Acc-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass Acc-request (<Acc-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Acc-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Acc-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name localizer_zed_f9r-srv:<Acc-request> is deprecated: use localizer_zed_f9r-srv:Acc-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Acc-request>) ostream)
  "Serializes a message object of type '<Acc-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Acc-request>) istream)
  "Deserializes a message object of type '<Acc-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Acc-request>)))
  "Returns string type for a service object of type '<Acc-request>"
  "localizer_zed_f9r/AccRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Acc-request)))
  "Returns string type for a service object of type 'Acc-request"
  "localizer_zed_f9r/AccRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Acc-request>)))
  "Returns md5sum for a message object of type '<Acc-request>"
  "4a842b65f413084dc2b10fb484ea7f17")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Acc-request)))
  "Returns md5sum for a message object of type 'Acc-request"
  "4a842b65f413084dc2b10fb484ea7f17")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Acc-request>)))
  "Returns full string definition for message of type '<Acc-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Acc-request)))
  "Returns full string definition for message of type 'Acc-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Acc-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Acc-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Acc-request
))
;//! \htmlinclude Acc-response.msg.html

(cl:defclass <Acc-response> (roslisp-msg-protocol:ros-message)
  ((x
    :reader x
    :initarg :x
    :type cl:float
    :initform 0.0)
   (y
    :reader y
    :initarg :y
    :type cl:float
    :initform 0.0)
   (z
    :reader z
    :initarg :z
    :type cl:float
    :initform 0.0))
)

(cl:defclass Acc-response (<Acc-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Acc-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Acc-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name localizer_zed_f9r-srv:<Acc-response> is deprecated: use localizer_zed_f9r-srv:Acc-response instead.")))

(cl:ensure-generic-function 'x-val :lambda-list '(m))
(cl:defmethod x-val ((m <Acc-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader localizer_zed_f9r-srv:x-val is deprecated.  Use localizer_zed_f9r-srv:x instead.")
  (x m))

(cl:ensure-generic-function 'y-val :lambda-list '(m))
(cl:defmethod y-val ((m <Acc-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader localizer_zed_f9r-srv:y-val is deprecated.  Use localizer_zed_f9r-srv:y instead.")
  (y m))

(cl:ensure-generic-function 'z-val :lambda-list '(m))
(cl:defmethod z-val ((m <Acc-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader localizer_zed_f9r-srv:z-val is deprecated.  Use localizer_zed_f9r-srv:z instead.")
  (z m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Acc-response>) ostream)
  "Serializes a message object of type '<Acc-response>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'z))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Acc-response>) istream)
  "Deserializes a message object of type '<Acc-response>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'z) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Acc-response>)))
  "Returns string type for a service object of type '<Acc-response>"
  "localizer_zed_f9r/AccResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Acc-response)))
  "Returns string type for a service object of type 'Acc-response"
  "localizer_zed_f9r/AccResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Acc-response>)))
  "Returns md5sum for a message object of type '<Acc-response>"
  "4a842b65f413084dc2b10fb484ea7f17")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Acc-response)))
  "Returns md5sum for a message object of type 'Acc-response"
  "4a842b65f413084dc2b10fb484ea7f17")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Acc-response>)))
  "Returns full string definition for message of type '<Acc-response>"
  (cl:format cl:nil "float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Acc-response)))
  "Returns full string definition for message of type 'Acc-response"
  (cl:format cl:nil "float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Acc-response>))
  (cl:+ 0
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Acc-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Acc-response
    (cl:cons ':x (x msg))
    (cl:cons ':y (y msg))
    (cl:cons ':z (z msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Acc)))
  'Acc-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Acc)))
  'Acc-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Acc)))
  "Returns string type for a service object of type '<Acc>"
  "localizer_zed_f9r/Acc")