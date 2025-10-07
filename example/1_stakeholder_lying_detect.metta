
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
; > ================================= ;
;                                     ;
; // Smart Port Halo Agent case//     ;
;                                     ;
; =================================== ;
;
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Unified Sky Knowledge Representation Framework/                                                            ;
;                                                                                                              ;
;               Statements, incl.                                                                              ;
;                                                                                                              ;
;               (1) State of Affairs                                                                           ;
;               (2) Real World Environment Limitation                                                          ;
;               (3) and (4) Mutually connected Domain Knowledge                                                ;
;                                                                                                              ;
;               uniformly represented within the massive reification-based                                     ;
;               Unified Sky Knowledge Representation Framework.                                                ;
;                                                                                                              ;                                                                                                              ;
;               The State of Affairs contains then the vessel soa_ALEXANDRA_MAERSK,                            ;
;               which moored at soa_berthMICT, the berth at MICT.                                              ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;
;
; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /State of Affairs/                                                                                          ;
;                                                                                                              ;
;               (1) ALEXANDRA MÆRSK declares payment in USDS to the Smart Port Treasury of MICT.               ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(ct-triple soa_epmuam type soaPay)
(ct-triple soa_epmuam type rexist)
(ct-triple soa_epmuam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_epmuam soaHas_goal soa_sptMICT)
(ct-triple soa_epmuam soaHas_instrument soa_USDS)

(ct-triple soa_berthMICT type soa_mooringBerth)
(ct-triple soa_sptMICT soa_associated-with soa_berthMICT)


; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Real World Environment Limitation/                                                                         ;
;                                                                                                              ;
;              (2) MICT Smart Port Treasury, following Reserve Bank of India (RBI) recommendations,            ;
;                  only accepts INRS, the Indian rupee-pegged stablecoin.                                      ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (inrs-only $ep)
  (let* ((True (ct-triple $ep type soaPay))
         (True (ct-triple $ep soaHas_goal $spt))
         (True (ct-triple $spt soa_associated-with soa_berthMICT)))
    True))

(= (ct-triple-for-add (inrs-only-id $ep) type necessary)
  (inrs-only $ep))
(= (ct-triple-for-add (inrs-only-id $ep) type hold)
  (inrs-only $ep))
(= (meta-triple-for-add $id $ep soaHas_instrument soa_INRS)
  (let* ((True (inrs-only $ep))
         ($id (inrs-only-id $ep)))
    True))


; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Domain Knowledge/                                                                                          ;
;                                                                                                              ;
;               Domain knowledge rules state:                                                                  ;
;                                                                                                              ;
;           (3) if the payment has been done in INRS, it is false that it has been done in USDS.               ;
;               In Hobbs' logic: Ve[(Pay(e)^has-instrument(e,inrs))=>¬has-instrument(e,usds)].                 ;
;                                                                                                              ;
;           (4) if the payment has been done in USDS, it is false that it has been done in INRS.               ;
;               In Hobbs' logic: Ve[(Pay(e)^has-instrument(e,usds))=>¬has-instrument(e,inrs)].                 ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (inrs-not-usds $e $i)
  (let ($i $ni) (superpose ((soa_INRS soa_USDS) (soa_USDS soa_INRS)))
    (ct-triple $e soaHas_instrument $ni)))

(= (ct-triple-for-add (inrs-not-usds-id $e $i) type false) (inrs-not-usds $e $i))
(= (ct-triple-for-add (inrs-not-usds-id $e $i) type hold) (inrs-not-usds $e $i))
(= (meta-triple-for-add $id $e soaHas_instrument $i)
  (let* ((True (inrs-not-usds $e $i))
         ($id (inrs-not-usds-id $e $i)))
    True))


; Init
!(make-triples)


; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /NB/                                                                                                                     ;
;               The state of affairs includes two pieces of Domain Knowledge stating that if the instrument                 ;
;               for paying is INRS then it is not USDS and if the instrument is USDS then it is not INRS.                   ;
;                                                                                                                           ;
;  /Inference/                                                                                                              ;
;                                                                                                                           ;
;               It's inferred from The Real World Environment Limitation that ALEXANDRA MÆRSK pays in INRS to               ;
;               the MICT Smart Port Treasury (since it is necessary to do so).                                              ;
;                                                                                                                           ;
;               Then it's inferred from Domain Knowledge that ALEXANDRA MÆRSK did not pay to the MICT Smart Port Treasury   ;
;               neither in INRS (payment in USDS has been declared)                                                         ;
;               nor in USDS (payment in INRS has been inferred from the Real World Environment Limitation).                 ;
;                                                                                                                           ;
;               There is of course a contradiction. However, a naive rule inferring contradiction from                      ;
;               simultaneous existance (Rexist modality) and non-existance (not Rexist modality) of the eventuality         ;
;               is unable to detect such contradiction.                                                                     ;
;                                                                                                                           ;
;               Therefore, a Contradiction judgement of different kind introduced (see: reason/judgement_level.metta):      ;
;               it searches for eventualities having a specific value for one of their roles and                            ;
;               for which, simultaneously, the knowledge graph asserts that that role does not                              ;
;               have that value                                                                                             ;
;                                                                                                                           ;
;               the rule infers: the fact that the eventuality has that value on that                                       ;
;               role is in contradiction with the fact that it does not have it.                                            ;
;                                                                                                                           ;
;               What would we conclude from the case in question? That either a stakeholder (ALEXANDRA MÆRSK)              ;
;               lied when declaring payment or that there is an error in the database, namely it's written there            ;
;               that at MICT it is necessary to pay in INRS, but this is not true: payments are also possible in USDS.      ;
;                                                                                                                           ;
; ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

; check
;!(let True (ct-triple $a $b $c) (triple $a $b $c))
;!(let True (meta-triple $id $a $b $c) (meta $id $a $b $c))
;!(let True (ct-not $a $b) (one-of $a $b))

!(let $r (is_in_contradiction_with $a $b) $r)


; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
;  /Future direction/                                                                                                         ;
;                                                                                                                             ;
;               Thus we've demonstrated the power of explicit representation of abnormalities:                                ;
;                                                                                                                             ;
;               fallacies,                                                                                                    ;
;               violations,                                                                                                   ;
;               mistakes,                                                                                                     ;
;               misalignment                                                                                                  ;
;               etc.                                                                                                          ;
;                                                                                                                             ;
;               fit to reason about them.                                                                                     ;
;                                                                                                                             ;
;               Reification provides a straightforward and effective way to achieve so.                                       ;
;                                                                                                                             ;
;               Thanks to reification, it is possible to replicate and automate such inferences.                              ;
;                                                                                                                             ;
;               It's possible to add OR learn judgements that                                                                 ;
;               e.g. after this particular Contradiciton judgement                                                            ;
;               infer that a stakeholder (e.g. ALEXANDRA MÆRSK) is acting in misalignment (e.g. lying)                        ;
;               about the eventuality that they had declared earlier                                                          ;
;                                                                                                                             ;
;               or infer that it's erroneously assumed that                                                                   ;
;               for a particular eventuality (ALEXANDRA MÆRSK paying to the Smart Port Treasury of MICT)                      ;
;               it is necessary for particular role (instrument in that case) to have a particular value (INRS).              ;
;                                                                                                                             ;
; > ––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
