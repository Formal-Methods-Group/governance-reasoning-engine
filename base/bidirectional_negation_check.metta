
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;   Bidirectional negation check                                       ;
;  `ct-not` succeeds if either `(e1 not e2)` or `(e2 not e1)` exists   ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-simple-not $e1 $e2)
  (unify &self (ct-simple-not $e1 $e2) True (empty)))

(= (ct-not $e1 $e2)
  (let True (ct-simple-not $e1 $e2) True))
(= (ct-not $e1 $e2)
  (let True (ct-simple-not $e2 $e1) True))
