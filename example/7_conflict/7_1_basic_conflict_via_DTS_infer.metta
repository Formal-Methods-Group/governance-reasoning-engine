
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
;               (1) ALEXANDRA MÆRSK is obliged to not pay in INRS                                                ;
;                                                                                                                ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-simple-not soa_enpiam soa_epiam)

(ct-triple soa_enpiam type obligatory)

(ct-triple soa_epiam type soaPay)
(ct-triple soa_epiam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_epiam soaHas_instrument soaINRS)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Regulation/                                                                                                   ;
;                                                                                                                ;
;               (2) ALEXANDRA MÆRSK is obliged to pay equivalent of 15000USD in INRS                             ;
;                                                                                                                ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_ep15iam type soaPay)
(ct-triple soa_ep15iam type obligatory)
(ct-triple soa_ep15iam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_ep15iam soaHas_amount (15000USD))

; Init
!(make-triples)

; Debug
;!(let True (ct-triple $a $b $c) (triple $a $b $c))
;!(let True (meta-triple $id $a $b $c) (meta $id $a $b $c))
;!(let True (ct-simple-not $a $b) (one-of $a $b))
;!(let True (ct-disjunction $a $b) (disj $a $b))
;!(let True (mod_not $ne $ddm) ($ne $ddm))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Inference/                                                                                                                       ;
;                                                                                                                                   ;
;               From (1) it's inferred that (3) ALEXANDRA MÆRSK is not permitted to pay in INRS [OB(¬p)=>¬PE(p)]                    ;
;                                                                                                                                   ;
;               From (2) it's inferred that (4) ALEXANDRA MÆRSK is permitted to pay equivalent of 15000USD in INRS [OB(p)=>PE(p)]   ;
;                                                                                                                                   ;
;               Finally, it is inferred that (3) and (4) are in conflict with each other.                                           ;
;                                                                                                                                   ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; Check
!(is-in-conflict-with $eo $e)
