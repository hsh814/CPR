# CPR Benchmark
## Setup
```shell
export PATH=/root/projects/CPR/scripts:$PATH
# 0. Generate meta-data.json with scripts/meta.py (already done)
# python3 scripts/meta.py
# 1. Genenrate patches
python3 scripts/sympatch.py reset patches
python3 scripts/sympatch.py concrete patches
python3 scripts/sympatch.py meta patches

# 2. Build (for single)
symfeas.py init 5321 # Run ./init.sh
symfeas.py val-build 5321 # ./val.sh
symfeas.py fuzz-build 5321 # ./aflrun.sh
# 2. Build (for all)
experiments.py fuzz --extra init
experiments.py fuzz --extra val-build
experiments.py fuzz --extra fuzz-build

# 3. Run test (for single)
symvass.py rerun 5321
symvass.py rerun 5321 --sym-level=high --max-fork=1024,1024,128
# 3. Run test (for all)
experiments.py exp
experiments.py exp --extra high

# 4. Run fuzzer (for single)
symfeas.py fuzz 5321
symfeas.py collect-inputs 5321
# 4. Run fuzzer (for all)
experiments.py fuzz
experiments.py fuzz --extra collect-inputs

# 5. Symbolic input validation (for single)
symvass.py symgroup 5321
symfeas.py val 5321
symfeas.py feas 5321
# 5. Symbolic input validation (for all)
experiments.py analyze --extra symgroup
experiments.py fuzz --extra val
exepriments.py fuzz --extra feas

# 6. Analyze: Check ./out directory and get results
experiments.py final --prefix <your-prefix-for-experiments>
```

## Test
First, filter out patches that fails on test
```shell
python3 scripts/meta-program.py filter 5321
```


```shell
python3 scripts/meta-test.py run 5321:0,1,2,3,4
```

[![Docker Pulls](https://img.shields.io/docker/pulls/rshariffdeen/cpr.svg)](https://hub.docker.com/r/rshariffdeen/cpr) [![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4668317.svg)](https://doi.org/10.5281/zenodo.4668317)

# CPR - CardioPulmonary Resuscitation
CPR: A new automated program repair technique based on concolic execution
which works on patch abstraction with the sub-optimal goal of refining the patch to less over-fit 
the initial test cases. 

Automated program repair reduces the manual effort in fixing program errors. 
However, existing repair techniques modify a buggy program such that it passes given tests.
Such repair techniques do not discriminate between correct patches and patches that overfit
the available tests and break untested but desired functionality. We attempt to solve this
problem with a novel solution that make use of the duality of space exploration in Input 
Space and Program Space. We implemented our technique in the form of a tool called CPR and
evaluated its efficacy in reducing the patch space by discarding overfitting patches from 
a pool of plausible patches. Similar to Cardio-Pulmonary Resuscitation (CPR) does to a
patient, our tool CPR resuscitate or recover programs via appropriate fixes. 

In this work, we therefore propose and implement an integrated approach for detecting and discarding 
overfitting patches by exploiting the relationship between the patch space and input space.
We leverage concolic path exploration to systematically traverse the input space 
(and generate inputs), while systematically ruling out significant parts of the patch space.
Given a long enough time budget, this approach allows a significant reduction in the 
pool of patch candidates, as shown by our experiments. 

CPR is a reconfigurable APR tool for C source-codes. CPR is:

* Extensible: CPR is designed so that it can be easily extended to plug in any component to replace existing
* Efficient: CPR utilize parallel computing to improve performance




## Build and Dependencies
We provide a ready-made container which includes all necessary envrionment set-up
to deploy and run our tool. Dependencies include:

* LLVM 3.4
* KLEE 1.4
* Python 3.7
* Z3 SMT Solver
* MathSAT Solver
* Docker

Build and run a container:

    docker build -t cpr .
    docker run --rm -ti cpr /bin/bash


# Example
We provide several examples you can run to test our tool, all test cases are included
in the 'tests' directory. 

Run examples:

    pypy3 CPR.py --conf=tests/bug-types/div-zero/div-zero-1/repair.conf
    pypy3 CPR.py --conf=tests/bug-types/div-zero/div-zero-2/repair.conf


## Documentation ##

* [Getting Started](doc/GetStart.md)
* [Example Usage](doc/Examples.md)
* [Experiment Replication](experiments/README.md)  
* [Manual](doc/Manual.md)


## Bugs ##
CPR should be considered alpha-quality software. Bugs can be reported here:

    https://github.com/rshariffdeen/CPR/issues
    
# Contributions
We welcome contributions to improve this work, see [details](doc/Contributing.md)

## Developers
* Ridwan Shariffdeen
* Yannic Noller

## Contributors
* Sergey Mechtaev 

## Publication ##
**Concolic Program Repair** <br>
Ridwan Shariffdeen, Yannic Noller, Lars Grunske, Abhik Roychoudhury <br>
42nd ACM SIGPLAN Conference on Programming Language Design and Implementation (PLDI), 2021


## Acknowledgements ##
This work was partially supported by the National Satellite of Excellence in Trustworthy Software Systems, funded by National Research Foundation (NRF) Singapore. 


# License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details


