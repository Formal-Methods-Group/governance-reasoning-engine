
; > =================================================================================== ;
;                                                                                       ;
; //Thematic/Semantic role//                                                            ;
;                                                                                       ;
;                                                                                       ;
; /Definition/     Mode of involvement of a participant in an eventuality               ;
;                                                                                       ;
; /Note 1/         Thematic/Semantic roles for specific eventualities are often         ;
;                  associated with prototypical semantic relations, e.g. if John        ;
;                  causes a breaking event, he is the Agent; if he uses a hammer,       ;
;                  it is the Instrument; and someone who receives something             ;
;                  is a Beneficiary.                                                    ;
;                                                                                       ;
; Given is an exhaustive list covering !all possible! semantic scenarios                ;
; As per ISO/LIRICS Definitions, LIRICS roles in the form of ISO data categories        ;
; As listed in ISO 24617-4:2014, Semantic annotation framework Part 4, Semantic roles   ;
;                                                                                       ;
; =================================================================================== < ;

(= (ct-ThematicRole) soaHas_agent)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_agent                                                                                                      ;
;                                                                                                              ;
; /Definition/   Participant in an event who intentionally initiates an event,                                 ;
;                and who exists independently of the event.                                                    ;
;                                                                                                              ;
; /Source/       Adapted from Dowty [1989], EAGLES, SIL, Sowa [2000] and UNL.                                  ;
;                                                                                                              ;
; /Explanation/  An agent may be animate, or only seemingly so, or perceived as animate; this is to            ;
;                ensure that cases of nonhuman agency such as a robot or an institution will not be            ;
;                excluded from being able to initiate an event (e.g. ‘GM offers rebates on its new models’).   ;
;                                                                                                              ;
; /Example/     ‘John [agent e1] built e1 the house’                                                           ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_beneficiary)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_beneficiary                                                                                                ;
;                                                                                                              ;
; /Definition/   Participant in an eventuality that is advantaged or disadvantaged by the eventuality.         ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES, Sowa [2000] and UNL                                                      ;
;                                                                                                              ;
; /Explanation/  The roles of beneficiary and recipient differ in that the recipient is the ultimate           ;
;                target of an action, whereas a beneficiary is not. They also differ in that the role of       ;
;                recipient is the ultimate goal of an action, whereas that of beneficiary is not.              ;
;                Furthermore, a recipient cannot take part in a state.                                         ;
;                                                                                                              ;
; /Example/     ‘John sold e1 the car (for a friend [beneficiary e1])’                                         ;
;               ‘He gave e1 his life for his country [beneficiary e1]’                                         ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_cause)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_cause                                                                                                      ;
;                                                                                                              ;
; /Definition/   Participant in an event that initiates the event, but does not act with any                   ;
;                intentionality or consciousness; the participant exists independently of the event.           ;
;                                                                                                              ;
; /Source/       Adapted from SIL (‘Causer’) and Sowa [2000] (‘Effector’)                                      ;
;                                                                                                              ;
; /Explanation/  Except for the lack of intentionality of the participant, this semantic role is very          ;
;                similar to that of the agent, and in fact shares all its other properties. The role of        ;
;                cause can often be identified with verbs of initiation or causation such as ‘to               ;
;                cause’, ‘to produce’, ‘to start’, ‘to originate’, ‘to occasion’ and ‘to generate’.            ;
;                                                                                                              ;
; /Example/     ‘The wind [cause e1] broke e1 the window’                                                      ;
;               ‘His talk [cause e1] produced e1 a violent reaction e2 from the crowd.’                        ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_goal)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_goal                                                                                                       ;
;                                                                                                              ;
; /Definition/   Participant in an event that is the (non-locative, non-temporal) end point of an              ;
;                action; the participant exists independently of the event.                                    ;
;                                                                                                              ;
; /Source/       Adapted from Sowa [2000]                                                                      ;
;                                                                                                              ;
; /Explanation/  Goal differs from beneficiary in that there need not be a clear benefit or                    ;
;                disadvantage for the participant; moreover, only events can have a participant                ;
;                towards whom the action is directed.                                                          ;
;                                                                                                              ;
; /Example/     ‘(The executive [agent e1]) recalls e1 ((Mr Corry [agent e2] whispering e2 (to him             ;
;               and others [goal e2]) [theme e1]).’                                                            ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_instrument)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_instrument                                                                                                 ;
;                                                                                                              ;
; /Definition/   Participant in an event that is manipulated by an agent, and with which an                    ;
;                intentional act is performed; it exists independently of the event.                           ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES (‘Implement’), SIL, Sowa [2000] and UNL                                   ;
;                                                                                                              ;
; /Explanation/  Some [Loos et al. 2004] would define instrument more simply as an inanimate                   ;
;                object used to implement an event. However, it is clear that the semantic role of             ;
;                instrument can also be animate (e.g. ‘The woman was dragged by her horse                      ;
;                [instrument] for several meters’, or even ‘John [instrument] threw himself at the             ;
;                door in a rage’), and can also take part in a state as well as an event (e.g. ‘The            ;
;                tarpaulin is tied down with rope [instrument]’).                                              ;
;                                                                                                              ;
; /Example/     ‘He opened e1 the door with the key [instrument e1]’                                           ;
;               ‘(The brick [instrument e1] [cause e2]) hit e1 the window and shattered e2 it’                 ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_partner)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_partner                                                                                                    ;
;                                                                                                              ;
; /Definition/   Participant in an event that is intentionally or consciously involved in carrying out         ;
;                the event. Participant is not the principal agent of the event, and exists                    ;
;                independently of the event.                                                                   ;
;                                                                                                              ;
; /Source/       Adapted from FrameNet, PropBank and UNL                                                       ;
;                                                                                                              ;
; /Explanation/  The semantic role of partner mainly differs from that of agent in that the participant        ;
;                in question is performing the action in accompaniment to the agent, and is not the            ;
;                primary focus. Sometimes partner is also known as ‘accompaniment’ (SIL and                    ;
;                Sowa, 2000). UNL distinguishes the roles co-agent and partner, but this distinction           ;
;                is felt to be unclear.                                                                        ;
;                                                                                                              ;
; /Example/      (Libyan leader Muammar Gaddafi [agent e1]) publicly [manner e1] announced e1                  ;
;                (his commitment e2) (to dismantle e3) (WMD programs [patient e3]) (in his                     ;
;                country) [location e3] [theme e1] [purpose e2]) following (negotiations e4 (with US           ;
;                and UK authorities [partner e4]) [reason e1])                                                 ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_patient)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_patient                                                                                                    ;
;                                                                                                              ;
; /Definition/   Participant in an event that undergoes a change of state, location or condition, is           ;
;                causally involved or directly affected by other participants, and exists                      ;
;                independently of the event.                                                                   ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES, Sowa [2000] and UNL                                                      ;
;                                                                                                              ;
; /Explanation/  Patient is distinguished from the semantic role of theme by the fact that it is               ;
;                structurally changed or affected by the event.                                                ;
;                                                                                                              ;
; /Example/      (White women [agent e1] [agent e2]) serve [e1] (tea and coffee [theme e1]), and               ;
;                then wash [e2] (the cups and saucers [patient e2]) (afterwards [time e2])                     ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_pivot)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_pivot                                                                                                      ;
;                                                                                                              ;
; /Definition/   Participant in a state that is characterised as being in a certain position or                ;
;                condition throughout that state, and has a major or central role or effect in that            ;
;                state. A pivot is more central to the state than a participant in a theme role.               ;
;                                                                                                              ;
; /Source/       Webster New Collegiate Dictionary [1976]                                                      ;
;                                                                                                              ;
; /Example/      (Vicar Marshall [agent e1, pivot e2]) admits [e1] (to mixed feelings [e2] (about this         ;
;                issue [theme e2]) [theme e1])                                                                 ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_purpose)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_purpose                                                                                                    ;
;                                                                                                              ;
; /Definition/   Participant that represents the set of facts or circumstances, which in turn describe         ;
;                what an agent wishes, or intends, to accomplish by performing some intentional action.        ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES and UNL                                                                   ;
;                                                                                                              ;
; /Explanation/  The notion of purpose implies intentional action. This property of purpose makes it           ;
;                impossible to have this semantic role in a state. The role of purpose differs from            ;
;                that of reason in that purpose describes the aims of an agent, whereas reason                 ;
;                indicates why the event is carried out or the state is true. Contrast the following           ;
;                examples: (a) ‘The authorities extradited him to the US for drug trafficking [reason]’        ;
;                and (b) ‘The authorities extradited him for trial [purpose] in the US’.                       ;
;                                                                                                              ;
; /Example/     ‘(The robber [agent e1]) (tied e1) Harry [theme e1] (to the chair [final-location e1])         ;
;                ((to stop e2) him [theme e2] (from getting away [purpose e2]) [purpose e1])’                  ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_reason)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_reason                                                                                                     ;
;                                                                                                              ;
; /Definition/   Participant that represents the set of facts or circumstances explaining why a state          ;
;                exists or an event occurs.                                                                    ;
;                                                                                                              ;
; /Source/       Adapted from Sowa [2000] (‘Matter’) and UNL                                                   ;
;                                                                                                              ;
; /Explanation/  The role of reason can be distinguished from that of purpose by the fact that the             ;
;                latter indicates the objective or goal of an agent that acts intentionally. However,          ;
;                the role of agent is not applicable to states. Reason is also different from manner           ;
;                and method in that reason describes why the event is being carried out, while                 ;
;                manner and method describe how it is being carried out.                                       ;
;                                                                                                              ;
; /Example/     ‘People love giant pandas because they look like teddy bears [reason]’                         ;
;               ‘Due to the spray from the hose [reason] the ink ran down the page before she                  ;
;                could read it.’                                                                               ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_result)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_result                                                                                                     ;
;                                                                                                              ;
; /Definition/   Participant in an event that comes into existence through the event; it indicates a           ;
;                terminal point for the event: when that is reached, the event does not continue.              ;
;                                                                                                              ;
; /Source/       Adapted from Sowa [2000]                                                                      ;
;                                                                                                              ;
; /Explanation/  Result is the completed point of a process and, unlike goal, is dependent on the              ;
;                event for its existence.                                                                      ;
;                                                                                                              ;
; /Example/     ‘(Within the past two months [duration e1]) (a bomb [cause e1]) exploded e1 (in                ;
;                the offices of El Espectador in Bogotá [location e1]), (destroying e2 (a major part           ;
;                of its installations and equipment [patient e2]) [result e1])’                                ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_setting)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_setting                                                                                                    ;
;                                                                                                              ;
; /Definition/   Participant that represents a set of facts or circumstances of the occurrence of an           ;
;                event or a state.                                                                             ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES (‘Scene’), UNL (‘Condition’) and FrameNet (‘Circumstances’)               ;
;                                                                                                              ;
; /Example/     ‘(A number of medical and agricultural research centers [pivot s1][instrumente1])              ;
;                had s1 (the potential [attribute s1]) to be used e1 (in BW research [setting e1]).’           ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_source)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_source                                                                                                     ;
;                                                                                                              ;
; /Definition/   Participant in an event that is the (non-locative, non-temporal) starting point of an         ;
;                event. The source exists independently of the event.                                          ;
;                                                                                                              ;
; /Source/       Adapted from Sowa [2000]                                                                      ;
;                                                                                                              ;
; /Example/     ‘(Eaton [beneficiary e1]) earned e1 (from continuing operations [source e1])’                  ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_theme)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_theme                                                                                                      ;
;                                                                                                              ;
; /Definition/   Participant in an eventuality in an event that is (i) essential to the event taking           ;
;                place, but does not have control over the way the event occurs and is not                     ;
;                structurally changed by the event, and (ii) characterised as being in a certain               ;
;                position or condition throughout the state, and is essential to the state being in            ;
;                effect but not as central to the state as a participant in a pivot role.                      ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES, Sowa [2000] and UNL (‘Object’)                                           ;
;                                                                                                              ;
; /Explanation/  The Theme role is distinguished from the Patient role by the fact that it is not              ;
;                structurally changed by the event or in the state in which it occurs.                         ;
;                                                                                                              ;
; /Example/     ‘(One man [agent e1]) wrapped e1 (several diamonds [theme e1]) (in the knot of                 ;
;                his tie [final-location e1])’                                                                 ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_time)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_time                                                                                                       ;
;                                                                                                              ;
; /Definition/   Participant that indicates an instant or a time interval during which a state exists or       ;
;                an event takes place.                                                                         ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES, SIL, Sowa [2000] and UNL                                                 ;
;                                                                                                              ;
; /Explanation/  As with location, time is divided into three subroles: beginning, end and duration.           ;
;                                                                                                              ;
; /Example/     ‘(Right now [time e1]) ((about a dozen [amount e1]) standardiseatories [agent                  ;
;                e1][agent e2], (in the U.S. Canada and Britain [location e1]), (are racing [e1]) (to          ;
;                unmask e2 (other suspected tumor-suppressing genes [theme e2]) [purpose e1]’                  ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_manner)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_manner                                                                                                     ;
;                                                                                                              ;
; /Definition/   Participant that represents the way or style of performing an action or the                   ;
;                degree/strength of a cognitive or emotional state.                                            ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES and UNL                                                                   ;
;                                                                                                              ;
; /Explanation/  The role of manner differs from that of instrument in that the former describes an            ;
;                event as a whole, whereas the latter characterises one of the components of the               ;
;                event or state. Manner is abstract, whereas instrument is concrete. The latter also           ;
;                differentiates instrument from means and method. The role of manner includes                  ;
;                secondary effects (quietly, loudly), and general descriptions comparing events or             ;
;                states (in the same way). It may also indicate salient characteristics of theme,              ;
;                experiencer, agent etc. (e.g. coldly, deliberately, eagerly and carefully).                   ;
;                                                                                                              ;
; /Example/     ‘The tiny stick (was fastened s1) tightly [manner s1] to his wrist.’                           ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_medium)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_medium                                                                                                     ;
;                                                                                                              ;
; /Definition/   Participant that represents the physical setting, entity or channel used by an agent          ;
;                (or agents) in an event or process.                                                           ;
;                                                                                                              ;
; /Source/       Adapted from Sowa [2000]                                                                      ;
;                                                                                                              ;
; /Explanation/  As a medium implies the presence of an agent, because it defines the physical                 ;
;                means of conveying an action, this semantic role is not relevant for states. There is         ;
;                some discussion about the difference between the semantic role of medium and                  ;
;                that of instrument. To illustrate the problem, consider the example ‘Joanna peeked            ;
;                at John through the binoculars.’ Here ‘the binoculars’ could be said to be the                ;
;                instrument used to perform the action of peeking, whereas ‘through the binoculars’            ;
;                could be said to be the medium. The main difference is that while medium is only              ;
;                ever a semantic role for a participant in an event, instrument can be present for a           ;
;                state as well.                                                                                ;
;                                                                                                              ;
; /Example/     ‘The students (heard e1) the news on (the radio [medium e1])’                                  ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_means)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_means                                                                                                      ;
;                                                                                                              ;
; /Definition/   Participant in an event that represents a procedure for performing an action in               ;
;                terms of component steps, or a methodology by which an intentional act is                     ;
;                performed by an agent. A means does not necessarily exist independently of the                ;
;                event.                                                                                        ;
;                                                                                                              ;
; /Source/       Adapted from UNL                                                                              ;
;                                                                                                              ;
; /Explanation/  Means differs from instrument in that the former describes possible activities and            ;
;                methods for doing something that may have no existence independent of the                     ;
;                event, while the latter describes previously existing tools. This distinction is              ;
;                exemplified by (a) ‘I sliced the cucumber in 1/8th inch slices with a knife                   ;
;                [instrument]’ and (b) ‘I sliced the cucumber in 1/8th inch slices by marking the              ;
;                intervals with a ruler [means]’. Sometimes this distinction is not really very clear-         ;
;                cut: (c) John ate the sushi with chopsticks [instrument]’ and (d) John ate the sushi          ;
;                ever a semantic role for a participant in an event, instrument can be present for a           ;
;                using chopsticks [means?]’.                                                                   ;
;                                                                                                              ;
; /Example/     ‘He had to button e1 his sleeve (by holding e2 the cuff in his mouth [means e1])’              ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_location)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_location                                                                                                   ;
;                                                                                                              ;
; /Definition/   Participant that represents the place where an event occurs, or a state is true,              ;
;                or a thing exists.                                                                            ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES (‘Place’), SIL (‘Locative’) and Sowa [2000]                               ;
;                                                                                                              ;
; /Example/     ‘Here [location e1] is s1 (an example [theme s1])’                                             ;
;               ‘She was cooking e1 (in the kitchen [location e1])’                                            ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_initial-location)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_initial-location                                                                                           ;
;                                                                                                              ;
; /Definition/   Participant in an event that indicates the location where an event begins or a state          ;
;                becomes true; initial-location exists independently of the event.                             ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES, Sowa [2000] (‘origin’) and UNL (‘Initial Place’)                         ;
;                                                                                                              ;
; /Example/     ‘(Half way out of the harbour [initial-location s1]) the sea becomes s1 really deep’           ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_final-location)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_final-location                                                                                             ;
;                                                                                                              ;
; /Definition/   Participant in an event that indicates the location where an event ends or a state            ;
;                becomes false; final-location exists independently of the event.                              ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES, Sowa [2000] (‘Destination’) and UNL (‘Final Place’)                      ;
;                                                                                                              ;
; /Example/     ‘(One man [agent e1]) wrapped e1 (several diamonds [theme e1]) (in the knot of                 ;
;               his tie [final-location e1])’                                                                  ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_distance)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_distance                                                                                                   ;
;                                                                                                              ;
; /Definition/   Length or extent of space.                                                                    ;
;                                                                                                              ;
; /Source/       Adapted from WordNet                                                                          ;
;                                                                                                              ;
; /Example/     ‘missiles [pivot s1] (capable s1) of (travelling e1) (more than 300 km [distance e1])’         ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_duration)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_duration                                                                                                  ;
;                                                                                                              ;
; /Definition/   Length or extent of time.                                                                     ;
;                                                                                                              ;
; /Source/       Adapted from WordNet                                                                          ;
;                                                                                                              ;
; /Example/     ‘Terry [agent e1] jogged e1 (for two hours [duration e1])’                                     ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_initial-time)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_initial-time                                                                                               ;
;                                                                                                              ;
; /Definition/   Participant that indicates when an event begins or a state becomes true.                      ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES, Sowa [2000] and UNL                                                      ;
;                                                                                                              ;
; /Example/     ‘(Harry [agent e1]) (teaches e1) (on Friday (from 14:45 [initial-time e1]) (to 12:30           ;
;                [final-time e1]) [time e1])’                                                                  ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_final-time)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_final-time                                                                                                 ;
;                                                                                                              ;
; /Definition/   Participant that indicates when an event ends or a state ceases to be true.                   ;
;                                                                                                              ;
; /Source/       Adapted from EAGLES, Sowa [2000] and UNL                                                      ;
;                                                                                                              ;
; /Example/     ‘(Harry [agent e1]) (teaches e1) (on Friday (from 14:45 [initial-time e1]) (to 12:30           ;
;                final-time e1]) [time e1])’                                                                   ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_path)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_path                                                                                                       ;
;                                                                                                              ;
; /Definition/   Participant that indicates an intermediate location or trajectory between two                 ;
;                locations, or in a designated space.                                                          ;
;                                                                                                              ;
; /Source/       Adapted from Sowa [2000]                                                                      ;
;                                                                                                              ;
; /Example/     ‘(The baby [agent e1]) crawled e1 (across the floor [path e1])’                                ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_amount)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_amount                                                                                                     ;
;                                                                                                              ;
; /Definition/   Participant denoting a quantity of something other than time or space.                        ;
;                                                                                                              ;
; /Source/       Adapted from PropBank (‘Extent’), FrrameNet (‘Amount’), UNL (‘Quantity’), Sowa                ;
;                [2000] (‘Amount’, ‘Measure’) and EAGLES (‘Quantity’)                                          ;
;                                                                                                              ;
; /Example/     ‘(The euro [theme s1]) (is worth s1) (nearly one and a half dollars [amount s1])’              ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;

(= (ct-ThematicRole) soaHas_attribute)

; > –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– ;
; *_attribute                                                                                                  ;
;                                                                                                              ;
; /Definition/   Property of an entity or entities.                                                            ;
;                                                                                                              ;
; /Source/       Adapted from FrameNet (‘Parameter’), UNL, Sowa [2000] and EAGLES                              ;
;                                                                                                              ;
; /Example/     ‘(We [agent e1]) will (paint e1) (the front door [theme e1]) (dark green [attribute            ;
;                e1])’                                                                                         ;
;                                                                                                              ;
; –––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––– < ;
