
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
; /State of Affairs/                                                                                           ;
;                                                                                                              ;
;               (1) ALEXANDRA MÆRSK leaves OR ALEXANDRA MÆRSK moors AND pays.                                  ;
;                                                                                                              ;
;               (2) ALEXANDRA MÆRSK does not leave.                                                            ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_eo type rexist)
(= (ct-or soa_eo) (soa_elam soa_ea))
(= (ct-and soa_ea) (soa_emam soa_epam))

(ct-triple soa_elam type soaLeave)
(ct-triple soa_elam soaHas_agent soa_ALEXANDRA_MAERSK)

(ct-triple soa_emam type soaMoor)
(ct-triple soa_emam soaHas_agent soa_ALEXANDRA_MAERSK)

(ct-triple soa_epam type soaPay)
(ct-triple soa_epam soaHas_agent soa_ALEXANDRA_MAERSK)

(ct-simple-not soa_enlam soa_elam)
(ct-triple soa_enlam type rexist)

;!(let* ((True (ct-not $e2 $e1))) ($e1 $e2))
;!(let* (($l (ct-or $eo)) ($e2 (superpose $l)) ($e1 (superpose $l)) (False (== $e2 $e1))) ($e1 $e2 $l $eo))
;!(let* (($rom (superpose (rexist obligatory))) ($l (ct-or $eo)) ($e2 (superpose $l)) ($e1 (superpose $l)) (False (== $e2 $e1)) (True (ct-not $e2 $e1)) (True (ct-triple $eo type $rom))) ($l $eo $rom))

; Init
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Inference/                                                                                                  ;
;                                                                                                              ;
;       It is inferred that "ALEXANDRA MÆRSK moors" and "ALEXANDRA MÆRSK pays".                                ;
;       This is achieved by inferring, from (2), that "ALEXANDRA MÆRSK leaves" holds false,                    ;
;       then from this and (1) it inferred that "ALEXANDRA MÆRSK moors and pays" holds true and then           ;
;       that "ALEXANDRA MÆRSK moors" and "ALEXANDRA MÆRSK pays" really exist.                                  ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; check
!(let True (ct-triple $a $b $c) (triple $a $b $c))
!(let True (meta-triple $id $a $b $c) (meta $id $a $b $c))
