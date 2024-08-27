(defun my-reverse (list)
  (let ((rest list) (res nil))
    (block nil
      (tagbody
        a
        (if (null rest) (return-from nil res))
        (setq res (cons (car rest) res))
        (setq rest (cdr rest))
        (go a)))))

(defun my-mapcar (func list)
  (let ((rest list) (res nil))
    (my-reverse
      (block nil
        (tagbody
          a
          (if (null rest) (return-from nil res))
          (setq res (cons (funcall func (car rest)) res))
          (setq rest (cdr rest))
          (go a))))))

(defun my-reduce (func list)
  (let ((rest (cdr list)) (acc (car list)))
    (block nil
      (tagbody
        a
        (if (null rest) (return-from nil acc))
        (setq acc (funcall func acc (car rest)))
        (setq rest (cdr rest))
        (go a)))))

