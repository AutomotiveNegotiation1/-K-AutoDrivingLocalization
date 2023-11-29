;; Auto-generated. Do not edit!


(when (boundp 'ipe::Imupos)
  (if (not (find-package "IPE"))
    (make-package "IPE"))
  (shadow 'Imupos (find-package "IPE")))
(unless (find-package "IPE::IMUPOS")
  (make-package "IPE::IMUPOS"))

(in-package "ROS")
;;//! \htmlinclude Imupos.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass ipe::Imupos
  :super ros::object
  :slots (_header _b_acc_o _b_gyro _mode _kf_psi _gyro_psi _cent_pos_est _cent_vel_est _state_o _acc_b_phi _acc_b_theta ))

(defmethod ipe::Imupos
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:b_acc_o __b_acc_o) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:b_gyro __b_gyro) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:mode __mode) 0.0)
    ((:kf_psi __kf_psi) 0.0)
    ((:gyro_psi __gyro_psi) 0.0)
    ((:cent_pos_est __cent_pos_est) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:cent_vel_est __cent_vel_est) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:state_o __state_o) 0.0)
    ((:acc_b_phi __acc_b_phi) 0.0)
    ((:acc_b_theta __acc_b_theta) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _b_acc_o __b_acc_o)
   (setq _b_gyro __b_gyro)
   (setq _mode (float __mode))
   (setq _kf_psi (float __kf_psi))
   (setq _gyro_psi (float __gyro_psi))
   (setq _cent_pos_est __cent_pos_est)
   (setq _cent_vel_est __cent_vel_est)
   (setq _state_o (float __state_o))
   (setq _acc_b_phi (float __acc_b_phi))
   (setq _acc_b_theta (float __acc_b_theta))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:b_acc_o
   (&optional __b_acc_o)
   (if __b_acc_o (setq _b_acc_o __b_acc_o)) _b_acc_o)
  (:b_gyro
   (&optional __b_gyro)
   (if __b_gyro (setq _b_gyro __b_gyro)) _b_gyro)
  (:mode
   (&optional __mode)
   (if __mode (setq _mode __mode)) _mode)
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
  (:state_o
   (&optional __state_o)
   (if __state_o (setq _state_o __state_o)) _state_o)
  (:acc_b_phi
   (&optional __acc_b_phi)
   (if __acc_b_phi (setq _acc_b_phi __acc_b_phi)) _acc_b_phi)
  (:acc_b_theta
   (&optional __acc_b_theta)
   (if __acc_b_theta (setq _acc_b_theta __acc_b_theta)) _acc_b_theta)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64[] _b_acc_o
    (* 8    (length _b_acc_o)) 4
    ;; float64[] _b_gyro
    (* 8    (length _b_gyro)) 4
    ;; float64 _mode
    8
    ;; float64 _kf_psi
    8
    ;; float64 _gyro_psi
    8
    ;; float64[] _cent_pos_est
    (* 8    (length _cent_pos_est)) 4
    ;; float64[] _cent_vel_est
    (* 8    (length _cent_vel_est)) 4
    ;; float64 _state_o
    8
    ;; float64 _acc_b_phi
    8
    ;; float64 _acc_b_theta
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float64[] _b_acc_o
     (write-long (length _b_acc_o) s)
     (dotimes (i (length _b_acc_o))
       (sys::poke (elt _b_acc_o i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _b_gyro
     (write-long (length _b_gyro) s)
     (dotimes (i (length _b_gyro))
       (sys::poke (elt _b_gyro i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64 _mode
       (sys::poke _mode (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
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
     ;; float64 _state_o
       (sys::poke _state_o (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _acc_b_phi
       (sys::poke _acc_b_phi (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _acc_b_theta
       (sys::poke _acc_b_theta (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64[] _b_acc_o
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _b_acc_o (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _b_acc_o i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _b_gyro
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _b_gyro (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _b_gyro i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64 _mode
     (setq _mode (sys::peek buf ptr- :double)) (incf ptr- 8)
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
   ;; float64 _state_o
     (setq _state_o (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _acc_b_phi
     (setq _acc_b_phi (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _acc_b_theta
     (setq _acc_b_theta (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get ipe::Imupos :md5sum-) "b7b3c908ac554cd386d59718bc4de7e4")
(setf (get ipe::Imupos :datatype-) "ipe/Imupos")
(setf (get ipe::Imupos :definition-)
      "std_msgs/Header header
float64[] b_acc_o
float64[] b_gyro
float64 mode
float64 kf_psi
float64 gyro_psi
float64[] cent_pos_est
float64[] cent_vel_est
float64 state_o
float64 acc_b_phi
float64 acc_b_theta
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



(provide :ipe/Imupos "b7b3c908ac554cd386d59718bc4de7e4")


