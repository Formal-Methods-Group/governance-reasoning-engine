
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
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /State of Affairs/                                                                                          ;
;                                                                                                              ;
;               (1) ALEXANDRA MÆRSK pays in INRS                                                               ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_epamINRS type soaPay)
(ct-triple soa_epamINRS type rexist)
(ct-triple soa_epamINRS soaHas_instrument soaINRS)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /State of Affairs/                                                                                          ;
;                                                                                                              ;
;               (2) ALEXANDRA MÆRSK pays in USDS                                                               ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_epamUSDS type soaPay)
(ct-triple soa_epamUSDS type rexist)
(ct-triple soa_epamUSDS soaHas_instrument soaUSDS)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Domain Knowledge/                                                                                          ;
;                                                                                                              ;
;               It is assumed that                                                                             ;
;                                   (1) and (2) refer to the same payment by ALEXANDRA MÆRSK                   ;
;                                                   — in Hobbs logic terms —                                   ;
;                                   (1) and (2) are different instantiations                                   ;
;                                                                            of the same abstract eventuality  ;
;                                                                                                              ;
;                                   The two instantiations differ only for                                     ;
;                                                   Instrument: INRS rather than USDS                          ;
;                                                               incompatible with each other:                  ;
;                                                               one transaction can only be made in            ;
;                                                               one stablecoin.                                ;
;                                                                                                              ;
;               The following domain knowledge states:                                                         ;
;                                    if the payment has been done by INRS                                      ;
;                                                           it is false that it has been done by USDS.         ;
;               In Hobbs logic:                                                                                ;
;                       Ve[(Pay(e)^has-instrument(e,INRS))=> ¬has-instrument(e,USDS)].                         ;
;                                                                                                              ;
;                                                AND                                                           ;
;                                                                                                              ;
;               if the payment has been done by USDS,                                                          ;
;                                   it is false that it has been done by INRS.                                 ;
;                                                                                                              ;
;               In Hobbs logic:                                                                                ;
;                       Ve[(Pay(e)^has-instrument(e,USDS))=>¬has-instrument(e,INRS)].                          ;                                                                                      ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (soa-INRS-not-USDS $e $t)
 (let* ((($t $nt) (superpose ((soaINRS soaUSDS) (soaUSDS soaINRS))))
        (True (ct-triple $e soaHas_instrument $nt)))
   True))

(= (ct-triple-for-add $id type false)
  (let* ((True (soa-INRS-not-USDS $e $t))
         ($id (soa-INRS-not-USDS-id $e $t)))
    True))
(= (ct-triple-for-add $id type hold)
  (let* ((True (soa-INRS-not-USDS $e $t))
         ($id (soa-INRS-not-USDS-id $e $t)))
    True))
(= (meta-triple-for-add $id $e soaHas_instrument $t)
  (let* ((True (soa-INRS-not-USDS $e $t))
         ($id (soa-INRS-not-USDS-id $e $t)))
    True))

; Init
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Inference/                                                                                                  ;
;                                                                                                              ;
;       From (1) it's inferred                                                                                 ;
;                              (3) ALEXANDRA MÆRSK does not pay in USDS, i.e.,                                 ;
;                                       the fact                                                               ;
;                                  ALEXANDRA MÆRSK pays by USDS                                                ;
;                                                               does not really exist.                         ;
;                                                                                                              ;
;       From (2) it's inferred                                                                                 ;
;                              (4) ALEXANDRA MÆRSK does not pay in INRS, i.e.,                                 ;
;                                       the fact                                                               ;
;                                  ALEXANDRA MÆRSK pays by INRS                                                ;
;                                                               does not really exist.                         ;
;                                                                                                              ;
;       Finally, it's inferred                                                                                 ;
;                              (2) and (3) are in contradiction                                                ;
;                                             and                                                              ;
;                               (1) and (4) are in contradiction.                                              ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; Debug
;!(let True (soa-INRS-not-USDS $e $t) ($e $t))
;!(let True (ct-triple $a $b $c) ($a $b $c))
;!(let True (meta-triple $id $a $b $c) ($id $a $b $c))
;!(let True (ct-simple-not $a $b) ($a $b))

; Check
!(is_in_contradiction_with $v $r)
