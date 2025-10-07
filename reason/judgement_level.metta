
; > ======================================================================================================= ;
;                                                                                                           ;
;   Inference rules from the 2nd to the 3rd level:                                                          ;
;   from classical logic to explicit representation of                                                      ;
;                                                                                                           ;
;   contradictions,                                                                                         ;
;                   conflicts,                                                                              ;
;                              violations,                                                                  ;
;                                          necessary violations,                                            ;
;                                                              >>>                                          ;
;                                                                fallacies,                                 ;
;                                                                           mistakes,                       ;
;                                                                                   >>>>                    ;
;                                                                                     misalignments,        ;
;                                                                                                    etc.   ;
;                                                                                                           ;                                                                                 ;
; ======================================================================================================= < ;
;
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following infers a contradiction                                                                           ;
; between two facts: 1) an eventuality really exists and 2) same eventuality does not really exist,              ;
; if the knowledge graph asserts that the eventuality Rexist and does not Rexist,                                ;
; i.e., in Hobbs's logic, if Rexist(?e) and ¬Rexist(?e) are both true.                                           ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (is_in_contradiction_with (meta-id $e type rexist (:: true (:: hold))) $r)
  (let* ((True (ct-triple $e type rexist))
         (True (meta-triple $r $e type rexist))
         (True (ct-triple $r type false))
         (True (ct-triple $r type hold)))
     ((meta-id $e type rexist true) $r)))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; If an eventuality ?e has a certain value on a certain thematic role, but at the same time                                                                   ;
; the knowledge graph asserts that that thematic role does not have that value,                                                                               ;
; the following infers that the fact that the eventuality has that value on that thematic role #is-in-contradiction-with the fact that it does not have it.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (is_in_contradiction_with (meta-id $e $tr $tv (:: true (:: hold))) $r)
  (let* (($tr (ct-ThematicRole))
         (True (ct-triple $e $tr $tv))
         (True (meta-triple $r $e $tr $tv))
         (True (ct-triple $r type false))
         (True (ct-triple $r type hold)))
     ((meta-id $e type rexist false) $r)))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following infers when an obligation                                            ;
; is complied with by an eventuality more specific than it and that really exists.   ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (is_complied_with_by $eo $e)
  (let* ((True (ct-triple $eo type obligatory))
         ; (True (trace! ($eo obligatory) True))
         ($c (ct-Eventuality))
         (False (== obligatory $c))
         (True (ct-triple $eo type $c))
         ; (True (trace! ($eo $c Eventuality) True))
         (True (ct-triple $e type rexist))
         ; (True (trace! ($e rexists) True))
         (True (ct-triple $e type $c))
         ; (True (trace! ($e type $c) True))
;         (() (collapse (let* (($tr (ct-ThematicRole)) (True (ct-triple $eo $tr $_1)) (() (collapse (ct-triple $e $tr $_2)))) True)))
;         (() (collapse (let* (($tr (ct-ThematicRole)) (True (ct-triple $eo $tr $vo)) (True (ct-triple $e $tr $ve))) (False (== $vo $ve)) True)))
        )
     ($eo $e)))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following infers when a prohibition, i.e., a non-permission,              ;
; is violated by an eventuality more specific than it and that really exists.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (is_violated_by $epr (meta-id $e type rexist (:: true (:: hold))))
  (let* ((True (meta-triple $epr $ep type permitted))
         (True (ct-triple $erp type false))
         (True (ct-triple $erp type hold))
;         (() (collapse (let* (($tr (ct-ThematicRole)) (True (ct-triple $ep $tr $_1)) (() (collapse (ct-triple $e $tr $_2)))) True)))
;         (() (collapse (let* (($tr (ct-ThematicRole)) (True (ct-triple $ep $tr $vp)) (True (ct-triple $e $tr $ve)) (False (== $vp $ve))) True)))
        )
     ($epr $e true)))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following infers when a prohibition, i.e., a non-permission, is in conflict with a permission.                                                   ;
; Note that the permission could be possibly inferred from an obligation in virtue of the entailment OB(p)=>PE(p) of the Deontic Traditional Scheme.   ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (is-in-conflict-with $enr (meta-id $e type permitted (:: true (:: hold ()))))
  (let* ((True (ct-triple $enr type false))
         (True (ct-triple $enr type hold))
         (True (meta-triple $enr $en type permitted))
         ($c (ct-Eventuality))
         (True (ct-triple $en type $c))
;         (True (trace! (quote ($enr $en $c)) True))
         (True (ct-triple $e type permitted))
;         (True (trace! (quote ($enr $en $c $e)) True))
         (() (collapse (let* (($tr (ct-ThematicRole)) (True (ct-triple $en $tr $_1)) (() (collapse (ct-triple $e $tr $_2)))) True)))
         (() (collapse (let* (($tr (ct-ThematicRole)) (True (ct-triple $en $tr $vn)) (True (ct-triple $e $tr $vp)) (False (== $vn $vp))) True))))
    (conflict $enr $e) ))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following infers when a prohibition, i.e., a non-permission, is necessarily violated by a constraint from the context,   ;
; i.e., by the fact that something must be necessarily done in a way that violates the prohibition.                            ;
; For example, when it is necessary to pay in INRS but, at the same time, it is prohibited to pay in INRS.                     ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (is-necessarily-violated-by $rep $ren)
 (let* (($trn (ct-ThematicRole))
        (True (ct-triple $en $trn $vn))
;         (True (trace! (en $en $trn $vn) True))
        (True (ct-triple $ren type necessary))
;         (True (trace! (ren1 $ren $en $trn $vn) True))
        (True (ct-triple $ren type hold))
;         (True (trace! (ren2 $ren $en $trn $vn) True))
        (True (meta-triple $ren $en $trn $vn))
;         (True (trace! (ren3 $ren $en $trn $vn) True))
        (True (ct-triple $rep type false))
        (True (ct-triple $rep type hold))
        (True (meta-triple $rep $ep type permitted))
;         (True (trace! (rep $rep $ep) True))
        ($c (ct-Eventuality))
        (True (ct-triple $en type $c))
        (True (ct-triple $ep type $c))
;         (True (trace! (c $c) True))
        (() (collapse (let* (($tr (ct-ThematicRole)) (True (ct-triple $en $tr $vn)) (() (collapse (ct-triple $ep $tr $vp)))) True)))
        (() (collapse (let* (($tr (ct-ThematicRole)) (True (ct-triple $en $tr $vn)) (True (ct-triple $ep $tr $vp)) (False (== $vn $vp))) True))))
   (quote ($rep $ren)) ))
