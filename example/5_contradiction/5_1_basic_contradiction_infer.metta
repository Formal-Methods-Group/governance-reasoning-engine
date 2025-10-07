
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
;               (1) ALEXANDRA MÆRSK moors                                                                      ;                                                            ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_emam type rexist)
(ct-triple soa_emam type soaMoor)
(ct-triple soa_emam soaHas_agent soa_ALEXANDRA_MAERSK)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /State of Affairs/                                                                                           ;
;                                                                                                              ;
;               (2) ALEXANDRA MÆRSK does not moor                                                              ;                                                          ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-simple-not soa_enmam soa_emam)
(ct-triple soa_enmam type rexist)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /State of Affairs/                                                                                           ;
;                                                                                                              ;
;               (3) LAURA MÆRSK pays equivalent of $15000 in INRS                                              ;                                                             ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_eplm type rexist)
(ct-triple soa_eplm type soaPay)
(ct-triple soa_eplm soaHas_agent soa_LAURA_MAERSK)
(ct-triple soa_eplm soaHas_amount (15000USD))
(ct-triple soa_eplm soaHas_instrument soa_INRS)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /State of Affairs/                                                                                           ;
;                                                                                                              ;
;               (4) LAURA MÆRSK doesn't pay equivalent of $15000 in INRS                                       ;                                                             ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-simple-not soa_enplm soa_eplm)
(ct-triple soa_enplm type rexist)

; Init
!(make-triples)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; /Inference/                                                                                                  ;
;                                                                                                              ;
;               It's inferred that                                                                             ;
;                                                                                                              ;
;               The fact that                                                                                  ;
;                               (ct-triple soa_emam type rexist) — Rexist(soa_emam) in Hobbs's logic —         ;
;               holds true                                                                                     ;
;                                           is in contradiction with                                           ;
;               the fact that                                                                                  ;
;                               the same triple                                                                ;
;               holds false, i.e.,                                                                             ;
;                               ¬Rexist(soa_emam) in Hobbs's logic                                             ;
;                                                                                                              ;
;                               ¬Rexist(soa_emam) is derived from                                              ;
;                                                                   (ct-triple soa_enmam type rexist)          ;
;                                                                                   and                        ;
;                                                                   (ct-simple-not soa_enmam soa_emam)         ;
;                                                                                                              ;
;                                                Similarly                                                     ;
;               the fact that                                                                                  ;
;                               (ct-triple soa_eplm type rexist)                                               ;
;               holds true                                                                                     ;
;                                           is in contradiction with                                           ;
;               the fact that                                                                                  ;
;                               the same triple                                                                ;
;               holds false.                                                                                   ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; check
!(is_in_contradiction_with $a $b)
