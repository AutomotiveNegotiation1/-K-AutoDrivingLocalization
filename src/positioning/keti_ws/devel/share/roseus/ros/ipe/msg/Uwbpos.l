;; Auto-generated. Do not edit!


(when (boundp 'ipe::Uwbpos)
  (if (not (find-package "IPE"))
    (make-package "IPE"))
  (shadow 'Uwbpos (find-package "IPE")))
(unless (find-package "IPE::UWBPOS")
  (make-package "IPE::UWBPOS"))

(in-package "ROS")
;;//! \htmlinclude Uwbpos.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass ipe::Uwbpos
  :super ros::object
  :slots (_header _tag_center_vel_est _tag_pos_est _tag_pos_b _UWBErrSum _heading_est _Nanchor _init_flag _zt_b ))

(defmethod ipe::Uwbpos
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:tag_center_vel_est __tag_center_vel_est) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:tag_pos_est __tag_pos_est) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:tag_pos_b __tag_pos_b) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:UWBErrSum __UWBErrSum) 0.0)
    ((:heading_est __heading_est) 0.0)
    ((:Nanchor __Nanchor) 0.0)
    ((:init_flag __init_flag) 0.0)
    ((:zt_b __zt_b) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _tag_center_vel_est __tag_center_vel_est)
   (setq _tag_pos_est __tag_pos_est)
   (setq _tag_pos_b __tag_pos_b)
   (setq _UWBErrSum (float __UWBErrSum))
   (setq _heading_est (float __heading_est))
   (setq _Nanchor (float __Nanchor))
   (setq _init_flag (float __init_flag))
   (setq _zt_b (float __zt_b))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:tag_center_vel_est
   (&optional __tag_center_vel_est)
   (if __tag_center_vel_est (setq _tag_center_vel_est __tag_center_vel_est)) _tag_center_vel_est)
  (:tag_pos_est
   (&optional __tag_pos_est)
   (if __tag_pos_est (setq _tag_pos_est __tag_pos_est)) _tag_pos_est)
  (:tag_pos_b
   (&optional __tag_pos_b)
   (if __tag_pos_b (setq _tag_pos_b __tag_pos_b)) _tag_pos_b)
  (:UWBErrSum
   (&optional __UWBErrSum)
   (if __UWBErrSum (setq _UWBErrSum __UWBErrSum)) _UWBErrSum)
  (:heading_est
   (&optional __heading_est)
   (if __heading_est (setq _heading_est __heading_est)) _heading_est)
  (:Nanchor
   (&optional __Nanchor)
   (if __Nanchor (setq _Nanchor __Nanchor)) _Nanchor)
  (:init_flag
   (&optional __init_flag)
   (if __init_flag (setq _init_flag __init_flag)) _init_flag)
  (:zt_b
   (&optional __zt_b)
   (if __zt_b (setq _zt_b __zt_b)) _zt_b)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64[] _tag_center_vel_est
    (* 8    (length _tag_center_vel_est)) 4
    ;; float64[] _tag_pos_est
    (* 8    (length _tag_pos_est)) 4
    ;; float64[] _tag_pos_b
    (* 8    (length _tag_pos_b)) 4
    ;; float64 _UWBErrSum
    8
    ;; float64 _heading_est
    8
    ;; float64 _Nanchor
    8
    ;; float64 _init_flag
    8
    ;; float64 _zt_b
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float64[] _tag_center_vel_est
     (write-long (length _tag_center_vel_est) s)
     (dotimes (i (length _tag_center_vel_est))
       (sys::poke (elt _tag_center_vel_est i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _tag_pos_est
     (write-long (length _tag_pos_est) s)
     (dotimes (i (length _tag_pos_est))
       (sys::poke (elt _tag_pos_est i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _tag_pos_b
     (write-long (length _tag_pos_b) s)
     (dotimes (i (length _tag_pos_b))
       (sys::poke (elt _tag_pos_b i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64 _UWBErrSum
       (sys::poke _UWBErrSum (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _heading_est
       (sys::poke _heading_est (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _Nanchor
       (sys::poke _Nanchor (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _init_flag
       (sys::poke _init_flag (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _zt_b
       (sys::poke _zt_b (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64[] _tag_center_vel_est
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _tag_center_vel_est (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _tag_center_vel_est i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _tag_pos_est
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _tag_pos_est (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _tag_pos_est i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _tag_pos_b
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _tag_pos_b (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _tag_pos_b i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64 _UWBErrSum
     (setq _UWBErrSum (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _heading_est
     (setq _heading_est (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _Nanchor
     (setq _Nanchor (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _init_flag
     (setq _init_flag (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _zt_b
     (setq _zt_b (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get ipe::Uwbpos :md5sum-) "b5e368833065ec31ffd718a3d27ee9f6")
(setf (get ipe::Uwbpos :datatype-) "ipe/Uwbpos")
(setf (get ipe::Uwbpos :definition-)
      "std_msgs/Header header
float64[] tag_center_vel_est
float64[] tag_pos_est
float64[] tag_pos_b
float64 UWBErrSum
float64 heading_est
float64 Nanchor
float64 init_flag
float64 zt_b
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



(provide :ipe/Uwbpos "b5e368833065ec31ffd718a3d27ee9f6")


