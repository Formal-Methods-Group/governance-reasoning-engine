
; > ============================================================= ;
;                                                                 ;
; Inference rules from the Deontic Traditional Scheme:            ;
; Some of these rules connect the 1st with the 2nd level,         ;
; the others make inferences only on either of the two levels.    ;
;                                                                 ;
; ============================================================= < ;
;
; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the derivations PE(p)=>¬OB(¬p) and OB(¬p)=>¬PE(p) of the Deontic Traditional Scheme.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (mod_not $ne $ddm)
  (let* ((True (ct-not $ne $e))
         (($m $ddm) (superpose ((obligatory permitted) (permitted obligatory))))
         (True (ct-triple $e type $m)))
    True))

(= (ct-triple-for-add $id type false)
 (let* ((True (mod_not $ne $ddm))
        ($id (mod-not-id $ne $ddm)))
   True))
(= (ct-triple-for-add $id type hold)
 (let* ((True (mod_not $ne $ddm))
        ($id (mod-not-id $ne $ddm)))
   True))
(= (meta-triple-for-add $id $ne type $ddm)
 (let* ((True (mod_not $ne $ddm))
        ($id (mod-not-id $ne $ddm)))
   True))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the derivations ¬OB(¬p)=>PE(p) and ¬PE(p)=>OB(¬p) of the Deontic Traditional Scheme.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-triple-for-add $ne type $ddm)
  (let* ((True (ct-not $e $ne))
         ;  (True (trace! (found-not $e $ne) True))
         (($m $ddm) (superpose ((obligatory permitted) (permitted obligatory))))
         (True (meta-triple $r $e type $m))
         ;  (True (trace! (found-meta $r $e $m $ddm) True))
         (True (ct-triple $r type false))
         (True (ct-triple $r type hold)))
     True))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the derivation (¬OB(p)∧¬OB(¬p))=>OP(p) of the Deontic Traditional Scheme.   ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-triple-for-add $e type optional)
  (let* (
         (True (ct-not $e $ne)) ;(True (trace! (is-not-1 $e $ne) True))
         (True (meta-triple $em $e type obligatory))
         (True (ct-triple $em type false))
         (True (ct-triple $em type hold))
         (True (meta-triple $enm $en type obligatory))
         (True (ct-triple $enm type false))
         (True (ct-triple $enm type hold))
         ; (True (trace! (obligatorys-not $em $e $enm $ne) True))
         )
    True))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the derivation OP(p)=>(¬OB(p)∧¬OB(¬p)) of the Deontic Traditional Scheme.   ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (optional-obligatory $ne)
  (let* ((True (ct-triple $e type optional))
         (True (debug! (optional-obligatory-1 $id type hold)))
         (True (ct-not $e $ne)))
     True))

(= (ct-triple-for-add $id type $t)
  (let* (($t (superpose (false hold)))
         (True (optional-obligatory $ne))
         ($id (optional-obligatory-id $ne)))
    True))
(= (meta-triple-for-add $id $ne type obligatory)
  (let* ((True (optional-obligatory $ne))
         ($id (optional-obligatory-id $ne)))
     True))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the derivation (OB(p)∨OB(¬p))=>¬OP(p) of the Deontic Traditional Scheme.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (not-optional $re)
  (let* ((True (ct-triple $a type obligatory))
         (True (ct-not $re $e))
         ($a (superpose ($re $e))))
    True))

(= (false+hold) false)
(= (false+hold) hold)

(= (meta-triple-for-add $id $e type optional)
  (let* ((True (not-optional $e))
         ($id (not-optional-id $e)))
     True))
(= (ct-triple-for-add $id type $fh)
 (let* (($fh (false+hold))
        (True (not-optional $e))
        ($id (not-optional-id $e)))
   True))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the derivation ¬OP(p)=>(OB(p)∨OB(¬p)) of the Deontic Traditional Scheme.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (mk-disjunction-not $e $ne)
  (let* ((True (ct-not $e $ne))
         (True (meta-triple $id $e type optional))
         (True (ct-triple $id type false))
         (True (ct-triple $id type hold)))
    True))
(= (meta-triple-for-add $id $e type obligatory)
  (let* ((True (mk-disjunction-not $e $_))
         ($id (disjunction-arg $e obligatory)))
  True))
(= (ct-triple-for-add $id type $t)
  (let* (($t (superpose (true hold)))
         (True (mk-disjunction-not $e $_))
         ($id (disjunction-arg $e obligatory)))
  True))
(= (meta-triple-for-add $id $e type obligatory)
  (let* ((True (mk-disjunction-not $_ $e))
         ($id (disjunction-arg $e obligatory)))
  True))
(= (ct-triple-for-add $id type $t)
  (let* (($t (superpose (true hold)))
         (True (mk-disjunction-not $_ $e))
         ($id (disjunction-arg $e obligatory)))
  True))
(= (ct-disjunction-simple $id1 $id2)
  (let* ((True (mk-disjunction-not $e $ne))
          ($id1 (disjunction-arg $e obligatory))
          ($id2 (disjunction-arg $ne obligatory)))
    True))
(= (ct-disjunction $id1 $id2) (ct-disjunction-simple $id1 $id2))
(= (ct-disjunction $id1 $id2) (ct-disjunction-simple $id2 $id1))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the derivation OB(p)=>PE(p) of the Deontic Traditional Scheme.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-triple-for-add $e type permitted) (ct-triple $e type obligatory))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the derivation ¬PE(p)=>¬OB(p) of the Deontic Traditional Scheme.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (not-obligatory $e)
 (let* ((True (meta-triple $id $e type permitted))
        (True (ct-triple $id type false))
        (True (ct-triple $id type hold)))
   True))

(= (meta-triple-for-add $id $e type obligatory)
 (let* ((True (not-obligatory $e))
        ($id (not-obligatory-id $e)))
   True))
(= (ct-triple-for-add  $id type $fh)
 (let* (($fh (superpose (false hold)))
        (True (not-obligatory $e))
        ($id (not-obligatory-id $e)))
   True))
