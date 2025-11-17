
; > =================================================================================================================== ;
;                                                                                                                       ;
;   Setting: Smart Port Agent monitors and controls a fully automated                                                   ;
;   Mundra International Container Terminal (MICT), India's busiest port, and manages its finances externally.          ;
;                                                                                                                       ;
;   ALEXANDRA MÆRSK, a container vessel operated by A.P. Møller – Mærsk of Denmark, arrives at MICT carrying            ;
;   16,000 TEU from European ports. The operation involves 10 different stakeholders across 4 regulatory                ;
;   jurisdictions, with payments flowing in 4 different stablecoins through blockchain-enabled settlement systems.      ;
;   Port Configuration:                                                                                                 ;
;	  •	12 automated Ship-to-Shore (STS) gantry cranes                                                                  ;
;	  •	IoT-enabled container tracking with 50,000+ sensors                                                             ;
;	  •	Multi-currency stablecoin treasury (INR, EUR, USD stablecoins)                                                  ;
;	  •	Cross-border compliance automation via smart contracts                                                          ;
;	  •	Real-time energy management with dynamic pricing                                                                ;
;	  •	Real-time weather forecasting and route optimization                                                            ;
;                                                                                                                       ;
; =================================================================================================================== < ;
;
; > ================================= ;
;                                     ;
; // Smart Port Agent case//          ;
;                                     ;
; =================================== ;
;
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Unified Sky Knowledge Representation Framework/                                                            ;
;                                                                                                              ;
;               Four statements, incl.                                                                         ;
;                                                                                                              ;
;               (1) Internal norm                                                                              ;
;               (2) Real World Environment Limitation                                                          ;
;               (3) Real World Regulatory Limitation                                                           ;
;               (4) State of Affairs                                                                           ;
;                                                                                                              ;
;               are uniformly represented within the massive reification-based                                 ;
;               Unified Sky Knowledge Representation Framework.                                                ;
;                                                                                                              ;
;               (1), (2), and (3) are formalized as inference rules.                                           ;
;                                                                                                              ;
;               The State of Affairs contains then the vessel soa_ALEXANDRA_MAERSK, a container vessel,        ;
;               which moored at soa_berthMICT, the berth at MICT.                                              ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;
;
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Internal regulation for the Smart Port Agent/                                                              ;
;                                                                                                              ;
;                (1) Whoever moors at a berth is obliged to pay equivalent of $15000 to                        ;
;                    the Smart Port Treasury associated with that berth.                                       ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (pay-obligatory $v $spt)
  (let* ((True (ct-triple $e type soaMoor))
         (True (ct-triple $e soaHas_agent $v))
         (True (ct-triple $e soaHas_location $b))
         (True (ct-triple $v type soaContainerVessel))
         (True (ct-triple $m type soa_mooringBerth))
         (True (ct-triple $spt soa_associated-with $b)))
    True))

(= (ct-triple-for-add $id type soaPay)
   (let* (($id (pay-obligatory-id $v $spt))
          (True (pay-obligatory $v $spt)))
     True))
(= (ct-triple-for-add $id type obligatory)
   (let* (($id (pay-obligatory-id $v $spt))
          (True (pay-obligatory $v $spt)))
     True))
(= (ct-triple-for-add $id soaHas_agent $v)
   (let* (($id (pay-obligatory-id $v $spt))
          (True (pay-obligatory $v $spt)))
     True))
(= (ct-triple-for-add $id soaHas_amount (15000USD))
   (let* (($id (pay-obligatory-id $v $spt))
          (True (pay-obligatory $v $spt)))
     True))
(= (ct-triple-for-add $id soaHas_goal $spt)
   (let* (($id (pay-obligatory-id $v $spt))
          (True (pay-obligatory $v $spt)))
     True))

; Init pre
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Real World Environment Limitation/                                                                         ;
;                                                                                                              ;
;                (2) MICT Smart Port Treasury accepts only INRS, the Indian rupee-pegged stablecoin.           ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (inrs-only $ep)
  (let* ((True (ct-triple $ep type soaPay))
;          (True (trace! (check-inrs $ep) True))
         (True (ct-triple $ep soaHas_goal $spt))
;          (True (trace! (check-inrs1 $spt) True))
         (True (ct-triple $spt soa_associated-with soa_berthMICT))
;          (True (trace! (check-inrs2 $ep $spt) True))
         )
    True))

(= (ct-triple-for-add (inrs-only-id $ep) type necessary) (inrs-only $ep))
(= (ct-triple-for-add (inrs-only-id $ep) type hold) (inrs-only $ep))
(= (meta-triple-for-add $id $ep soaHas_instrument soa_INRS)
 (let* ((True (inrs-only $ep))
;         (True (trace! (inrs1 $ep) True))
        ($id (inrs-only-id $ep)))
  True))


; Init pre
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Real World Regulatory Limitation/                                                                           ;
;                                                                                                              ;
;               (3) As of today, MiCA, EU regulatory framework, which A.P. Møller – Mærsk must adhere to,      ;
;                   has suspended INRS from the approved stablecoin list until further notice.                 ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (inrs-prohibited $v)
  (let True (ct-triple $v type soaContainerVessel) True))

(= (ct-triple-for-add (inrs-prohibited-id $v) type false)
  (inrs-prohibited $v))
(= (ct-triple-for-add (inrs-prohibited-id $v) type hold)
  (inrs-prohibited $v))
(= (meta-triple-for-add (inrs-prohibited-id $v) (payment-from-id $v) type permitted)
  (inrs-prohibited $v))
(= (ct-triple-for-add (payment-from-id $v) type soaPay)
  (inrs-prohibited $v))
(= (ct-triple-for-add $id soaHas_agent $v)
  (let* (($id (payment-from-id $v))
         (True (inrs-prohibited $v)))
     True))
(= (ct-triple-for-add (payment-from-id $v) soaHas_instrument soa_INRS)
  (inrs-prohibited $v))

; Init
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /State of Affairs/                                                                                           ;
;                                                                                                              ;
;               (4) ALEXANDRA MÆRSK is a container vessel, which moors at soa_berthMICT.                       ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_emam type soaMoor)
(ct-triple soa_emam type rexist)
(ct-triple soa_emam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_emam soaHas_location soa_berthMICT)

(ct-triple soa_ALEXANDRA_MAERSK type soaContainerVessel)
(ct-triple soa_berthMICT type soa_mooringBerth)
(ct-triple soa_sptMICT soa_associated-with soa_berthMICT)

; Init pre
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Inference/                                                                                                  ;
;                                                                                                              ;
;               From (1) and (4), it is inferred that (5) ALEXANDRA MÆRSK is obliged to pay equivalent of      ;
;               $15000 to the MICT Smart Port Treasury, i.e., soa_sptMICT.                                     ;
;                                                                                                              ;
;               From (2) and (5), it is inferred that (6) ALEXANDRA MÆRSK is obliged to pay equivalent of      ;
;               $15000 in INRS to the MICT Smart Port Treasury.                                                ;
;                                                                                                              ;
;               Finally, it is inferred that (3) and (6) are in conflict.                                      ;
;                                                                                                              ;
;               In parallel, it is inferred that (3) is necessarily violated by the fact that the              ;
;               instrument of (2) must necessarily be INRS.                                                    ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; check
;!(let True (ct-triple $a $b $c) (triple $a $b $c))
;!(let True (meta-triple $id $a $b $c) (meta $id $a $b $c))
;!(let True (ct-not $a $b) (one-of $a $b))

!(let $r (is-in-conflict-with $a $b) $r)
!(let $r (is-necessarily-violated-by $a $b) $r)
