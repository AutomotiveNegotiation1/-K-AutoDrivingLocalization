
(cl:in-package :asdf)

(defsystem "localizer_zed_f9r-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "GNSS" :depends-on ("_package_GNSS"))
    (:file "_package_GNSS" :depends-on ("_package"))
  ))