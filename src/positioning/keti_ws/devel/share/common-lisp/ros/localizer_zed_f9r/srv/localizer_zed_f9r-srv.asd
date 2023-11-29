
(cl:in-package :asdf)

(defsystem "localizer_zed_f9r-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Acc" :depends-on ("_package_Acc"))
    (:file "_package_Acc" :depends-on ("_package"))
    (:file "Gyro" :depends-on ("_package_Gyro"))
    (:file "_package_Gyro" :depends-on ("_package"))
  ))