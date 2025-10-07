; Sample MeTTa file for testing the modular inference library
; This demonstrates basic inference patterns

(Entity agent1)
(Entity action1)
(Entity rule1)

; Define some relationships
(obliged agent1 action1)
(forbidden agent1 action1)

; This should create a conflict
(conflict rule1)

; Test compliance
(is_complied_with_by rule1 action1)

; Output for testing
!(println "Test example loaded successfully")