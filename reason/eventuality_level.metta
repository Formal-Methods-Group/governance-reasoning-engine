
; > ================================================================= ;
;                                                                     ;
; Inference rules at the 1st level, the level of the eventualities.   ;
;                                                                     ;
; ================================================================= < ;
;
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following infers that the two eventualities are connected by the "not" property in the following case:         ;
; both eventualities are direct instantiations of the same abstract eventuality                                      ;
; (i.e., they both describe the same event, state, process, or action with the same set of thematic roles),          ;
; however it is asserted for at least one of these thematic roles that one of the two eventualities                  ;
; has a certain value on that thematic role, while the other eventuality does not.                                   ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-simple-not-for-add $e1 $e2)
  (let* (
         ($c (ct-Eventuality))
         (True (debug! (evntuality $c)))
         (True (ct-triple $e1 type $c))
;         (True (debug! (1 $e1 not $e2)))
         (True (ct-triple $e2 type $c))
         (False (== $e1 $e2))
         (True (debug! (x1 $e1 not $e2)))
         ($trn (ct-ThematicRole))
         (True (ct-triple $e1 $trn $vn))
         (True (debug! (ThematicRole $trn)))
         (True (meta-triple $id $e2 $trn $vn))
         (True (debug! (meta-r $id $e2 $trn $vn)))
         (True (ct-triple $id type false))
         (True (ct-triple $id type hold))
;         (() (collapse (let* (($tr1 (ct-ThematicRole))
;                              (True (ct-triple $e1 $tr1 $s1))
;                              (False (== $tr1 $trn))
;                              (() (collapse (ct-triple $e2 $tr1 $___))))
;                         True)))
;         (() (collapse (let* (($tr2 (ct-ThematicRole))
;                              (True (ct-triple $e2 $tr2 $s2))
;                              (True (if (== $tr1 $trn) True (empty)))
;                              (() (collapse (ct-triple $e1 $tr2 $____))))
;                         True)))
;         (() (collapse (let* (($tr3 (ct-ThematicRole))
;                              (True (ct-triple $e1 $tr3 $tv1))
;                              (True (ct-triple $e2 $tr3 $tv2))
;                              (False (== $tr1 $trn))
;                              (False (== $tv1 $tv2)))
;                         True)))
         (True (debug! (exit1 $e1 not $e2)))
     )
  True))


; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following encodes ∀ea,e1,e2[and(ea, e1, e2)=>(ropom(ea)<=>(ropom(e1)∧ropom(e2)))],                              ;
; in which ropom is one of the modalities Rexist, Obligatory, Permitted, and Optional.                                ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-triple-for-add $e1 type $ropom)
  (let* (
         (True (debug! (2 $e1 type $ropom)))
         ($ropom (ct-modality))
         ($a (ct-and $ea)) ; superpose does not evaluate arg.
         ($e1 (superpose $a))
         (True (ct-triple $ea type $ropom))
         (True (debug! (exit2 $e1 type $ropom))))
   True))

(= (ct-triple-for-add $ea type $ropom)
  (let* (
         (True (debug! (3 $e1 type $ropom)))
         ($l (ct-and $ea))
         (True (debug! (and-3 $ea $l)))
         ($ropom (ct-modality))
         (True (all_is $ropom $l))
         (True (debug! (exit3 $e1 type $ropom))))
   True))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following implements the Disjunctive Syllogism (((A∨B)∧¬B)=>A)                                                              ;
; at the level of the eventualities for the Rexist and the Obligatory modality.                                                   ;
; NB. The Disjunctive Syllogism at the level of the eventualities does not hold for the Permitted or for the Optional modality.   ;
; However, for Permitted the equivalence ∀eo,e1,e2[or(eo, e1, e2)=>(Permitted(eo)<=>(Permitted(e1)∨Permitted(e2)))] hold.         ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-triple-for-add $e2 type $rom)
  (let* (
         (True (debug! (4 $e1 type $rom)))
         ($rom (superpose (rexist obligatory)))
         ($l (ct-or $eo))
         ($e2 (superpose $l))
         ($e1 (superpose $l))
         (False (== $e2 $e1))
         (True (debug! (or $e2 $e1)))
         (True (ct-triple $eo type $rom))
         (True (ct-not $en1 $e1))
         (True (ct-triple $en1 type $rom))
         (True (debug! (exit4 $e1 type $rom))))
   True))
