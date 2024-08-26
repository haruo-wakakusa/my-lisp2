;(flet ((f (x) (if (= x 0) 0 (+ x (f (- x 1)))))) (f 10)) => error

(prin1 (labels ((f (x) (if (= x 0) 0 (+ x (f (- x 1)))))) (f 10))) ; 55

