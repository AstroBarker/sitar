# Sitar
SIngle isoTope sAha solveR. Also a stringed instrument from the Indian subcontinent.

playing with making a Saha ionization solver

The figure below shows ionization fractions for unionized, singly, and doubly ionized Calcium.
Compare to Figure 2.6 of "Supernovae and Nucleosynthesis", David Arnett.
![First three ionization levels of Calcium](ca.png "Caclium ionization")

## Building
To build `Sitar`, simply
```sh
mkdir build && cd build
cmake ..
make -j
```
Then an executable `saha` will be created in `build/src`.

An observation: a Saha solver requires an iterative solver, such as Newton-Raphson.
Implementing Anderson accelerated Newton-Raphson provides a roughly 4x speedup for the calculations above.

# Testing
Compilation and regression tests are run on each PR.

# Code Style

We use `clang format` ((here)[https://clang.llvm.org/docs/ClangFormat.html]) 
and `ruff` ((here)[https://docs.astral.sh/ruff/linter/]) for code cleanliness. 
C++ rules are listed in `.clang-format`.
The current version of `clang-format` used is `clang-format-13`, probably.
Simply call `tools/bash/format.sh` to format the `.hpp` and `.cpp` files.

Python code linting and formatting is done with `ruff`. 
Rules are listed in `ruff.toml`. 
To check all python in the current directory, you may `ruff ..`
To format a given file according to `ruff.toml`, run `ruff format file.py`. 

Checks for formatting are performed on each PR.
