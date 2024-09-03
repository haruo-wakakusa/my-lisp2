; my-lisp2のライブラリ(Common Lispの処理系に読み込ませるとエラーになる)

(defun first (list) (car list))
(defun second (list) (car (cdr list)))
(defun rest (list) (cdr list))

(defun 1+ (number) (+ number 1))
(defun 1- (number) (- number 1))

(defun not (x) (null x))

