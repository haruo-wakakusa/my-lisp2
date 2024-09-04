; my-lisp2のライブラリ(Common Lispの処理系に読み込ませるとエラーになる)

(defun first (list) (car list))
(defun second (list) (car (cdr list)))
(defun third (list) (car (cdr (cdr list))))
(defun rest (list) (cdr list))

(defun 1+ (number) (+ number 1))
(defun 1- (number) (- number 1))

(defun not (x) (null x))

(defun reverse (list)
  (let ((rest list) (res nil))
    (block nil
      (tagbody
        a
        (if (null rest) (return-from nil res))
        (setq res (cons (car rest) res))
        (setq rest (cdr rest))
        (go a)))))

(defun mapcar (func list)
  (let ((rest list) (res nil))
    (reverse
      (block nil
        (tagbody
          a
          (if (null rest) (return-from nil res))
          (setq res (cons (funcall func (car rest)) res))
          (setq rest (cdr rest))
          (go a))))))

(defun reduce (func list)
  (let ((rest (cdr list)) (acc (car list)))
    (block nil
      (tagbody
        a
        (if (null rest) (return-from nil acc))
        (setq acc (funcall func acc (car rest)))
        (setq rest (cdr rest))
        (go a)))))

(defun append (&rest lists)
  (let ((ls (reverse (mapcar #'reverse lists)))
        (rest)
        (res nil))
    (block nil
      (tagbody
        a
        (if (null ls) (return-from nil res))
        (setq rest (first ls))
        (tagbody
          b
          (if (null rest) (go bend))
          (setq res (cons (first rest) res))
          (setq rest (rest rest))
          (go b)
          bend)
        (setq ls (rest ls))
        (go a)))))

(defmacro and (&rest conds)
  (let ((len (list-length conds)))
    (if (= len 0) T
      (if (= len 1) (first conds)
        (labels
            ((f (conds len)
                (if (= len 2)
                    (list 'if (first conds) (second conds))
                    (list 'if (first conds) (f (rest conds) (1- len))))))
          (f conds len))))))

(defmacro or (&rest conds)
  (let ((len (list-length conds)))
    (if (= len 0) NIL
      (labels
          ((f (conds len)
              (if (= len 1)
                  (first conds)
                  (let ((var (gensym)))
                    (list 'let (list (list var (first conds)))
                          (list 'if
                                var
                                var
                                (f (rest conds) (1- len))))))))
       (f conds len)))))

(defmacro return (retval)
  (list 'return-from nil retval))

(defmacro cond (&rest test-&-forms-list)
  (labels ((f (test-&-forms-list)
      (if (null test-&-forms-list)
          nil
          (let* ((test-&-forms (first test-&-forms-list))
                 (test (first test-&-forms))
                 (forms (rest test-&-forms))
                 (len (list-length forms))
                 (smaller (f (rest test-&-forms-list))))
            (if (not (consp test-&-forms))
                (error "COND: 引数がリストではありません")
                (if (null forms)
                    (let ((var (gensym)))
                      (list 'let (list (list var test))
                            (list 'if var var smaller)))
                    (if (= len 1)
                        (list 'if test (first forms) smaller)
                        (list 'if test (cons 'progn forms) smaller))))))))
    (f test-&-forms-list)))

