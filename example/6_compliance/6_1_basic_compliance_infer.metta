
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
;               (1) ALEXANDRA MÆRSK is prohibited to not pay, i.e. ALEXANDRA MÆRSK is not permitted to not pay ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_enpamm type false)
(ct-triple soa_enpamm type hold)
(meta-triple soa_enpamm soa_enpam type permitted)
(ct-triple soa_enpam type soaPay)
(ct-triple soa_enpam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-simple-not soa_enpam soa_epam)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /State of Affairs/                                                                                           ;
;                                                                                                              ;
;               (2) ALEXANDRA MÆRSK pays 15000USD                                                              ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_epam15k type soaPay)
(ct-triple soa_epam15k type rexist)
(ct-triple soa_epam15k soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_epam15k soaHas_amount (15000USD))

; Init
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Inference/                                                                                                  ;
;                                                                                                              ;
;               From (1), it is inferred through the inference rules implementing                              ;
;               the Deontic Traditional Scheme, specifically by the one implementing ¬PE(p)=>OB(¬p),           ;
;               that (3) ALEXANDRA MÆRSK is obliged to pay.                                                    ;
;                                                                                                              ;
;               Then, from (2) and (3), it is inferred that (2) complies with (3).                             ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; Debug
;!(let True (ct-triple $a $b $c) (triple $a $b $c))
;!(let True (meta-triple $id $a $b $c) (meta $id $a $b $c))
;!(let True (ct-simple-not $a $b) (one-of $a $b))
;!(let True (mod_not $ne $ddm) ($ne $ddm))

; Check
!(is_complied_with_by $eo $e)
