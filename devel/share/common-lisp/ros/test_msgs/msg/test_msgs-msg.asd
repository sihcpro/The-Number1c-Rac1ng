
(cl:in-package :asdf)

(defsystem "test_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "my_file" :depends-on ("_package_my_file"))
    (:file "_package_my_file" :depends-on ("_package"))
  ))