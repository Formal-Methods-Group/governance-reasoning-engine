Unified Governance Knowledge Representation and Reasoning Framework implemented in [MeTTa language for cognitive computations](https://metta-lang.dev/).

[Basis](https://www.cambridge.org/core/books/formal-theory-of-commonsense-psychology/20289940AFB026AB3EF31EBCF8875628#fndtn-information): a first-order logical framework for Natural Language Semantics massively grounded on the notion of reification.

Reification is the action of conceptualising abstract entities as things of the world. 

In formal logic, this amounts at formalising abstract entities as first-order individuals, i.e., constants or variables of the logic. 

The Framework is massively grounded on the notion of reification, meaning that any abstract entity can be reified into a first-order individual; then, new assertions can be made on these individuals and the fact that one of these assertions holds for one of these individuals can be recursively reified again into a new first-order individual.

The Framework is powered by massive reification, abstract eventuality and instantiations handling, [ISO 24617-4:2014 role set](https://www.iso.org/standard/56866.html), Deontic and Rexist modalities, Hobbs' logic, [Deontic Traditional Scheme](https://plato.stanford.edu/entries/logic-deontic/index.html#fighex), 3 reasoning levels, and explicit representation of judgements. 

Augmented with the C++ layer opening road to Autonomous Cumulative Misalignment Learning -> Gradual Governance Automation.

## Docker 

Image: https://hub.docker.com/r/pvl8/governance-reasoning-engine

Built and tested on macOS 15.5 ARM64.

> **⚠️ Memory Requirements**: This Docker image requires significant memory allocation (16GB+ recommended). Some examples, particularly `example/4_mututally_exclusive_detect.metta`, are memory-intensive and may be killed if insufficient memory is available.

> **Note**: Pretty output formatting is very much work in progress. Using `-r` or `--raw` flag is recommended.

For 2_smart_port_example.metta example case run 

```
docker run --rm governance-reasoning-engine:latest metta_cli -r /app/example/2_smart_port_example.metta
```

## Road to Gradual Governance Automation. Big picture

<img width="3624" height="7730" alt="__1" src="https://github.com/user-attachments/assets/6e5ed383-9c3e-48b7-aa57-0934a5e58903" />

## [Knowledge Representation](https://github.com/Formal-Methods-Group/governance-reasoning-engine/tree/main/knowledge)

<img width="3624" height="4152" alt="PVL_knowlege_scheme_10 25" src="https://github.com/user-attachments/assets/bb1560bc-23b4-4372-a939-1f29df761a3a" />


## [Reasoning](https://github.com/Formal-Methods-Group/governance-reasoning-engine/tree/main/reason)

<img width="3624" height="4152" alt="PVL_reason_scheme_10 25" src="https://github.com/user-attachments/assets/e5361874-2520-4488-808a-2dbacbaa5eae" />


## [example/1_stakeholder_lying_detect.metta](https://github.com/Formal-Methods-Group/governance-reasoning-engine/blob/main/example/1_stakeholder_lying_detect.metta)

<img width="2253" height="5727" alt="PVL_port_1_scheme_10 25" src="https://github.com/user-attachments/assets/1f72b304-569e-465d-8301-860b9119dcad" />


## [example/2_smart_port_example.metta](https://github.com/Formal-Methods-Group/governance-reasoning-engine/blob/main/example/2_smart_port_example.metta)

<img width="2253" height="5727" alt="_2" src="https://github.com/user-attachments/assets/3ad54bfb-190f-41c1-8f00-844532c01f4f" />

