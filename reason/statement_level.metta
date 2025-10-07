
; > ========================================================================================================== ;
;                                                                                                              ;
;   Inference rules at the 2nd level (the level of the statements).                                            ;
;   Here only rules enabling /paraconsistent/ inferences, i.e. /NOT/ generating the Ex falso quodlibet.        ;
;                                                                                                              ;
;   Also connections between the 1st to the 2nd level: from Hobbs's logic to classical logic and vice versa.   ;
;                                                                                                              ;
; ========================================================================================================== < ;
;
; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following encodes the Disjunctive Syllogism (((A∨B)∧¬B)=>A) at the level of the statements.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-triple-for-add $r1 type hold)
   (let* (
          (True (debug! (5 $r1 type hold)))
          (True (ct-disjunction $r1 $r2))
          ; (True (trace! (found-disj $r1 $r2) True))
          (($tvr1 $tvrn1) (superpose ((true false) (false true))))
          (True (meta-triple $r1 $s $p $o))
          (True (ct-triple $r1 type $tvr1))
          ; (True (trace! (r1 $tvr1 $r1 $s $p $o) True))
          (True (meta-triple $rn1 $s $p $o))
          (True (ct-triple $r1 type hold))
          (True (ct-triple $r1 true $tvrn1))
          ; (True (trace! (hold disj $r1 $r2) True))
          (True (debug! (exit5 $r1 type hold))))
   True))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following encodes □A→A: everything that is necessary is true.                                                                   ;
; If the knowledge graph states that the reification of a triple holds *true* in the state of affairs,                                ;
; the following directly asserts the triple in the knowledge graph, to enable further inferences at the level of the eventualities.   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-triple-for-add $s $p $o)
  (let* (
        (True (meta-triple $id $s $p $o))
        ($t (superpose (true necessary)))
        (True (ct-triple $id type $t))
;        (True (trace! (6 $s $p $o) True))
        (True (ct-triple $id type hold)))
    True))

(= (collect-triple) (empty))
(= (collect-triple)
  (let True (ct-triple-for-add $s $p $o)
    ($s $p $o)))

(= (collect-meta-triple) (empty))
(= (collect-meta-triple)
  (let True (meta-triple-for-add $id $s $p $o)
    ($id $s $p $o)))

(= (collect-nots) (empty))
(= (collect-nots)
  (let True (ct-simple-not-for-add $s $o)
    ($s $o)))

(= (store-triples $l)
  (if (== () $l) ()
    (let* (($t (car-atom $l))
           (($s $p $o) $t)
           ($r (cdr-atom $l))
           ($v (store-triples $r)))
    (unify &self (ct-triple $s $p $o) $v
       (let $_ (add-atom &self (ct-triple $s $p $o))
          (cons-atom $t $v))))))

(= (store-nots $l)
  (if (== () $l) ()
    (let* (($t (car-atom $l))
           (($s $o) $t)
           ($r (cdr-atom $l))
           ($v (store-nots $r)))
    (unify &self (ct-simple-not $s $o) $v
       (let $_ (add-atom &self (ct-simple-not $s $o))
          (cons-atom $t $v))))))

(= (store-meta-triples $l)
  (if (== () $l) ()
    (let* (($t (car-atom $l))
           (($id $s $p $o) $t)
           ($r (cdr-atom $l))
           ($v (store-meta-triples $r)))
    (unify &self (meta-triple $id $s $p $o) $v
       (let $_ (add-atom &self (meta-triple $id $s $p $o))
          (cons-atom $t $v))))))

(= (make-simple-triples-step)
 (let* (($tnew (collapse (collect-triple)))
        ; (True (trace! $tnew True))
        ($rest (store-triples $tnew))
        ; (True (trace! ($rest $resm $resn (and (and (== () $rest) (== () $resm)) (== () $resn))) True))
        )
   (== () $rest)))

(= (make-not-triples-step)
 (let* (($nnew (collapse (collect-nots)))
        ; (True (trace! $nnew True))
        ($resn (store-nots $nnew))
        ; (True (trace! ($rest $resm $resn (and (and (== () $rest) (== () $resm)) (== () $resn))) True))
        )
   (== () $resn)))

(= (make-meta-triples-step)
 (let* (($mnew (collapse (collect-meta-triple)))
        ; (True (trace! $mnew True))
        ($resm (store-meta-triples $mnew))
        ; (True (trace! ($rest $resm $resn (and (and (== () $rest) (== () $resm)) (== () $resn))) True))
        )
   (== () $resm)))

(= (make-triples-step)
 (let* (($rest (make-simple-triples-step))
        ($resm (make-meta-triples-step))
        ($resn (make-not-triples-step)))
   (and (and $rest $resm) $resn)))

(= (make-triples)
 (if (make-triples-step) () (make-triples)))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following states that if one of the two arguments of "not" really exists (Rexist),                                 ;
; the other does not and vice versa. In Hobbs's logic, it corresponds to the bi-implication Rexist(?e)<=>¬Rexist(?ne).   ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (not_not_false $ne)
  (let* (
         (True (debug! (8 $ne)))
         (True (ct-not $e $ne))
         (True (ct-triple $e type rexist)))
   True))


(= (ct-triple-for-add $id type false)
   (let* (
          (True (debug! (9 $id type false)))
          (True (not_not_false $ne))
          ($id (cons-atom id_not_not_false ($ne))))
     True))
(= (ct-triple-for-add $id type hold)
   (let* (
          (True (debug! (10 $id type hold)))
          (True (not_not_false $ne))
          ($id (cons-atom id_not_not_false ($ne))))
     True))
(= (meta-triple-for-add $id $ne type rexist)
  (let* ((True (not_not_false $ne))
         ($id (cons-atom id_not_not_false ($ne))))
    True))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; The following encodes ∀eo,e1,e2[or(eo, e1, e2)=>(rpom(eo)<=>(rpom(e1)∨rpom(e2)))]; rpom being Rexist, Permitted, or Optional.      ;
; This states that if three eventualities eo, e1, and e2 are such that or(eo, e1, e2), then if eo really exists or it is permitted   ;
; or it is optional, then also the disjunction of the same modality applied to e1 and e2 is so and vice versa.                       ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (disjunction-from-or $e1 $e2 $rpom)
  (let* (($l (ct-or $eo))
         (($e1 $e2) $l)
         ($rpom (superpose (rexist permitted optional)))
         (True (ct-triple $eo type $rpom)))
   True))

(= (ct-triple-for-add $id type true)
 (let* ((True (disjunction-from-or $e1 $e2 $rpom))
        ($e (superpose ($e1 $e2)))
        ($id (disjunction-from-or-id $e $rpom)))
   True))
(= (meta-triple-for-add $id $e type $rpom)
 (let* ((True (disjunction-from-or $e1 $e2 $rpom))
        ($e (superpose ($e1 $e2)))
        ($id (disjunction-from-or-id $e $rpom)))
   True))
(= (ct-triple-for-add $d1 disjunction $d2)
 (let* ((True (disjunction-from-or $e1 $e2 $rpom))
        ($d1 (disjunction-from-or-id $e1 $rpom))
        ($d2 (disjunction-from-or-id $e2 $rpom)))
   True))

(= (ct-triple-for-add $eo type $rpom)
  (let* (($l (ct-or))
         ($e (superpose $l))
         ($rpom (superpose (rexist permitted optional)))
         (True (ct-triple $e type $rpom)))
    True))
