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

![scheme 8 10](https://github.com/user-attachments/assets/eadb6c8a-0a71-4c9a-8134-a27772f4b159)


## [Knowledge Representation](https://github.com/Formal-Methods-Group/governance-reasoning-engine/tree/main/knowledge)


![scheme 2](https://github.com/user-attachments/assets/1971f5a3-fab0-49df-adbe-b099dc9485b6)


## [Reasoning](https://github.com/Formal-Methods-Group/governance-reasoning-engine/tree/main/reason)


![scheme 3](https://github.com/user-attachments/assets/766d339d-6fcc-4d43-98ec-e49bc193e998)


## [example/1_stakeholder_lying_detect.metta](https://github.com/Formal-Methods-Group/governance-reasoning-engine/blob/main/example/1_stakeholder_lying_detect.metta)

![3 10_1](https://github.com/user-attachments/assets/e7ba744b-b75c-4180-ba9a-85ea35018c1b)

## [example/2_smart_port_example.metta](https://github.com/Formal-Methods-Group/governance-reasoning-engine/blob/main/example/2_smart_port_example.metta)

![3 10_2](https://github.com/user-attachments/assets/51aa5461-c606-4f6f-9eb7-e3b0fd3ee8a9)

