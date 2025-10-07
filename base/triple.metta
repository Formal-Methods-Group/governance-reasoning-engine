
; > ==================================================== ;
;   Core Data Structures: triple operations and storage  ;
; ==================================================== < ;
;
; > ––––––––––––––––––––––––––––––––––––––– ;
;   Core triple: subject-predicate-object   ;
; ––––––––––––––––––––––––––––––––––––––– < ;
(= (ct-triple $a $b $c) (empty))

; > –––––––––––––––––––––––– ;
;   Core triple conjunction  ;
; –––––––––––––––––––––––– < ;

(= (ct-and $a) (empty))

; > –––––––––––––––––––––––– ;
;   Core triple disjunction  ;
; –––––––––––––––––––––––– < ;

(= (ct-or $a) (empty))

; > –––––––––––––––––––– ;
;   Meta triple with ID  ;
; –––––––––––––––––––– < ;

(= (meta-triple $id $a $b $c) (empty))

; > –––––––––––––––––––––––– ;
;   Core triple unification  ;
; –––––––––––––––––––––––– < ;

(= (ct-triple $o $p $s)
  (unify &self (ct-triple $o $p $s) True (empty)))

; > –––––––––––––––––––––––– ;
;   Meta triple unification  ;
; –––––––––––––––––––––––– < ;

(= (meta-triple $id $o $p $s)
  (unify &self (meta-triple $id $o $p $s) True (empty)))
