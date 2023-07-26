;; Auto-generated. Do not edit!


(when (boundp 'localizer_zed_f9r::Acc)
  (if (not (find-package "LOCALIZER_ZED_F9R"))
    (make-package "LOCALIZER_ZED_F9R"))
  (shadow 'Acc (find-package "LOCALIZER_ZED_F9R")))
(unless (find-package "LOCALIZER_ZED_F9R::ACC")
  (make-package "LOCALIZER_ZED_F9R::ACC"))
(unless (find-package "LOCALIZER_ZED_F9R::ACCREQUEST")
  (make-package "LOCALIZER_ZED_F9R::ACCREQUEST"))
(unless (find-package "LOCALIZER_ZED_F9R::ACCRESPONSE")
  (make-package "LOCALIZER_ZED_F9R::ACCRESPONSE"))

(in-package "ROS")





(defclass localizer_zed_f9r::AccRequest
  :super ros::object
  :slots ())

(defmethod localizer_zed_f9r::AccRequest
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

(defclass localizer_zed_f9r::AccResponse
  :super ros::object
  :slots (_x _y _z ))

(defmethod localizer_zed_f9r::AccResponse
  (:init
   (&key
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:z __z) 0.0)
    )
   (send-super :init)
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _z (float __z))
   self)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:z
   (&optional __z)
   (if __z (setq _z __z)) _z)
  (:serialization-length
   ()
   (+
    ;; float64 _x
    8
    ;; float64 _y
    8
    ;; float64 _z
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _x
       (sys::poke _x (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _y
       (sys::poke _y (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _z
       (sys::poke _z (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _x
     (setq _x (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _y
     (setq _y (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _z
     (setq _z (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(defclass localizer_zed_f9r::Acc
  :super ros::object
  :slots ())

(setf (get localizer_zed_f9r::Acc :md5sum-) "4a842b65f413084dc2b10fb484ea7f17")
(setf (get localizer_zed_f9r::Acc :datatype-) "localizer_zed_f9r/Acc")
(setf (get localizer_zed_f9r::Acc :request) localizer_zed_f9r::AccRequest)
(setf (get localizer_zed_f9r::Acc :response) localizer_zed_f9r::AccResponse)

(defmethod localizer_zed_f9r::AccRequest
  (:response () (instance localizer_zed_f9r::AccResponse :init)))

(setf (get localizer_zed_f9r::AccRequest :md5sum-) "4a842b65f413084dc2b10fb484ea7f17")
(setf (get localizer_zed_f9r::AccRequest :datatype-) "localizer_zed_f9r/AccRequest")
(setf (get localizer_zed_f9r::AccRequest :definition-)
      "---
float64 x
float64 y
float64 z
")

(setf (get localizer_zed_f9r::AccResponse :md5sum-) "4a842b65f413084dc2b10fb484ea7f17")
(setf (get localizer_zed_f9r::AccResponse :datatype-) "localizer_zed_f9r/AccResponse")
(setf (get localizer_zed_f9r::AccResponse :definition-)
      "---
float64 x
float64 y
float64 z
")



(provide :localizer_zed_f9r/Acc "4a842b65f413084dc2b10fb484ea7f17")


