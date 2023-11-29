
(cl:in-package :asdf)

(defsystem "ipe-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Anchor" :depends-on ("_package_Anchor"))
    (:file "_package_Anchor" :depends-on ("_package"))
    (:file "Fusion" :depends-on ("_package_Fusion"))
    (:file "_package_Fusion" :depends-on ("_package"))
    (:file "Imupos" :depends-on ("_package_Imupos"))
    (:file "_package_Imupos" :depends-on ("_package"))
    (:file "Tag" :depends-on ("_package_Tag"))
    (:file "_package_Tag" :depends-on ("_package"))
    (:file "Uwbpos" :depends-on ("_package_Uwbpos"))
    (:file "_package_Uwbpos" :depends-on ("_package"))
  ))