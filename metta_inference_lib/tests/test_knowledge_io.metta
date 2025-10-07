; Test file for knowledge_io s-expression parsing

; ========== NORMS ==========

(= (pay-obligatory $v $spt)
  (let* (($x (triple $v type rexist))
         ($y (triple $spt type smartport)))
    True))

(= (moor-forbidden $v $spt)
  (let* (($z (triple $v type vessel)))
    True))

; ========== STATE OF AFFAIRS ==========

(ct-triple soa_emam type soaMoor)
(ct-triple soa_emam type rexist)
(ct-triple soa_emam soaHas_agent soa_ALEXANDRA_MAERSK)
(ct-triple soa_emam soaHas_location soa_SPT)

(meta-triple entity1 property value)