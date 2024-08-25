(let ((x 1))
  (tagbody
    a
    (prin1 x)
    (setq x (+ x 1))
    (if (< x 10) (go a))))

