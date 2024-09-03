(defmacro my-and (&rest conds)
  (let ((len (list-length conds)))
    (if (= len 0) T
      (if (= len 1) (first conds)
        (labels
            ((f (conds len)
                (if (= len 2)
                    (list 'if (first conds) (second conds))
                    (list 'if (first conds) (f (rest conds) (1- len))))))
          (f conds len))))))

(defmacro my-or (&rest conds)
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

(defmacro my-return (retval)
  (list 'return-from nil retval))

(defmacro my-cond (&rest test-&-forms-list)
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

