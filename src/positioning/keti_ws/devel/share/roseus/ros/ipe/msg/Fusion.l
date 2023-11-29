;; Auto-generated. Do not edit!


(when (boundp 'ipe::Fusion)
  (if (not (find-package "IPE"))
    (make-package "IPE"))
  (shadow 'Fusion (find-package "IPE")))
(unless (find-package "IPE::FUSION")
  (make-package "IPE::FUSION"))

(in-package "ROS")
;;//! \htmlinclude Fusion.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass ipe::Fusion
  :super ros::object
  :slots (_header _kf_psi _gyro_psi _cent_pos_est _cent_vel_est _prevTagHeading _init_flag _prevTagPos _acc_b_phi ))

(defmethod ipe::Fusion
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:kf_psi __kf_psi) 0.0)
    ((:gyro_psi __gyro_psi) 0.0)
    ((:cent_pos_est __cent_pos_est) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:cent_vel_est __cent_vel_est) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:prevTagHeading __prevTagHeading) 0.0)
    ((:init_flag __init_flag) 0.0)
    ((:prevTagPos __prevTagPos) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:acc_b_phi __acc_b_phi) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _kf_psi (float __kf_psi))
   (setq _gyro_psi (float __gyro_psi))
   (setq _cent_pos_est __cent_pos_est)
   (setq _cent_vel_est __cent_vel_est)
   (setq _prevTagHeading (float __prevTagHeading))
   (setq _init_flag (float __init_flag))
   (setq _prevTagPos __prevTagPos)
   (setq _acc_b_phi (float __acc_b_phi))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:kf_psi
   (&optional __kf_psi)
   (if __kf_psi (setq _kf_psi __kf_psi)) _kf_psi)
  (:gyro_psi
   (&optional __gyro_psi)
   (if __gyro_psi (setq _gyro_psi __gyro_psi)) _gyro_psi)
  (:cent_pos_est
   (&optional __cent_pos_est)
   (if __cent_pos_est (setq _cent_pos_est __cent_pos_est)) _cent_pos_est)
  (:cent_vel_est
   (&optional __cent_vel_est)
   (if __cent_vel_est (setq _cent_vel_est __cent_vel_est)) _cent_vel_est)
  (:prevTagHeading
   (&optional __prevTagHeading)
   (if __prevTagHeading (setq _prevTagHeading __prevTagHeading)) _prevTagHeading)
  (:init_flag
   (&optional __init_flag)
   (if __init_flag (setq _init_flag __init_flag)) _init_flag)
  (:prevTagPos
   (&optional __prevTagPos)
   (if __prevTagPos (setq _prevTagPos __prevTagPos)) _prevTagPos)
  (:acc_b_phi
   (&optional __acc_b_phi)
   (if __acc_b_phi (setq _acc_b_phi __acc_b_phi)) _acc_b_phi)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64 _kf_psi
    8
    ;; float64 _gyro_psi
    8
    ;; float64[] _cent_pos_est
    (* 8    (length _cent_pos_est)) 4
    ;; float64[] _cent_vel_est
    (* 8    (length _cent_vel_est)) 4
    ;; float64 _prevTagHeading
    8
    ;; float64 _init_flag
    8
    ;; float64[] _prevTagPos
    (* 8    (length _prevTagPos)) 4
    ;; float64 _acc_b_phi
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float64 _kf_psi
       (sys::poke _kf_psi (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _gyro_psi
       (sys::poke _gyro_psi (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64[] _cent_pos_est
     (write-long (length _cent_pos_est) s)
     (dotimes (i (length _cent_pos_est))
       (sys::poke (elt _cent_pos_est i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _cent_vel_est
     (write-long (length _cent_vel_est) s)
     (dotimes (i (length _cent_vel_est))
       (sys::poke (elt _cent_vel_est i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64 _prevTagHeading
       (sys::poke _prevTagHeading (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _init_flag
       (sys::poke _init_flag (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64[] _prevTagPos
     (write-long (length _prevTagPos) s)
     (dotimes (i (length _prevTagPos))
       (sys::poke (elt _prevTagPos i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64 _acc_b_phi
       (sys::poke _acc_b_phi (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64 _kf_psi
     (setq _kf_psi (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _gyro_psi
     (setq _gyro_psi (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64[] _cent_pos_est
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _cent_pos_est (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _cent_pos_est i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _cent_vel_est
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _cent_vel_est (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _cent_vel_est i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64 _prevTagHeading
     (setq _prevTagHeading (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _init_flag
     (setq _init_flag (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64[] _prevTagPos
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _prevTagPos (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _prevTagPos i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64 _acc_b_phi
     (setq _acc_b_phi (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get ipe::Fusion :md5sum-) "15f601fb989711f5c196163f3ad3e1f7")
(setf (get ipe::Fusion :datatype-) "ipe/Fusion")
(setf (get ipe::Fusion :definition-)
      "std_msgs/Header header
float64 kf_psi
float64 gyro_psi
float64[] cent_pos_est
float64[] cent_vel_est
float64 prevTagHeading
float64 init_flag
float64[] prevTagPos
float64 acc_b_phi
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

")



(provide :ipe/Fusion "15f601fb989711f5c196163f3ad3e1f7")


