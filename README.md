# CPR Benchmark
### Environment Setup
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

# 2. Build the target program
symutil.py build 3623 # Or run ./init.sh

# 3. Run the filter step
symradar.py filter 3623
# (Optional) Analyze filter results: symradar.py analyze 3623 -p filter

# 4. Run SymRadar analysis
symradar.py rerun 3623 --sym-level=high -s high
symradar.py analyze 3623 -s high

# 5. The output table can be found in:
#    patches/extractfix/libtiff/CVE-2016-3623/patched/high-*/table_v3.sbsv
```


## Experiment Replication
Use `experiments.py` to run experiments for all subjects in the benchmark in parallel.
This script calls `python3 <script> <cmd> <subject>` for each subject.
```shell
export PATH=/root/projects/CPR/scripts:$PATH

# 1. Compile patches
sympatch.py compile patches

# 2. Build subjects
experiments.py util --extra build

# 3. Run the filter step for all subjects
experiments.py filter

# 4. Run SymRadar for all subjects
experiments.py exp --extra high -s high
experiments.py analyze -s high

# 5. Collect final results (check the ./out directory)
experiments.py final -s high
```

## Script commands and options
The scripts directory contains three main scripts (sympatch.py, symutil.py, symradar.py) and the experiments.py script for parallel execution across multiple subjects.
Ensure the scripts directory is in your PATH: `export PATH=/root/projects/CPR/scripts:$PATH`.

### 1. `sympatch.py`
This script extracts concrete patches from CPR-generated patch files and converts them into a meta-program format. The extraction process is already completed; you only need to run the compile command to process the patches for all subjects found in the specified directory.
```shell
# Usage: sympatch.py <cmd> <patch-dir>
# Example: Compile patches located in the 'patches' directory
sympatch.py compile patches
```
### 2. `symutil.py`
This script provides utility functions for building subjects.
```shell
# Usage: symutil.py <cmd> <subject>
# Example: Build a specific subject
symutil.py build <subject>
```

### 3. `symradar.py`
This is the main script for running the `SymRadar` analysis. It invokes `uni-klee` with the appropriate options and analyzes the results.
```shell
# Usage: symradar.py <cmd> <subject> [options]
symradar.py filter <subject>
symradar.py rerun <subject> --sym-level=high -p high
symradar.py uc <subject> -p uc
symradar.py analyze <subject> -p high
```
Note: For `symutil.py` and `symradar.py`, you can specify a subject using only a unique part of its name (e.g., 3623 will be recognized as CVE-2016-3623).

#### `symradar.py` Commands (`<cmd>`):
- filter: Filters patches using concrete inputs.
- run/rerun: Executes the main SymRadar analysis. run reuses an existing KLEE state snapshot if available, while rerun always starts fresh (deleting any existing snapshot).
- analyze: Analyzes the results from a run or rerun execution.
- uc: Enables UC-KLEE mode (details assumed specific to the project).

#### `symradar.py` Options (`[options]`):
- `-p <prefix>`: Prefix for the output directory (used by analyze to find results, potentially by run/rerun implicitly).
- `--snapshot-prefix <prefix>`: Prefix for the snapshot directory (used by run/rerun).
- `--sym-level <level>`: Sets the symbolization level (e.g., none, high). We used high in our experiments.
- `-t <timeout>`: Sets the timeout duration (e.g., 12h for 12 hours).
- `--mode <mode>`: Selects the operational mode (symradar or extractfix).
- `--vulmaster-id <id>`: Enables VulMaster mode using the specified patch ID.

### 4. `experiments.py`
This script automates running commands (like build, filter, analyze) across all subjects (28 by default) in parallel.
```shell
# Usage: experiments.py <cmd> [options]
experiments.py util --extra build
experiments.py filter
experiments.py exp --extra high -s high
experiments.py analyze -s high
experiments.py final -s high
```
#### `experiments.py` Main Options:
- `--extra <value>`: Provides necessary arguments to the underlying script command being called (e.g., build type, sym-level). The required value depends on the specific <cmd>.
- `-s <prefix>`: Specifies the directory prefix used for SymRadar outputs and snapshots when calling symradar.py.
- `--mode <mode>`: Selects the mode (symradar or extractfix) for relevant commands.
- `--vulmaster`: Enables VulMaster mode for relevant commands.

#### `experiments.py` Commands (`<cmd>`)
* `util`: Calls `symutil.py`.
  - Values for `--extra`:
    - `build`: Build target program.
    - `extractfix-build`: Build target program for `ExtractFix`.
    - `vulmaster-build`: Build target program for `VulMaster` patches.
    - `vulmaster-extractfix-build`: Build target program for `VulMaster` and use `ExtractFix` mode.

* `filter`: Calls `symradar.py`. The `--extra` option is not needed.

* `run`/`exp`: Calls `symradar.py run` or `symradar.py rerun`. `run` reuses existing snapshot and `exp` deletes existing snapshot before running.
  - Values for `--extra`:
    - `high`: We used this option, which corresponds to `--sym-level=high`.
    - `none`: This one is used in `--mode=extractfix`. (`--sym-level=none`)

* `analyze`: Calls `symradar.py`. Analysis usually runs automatically after `run`/`exp`, but this command ensures it's done, especially if a run timed out. Recommended before running `final`.

* `final`: Collects results generated by analyze from the individual subject directories (identified by the `-s <prefix>`) and aggregates them into a single summary file (in the ./out directory).




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
