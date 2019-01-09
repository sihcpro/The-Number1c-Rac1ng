
(cl:in-package :asdf)

(defsystem "traffic_sign-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "traffic_sign_msg" :depends-on ("_package_traffic_sign_msg"))
    (:file "_package_traffic_sign_msg" :depends-on ("_package"))
  ))