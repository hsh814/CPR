# CPR Benchmark
### Environment setting
```shell
cd /root/projects/CPR/lib
make

cd /root/projects/uni-klee/build
make clean && make -j 32 install
```

## Simple Example

```shell
export PATH=/root/projects/CPR/scripts:$PATH
# 1. Compile patches
sympatch.py compile patches

# 2. Build
symutil.py build 3623 # Or run ./init.sh

# 3. Run filter
symradar.py filter 3623
# symradar.py analyze 3623 -p filter

# 4. Run symradar
symradar.py rerun 3623 --sym-level=high -s high
symradar.py analyze 3623 -s high

# 5. The output is in patches/extractfix/libtiff/CVE-2016-3623/patched/high-*/table_v3.sbsv
```


## Experiment Replication
Use `experiments.py` to run experiments for all subjects in the benchmark.
It just calls `python3 <script> <cmd> <subject>` in parallel.
```shell
export PATH=/root/projects/CPR/scripts:$PATH
# 1. Compile patches
sympatch.py compile patches

# 2. Build
experiments.py feas --extra build

# 3. Run filter
experiments.py filter
experiments.py analyze --extra analyze -s filter

# 4. Run SymRadar
experiments.py exp --extra high -s high
experiments.py analyze --extra analyze -s high

# 5. Collect results (check ./out directory)
experiments.py final -s high
```

## Script commands and options
In `scripts` directory, there are 3 main scripts and 1 `experiments.py` script for parallel execution.
Run `export PATH=/root/projects/CPR/scripts:$PATH` to use them.

### 1. `sympatch.py`
This script extract concrete patches from `CPR` generated patches and convert them into meta program. Extraction is already done, so you only need to run `compile`.
Build patch of all subjects.
```shell
# sympatch.py <cmd> <patch-dir>
sympatch.py compile patches
```
### 2. `symutil.py`
This script has utilities for build subjects.
```shell
symutil.py build <subject>
```

### 3. `symradar.py`
This is the main script for `SymRadar`. It runs `uni-klee` with proper options and analyze the results.
```shell
symradar.py <cmd> <subject> <options>
symradar.py filter <subject>
symradar.py rerun <subject> --sym-level=high -p high
symradar.py analyze <subject> -p high
```
In `symutil.py` and `symradar.py`, you can enter `subject` only part of their name.
For example, `3623` will be recognized as `CVE-2016-3623`.

cmds:
- filter: filter patches with concrete input.
- run/rerun: run experiments. `run` reuses existing snapshot and `rerun` deletes existing snapshot.
- analyze: analyze the experiment result.
- uc: Use `UC-KLEE` mode.

options:
- `-p`: Output directory prefix.
- `-s`: Snapshot directory prefix.
- `--sym-level`: Symbolize level. In our experiment, we used `high`.
- `-t`: Timeout given format as `12h`.
- `--mode`: You can select symradar or extractfix.
- `--vulmaster-id`: If given, use vulmaster mode. 

### 4. `experiments.py`
This is the script for running experiments on all subjects. By default, it runs all 28 subjects in parallel.
```shell
experiments.py <cmd> <options>
```
Main options:
- `--extra`: used for most commands. Required values are different by each command.
- `-s`: SymRadar output directory prefix.
- `--mode`: Select mode between `symradar` and `extractfix`.
- `--vulmaster`: Use vulmaster mode.

cmds:
* `util`: This command calls `symutil.py`.
Options for `--extra`:
    - build: Build target program
    - extractfix-build: Build target program for `ExtractFix`
    - vulmaster-build: Build target program for `VulMaster` patches
    - vulmaster-extractfix-build: Build target program for `VulMaster` and use `ExtractFix` mode.

* `filter`: This command calls `symradar.py`. You don't need an option `--extra`.

* `run`/`exp`: These commands call `symradar.py`. `run` reuses existing snapshot and `exp` deletes existing snapshot and rerun the experiments.
Options for `--extra`:
    - `high`: We used this option.
    - `none`: This one is used in `--mode=extractfix`.

* `analyze`: This command calls `symradar.py`. In most cases, analysis is done automatically in `symradar.py run/rerun`. But in case of timeout, analysis may not be done. So, you should run this command before collecting final results.
Options for `--extra`:
    - `analyze`: This is used.

* `final`: This command collects results generated from `analyze` into single file.

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
