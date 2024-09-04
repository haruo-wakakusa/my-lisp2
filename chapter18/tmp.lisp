(defmacro my-do (var-list test-&-result &rest statements)
  (let ((vlist1 (reverse var-list))
        (vlist2 nil)
        (slist nil) ; step-form-list
        (vis) ; var / init-form / step-form
        (jump-label (gensym)))
    (tagbody
      loop1
      (if (null vlist1) (go loopend1))
      (setq vis (first vlist1))
      (cond
        ((symbolp vis)
         (setq vlist2 (cons (list vis nil) vlist2)))
        ((= (list-length vis) 1)
         (setq vlist2 (cons (list (first vis) nil) vlist2)))
        ((= (list-length vis) 2)
         (setq vlist2 (cons vis vlist2)))
        (t
         (setq vlist2 (cons (list (first vis) (second vis)) vlist2))
         (setq slist (cons (first vis) (cons (third vis) slist)))))
      (setq vlist1 (rest vlist1))
      (go loop1)
      loopend1)
    (list 'let vlist2
          (list 'block nil
                (append
                  (list 'tagbody
                        jump-label
                        (list 'if
                              (first test-&-result)
                              (list 'return-from
                                    nil
                                    (cons 'progn (rest test-&-result)))))
                  statements
                  (list (cons 'psetq slist)
                        (list 'go jump-label)))))))

(defmacro my-do* (var-list test-&-result &rest statements)
  (let ((vlist1 (reverse var-list))
        (vlist2 nil)
        (slist nil) ; step-form-list
        (vis) ; var / init-form / step-form
        (jump-label (gensym)))
    (tagbody
      loop1
      (if (null vlist1) (go loopend1))
      (setq vis (first vlist1))
      (cond
        ((symbolp vis)
         (setq vlist2 (cons (list vis nil) vlist2)))
        ((= (list-length vis) 1)
         (setq vlist2 (cons (list (first vis) nil) vlist2)))
        ((= (list-length vis) 2)
         (setq vlist2 (cons vis vlist2)))
        (t
         (setq vlist2 (cons (list (first vis) (second vis)) vlist2))
         (setq slist (cons (first vis) (cons (third vis) slist)))))
      (setq vlist1 (rest vlist1))
      (go loop1)
      loopend1)
    (list 'let* vlist2
          (list 'block nil
                (append
                  (list 'tagbody
                        jump-label
                        (list 'if
                              (first test-&-result)
                              (list 'return-from
                                    nil
                                    (cons 'progn (rest test-&-result)))))
                  statements
                  (list (cons 'setq slist)
                        (list 'go jump-label)))))))

(defmacro my-dotimes (var-spec &rest statements)
  (let ((var (first var-spec))
        (count-form (second var-spec))
        (result-form (third var-spec))
        (end-val (gensym)))
    (append
      (list 'my-do*
            (list (list var 0 (list '1+ var))
                  (list end-val count-form))
            (list (list '= var end-val) result-form))
      statements)))

(defmacro my-dolist (var-spec &rest statements)
  (let ((var (first var-spec))
        (list-form (second var-spec))
        (result-form (third var-spec))
        (list-var (gensym)))
    (append
      (list 'my-do*
            (list (list list-var list-form (list 'cdr list-var))
                  (list var (list 'car list-var) (list 'car list-var)))
            (list (list 'null list-var) result-form))
      statements)))

