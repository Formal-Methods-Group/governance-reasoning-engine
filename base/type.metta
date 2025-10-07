
; > ================ ;
;   Type operations  ;
; ================ < ;
;
; > –––––––––––––––– ;
;   Type extraction  ;
; –––––––––––––––– < ;

(= (ct-type $o)
  (let True (ct-triple $o type $t) $t))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;   Type Checking                                                   ;
;   Recursively checks if all elements in list `$l` have type `$t`  ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (all_is $t $l)
  (if (== () $l) True
    (let* (($h (car-atom $l))
           (True (ct-triple $h type $t)))
        (all_is $t (cdr-atom $l)))))
