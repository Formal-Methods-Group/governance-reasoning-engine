
; > =================================================================================================================== ;
;                                                                                                                       ;
;   Setting: Smart Port Halo Agent monitors and controls a fully automated                                              ;
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
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Regulation/                                                                                                   ;
;                                                                                                                ;
;               (1) It is obligatory to pay with INRS encoded as                                                 ;
;                   Every container vessel is obliged to pay with INRS                                           ;
;                                                                                                                ;                                                 ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ob-inrs $u)
  (let* ((True (ct-triple $u type soaContainerVessel)))
    True))

(= (ct-triple-for-add $id type soaPay)
  (let* ((True (ob-inrs $u))
         ($id (ob-inrs-id $u)))
    True))
(= (ct-triple-for-add $id type obligatory)
  (let* ((True (ob-inrs $u))
         ($id (ob-inrs-id $u)))
    True))
(= (ct-triple-for-add $id soaHas_agent $u)
  (let* ((True (ob-inrs $u))
         ($id (ob-inrs-id $u)))
    True))
(= (ct-triple-for-add $id soaHas_instrument soa_INRS)
  (let* ((True (ob-inrs $u))
         ($id (ob-inrs-id $u)))
    True))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Regulation/                                                             ;
;                                                                          ;
;               (2) It is obligatory to pay by USDS, encoded as            ;
;                   Every container vessel is obliged to pay with USDS     ;                                                                                            ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ob-usds $u)
  (let* ((True (ct-triple $u type soaContainerVessel)))
    True))

(= (ct-triple-for-add $id type soaPay)
  (let* ((True (ob-usds $u))
         ($id (ob-usds-id $u)))
    True))
(= (ct-triple-for-add $id type obligatory)
  (let* ((True (ob-usds $u))
         ($id (ob-usds-id $u)))
    True))
(= (ct-triple-for-add $id soaHas_agent $u)
  (let* ((True (ob-usds $u))
         ($id (ob-usds-id $u)))
    True))
(= (ct-triple-for-add $id soaHas_instrument soa_USDS)
  (let* ((True (ob-usds $u))
         ($id (ob-usds-id $u)))
    True))

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Domain Knowledge/                                                                                          ;
;                                                                                                              ;
;   The following domain knowledge rule states that if the payment has been done by INRS, it is false that     ;
;   it has been done by USDS.                                                                                  ;
;   In Hobbs': Ve[(Pay(e)^has-instrument(e,inrs))=>¬has-instrument(e,usds)].                                   ;                                       ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (inrs-not-usds $e)
  (let* ((True (ct-triple $e soaHas_instrument soa_INRS)))
    True))

(= (ct-triple-for-add $id type $t)
   (let* ((True (inrs-not-usds $e))
          ($id (inrs-not-usds-id $e))
          ($t (superpose (false hold))))
     True))
(= (meta-triple-for-add $id $e soaHas_instrument soa_USDS)
   (let* ((True (inrs-not-usds $e))
          ($id (inrs-not-usds-id $e))
          ($t (superpose (false hold))))
     True))


; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Domain Knowledge/                                                                                          ;
;                                                                                                              ;
;   The following domain knowledgerule states that if the payment has been done by USDS, it is false that      ;
;   it has been done by INRS.                                                                                  ;
;   In Hobbs': Ve[(Pay(e)^has-instrument(e,usds))=>¬has-instrument(e,inrs)].                                   ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (usds-not-inrs $e)
  (let* ((True (ct-triple $e soaHas_instrument soa_USDS)))
    True))

(= (ct-triple-for-add $id type $t)
   (let* ((True (usds-not-inrs $e))
          ($id (usds-not-inrs-id $e))
          ($t (superpose (false hold))))
     True))
(= (meta-triple-for-add $id $e soaHas_instrument soa_INRS)
   (let* ((True (usds-not-inrs $e))
          ($id (usds-not-inrs-id $e))
          ()$t (superpose (false hold)))
     True))


; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /State of Affairs/                                                                                          ;
;                                                                                                              ;
;               (3) ALEXANDRA MÆRSK is a container vessel.                                                     ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple ALEXANDRA_MAERSK type soaContainerVessel)

; Init
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /NB/                                                                                                        ;
;               The state of affairs includes two implications stating that if the instrument for paying       ;
;               is INRS then it is not USDS and if the instrument is USDS then it is not INRS.                 ;
;                                                                                                              ;
;  /Inference/                                                                                                 ;
;                                                                                                              ;
;               From (1), (2), and (3) it's inferred that (5) ALEXANDRA MÆRSK is obliged to pay in INRS        ;
;               and (6) ALEXANDRA MÆRSK is obliged to pay by USDS.                                             ;
;               From (5) and (6) that the two obligatory actions of ALEXANDRA MÆRSK are connected by "not"     ;
;               This is done through rule inferring that (5) and (6) are two instantiations of the same        ;
;               abstract eventuality and, in particular, that they are connected by the "not" property.        ;
;               This is actually a pragmatic implicature.                                                      ;
;                                                                                                              ;
;               It's inferred through the rules of the Deontic Traditional Scheme that they are both           ;
;               permitted and not permitted, and, finally, that there is conflict between the facts that       ;
;               they and their negations are both permitted and not permitted.                                 ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; check
;!(let True (ct-triple $a $b $c) (triple $a $b $c))
;!(let True (meta-triple $id $a $b $c) (meta $id $a $b $c))
;!(let True (ct-not $a $b) (one-of $a $b))

!(let $r (is-in-conflict-with $a $b) $r)
