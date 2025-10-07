
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
; /Regulation/                                                                                                 ;
;                                                                                                              ;
;               (1) It is not optional for ALEXANDRA MÆRSK to pay in INRS, i.e., it is either obligatory or    ;
;                   prohibited, but we don't know which one of the two                                         ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_eniamm type false)
(ct-triple soa_eniamm type hold)
(meta-triple soa_eniamm soa_epiam type optional)
(ct-triple soa_epiam type soaPay)
(ct-triple soa_epiam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_epiam soaHas_instrument soaINRS)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Regulation/                                                                                                 ;
;                                                                                                              ;
;               (2) ALEXANDRA MÆRSK is permitted to pay in INRS                                                ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_epiam type permitted)
(ct-simple-not soa_eniam soa_epiam)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /State of Affairs/                                                                                           ;
;                                                                                                              ;
;               (3) ALEXANDRA MÆRSK pays equivalent of 15000USD in INRS
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_ep15kiam type soaPay)
(ct-triple soa_ep15kiam type rexist)
(ct-triple soa_ep15kiam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_ep15kiam soaHas_amount (15000USD))
(ct-triple soa_ep15kiam soaHas_instrument soaINRS)

; Init
!(make-triples)

; Debug
;!(let True (ct-triple $a $b $c) (triple $a $b $c))
;!(let True (meta-triple $id $a $b $c) (meta $id $a $b $c))
;!(let True (ct-simple-not $a $b) (one-of $a $b))
;!(let True (ct-disjunction $a $b) (disj $a $b))
;!(let True (mod_not $ne $ddm) ($ne $ddm))

; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Inference/                                                                                                 ;
;                                                                                                             ;
;               From (1), that (4) "ALEXANDRA MÆRSK is obliged to pay in INRS or                              ;
;                                   ALEXANDRA MÆRSK is obliged to not pay in INRS" [¬OP(p)=>(OB(p)∨OB(¬p))]   ;
;                                                                                                             ;
;               From (2), that (5) "ALEXANDRA MÆRSK is not obliged to not pay in INRS" [PE(p)=>¬OB(¬p)]       ;
;                                                                                                             ;
;               From (4) and (5) that (6) "ALEXANDRA MÆRSK is obliged to pay in INRS"                         ;
;                                          [via the Disjunctive Syllogism at the level of the statements]     ;
;                                                                                                             ;
;               Finally, it is inferred that (3) complies with (6).                                           ;
;                                                                                                             ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; Check
!(is_complied_with_by $eo $e)
