# s2
R bindings for Google's s2 library for geometry on the sphere

The package (`master` branch) currently passes `R CMD check` without any errors and warnings on Linux, OSX and Windows.
It is simply an empty shell with only one useless R function to check that the interface to the underlying C++ code works, so there is really no point in installing the package at the moment unless you want to play with the C++ code and write some R bindings yourself.

The core C++ code is in `src/geometry`. This code is a slightly modified copy of the corresponding directory in [this repo](https://github.com/micolous/s2-geometry-library). (The subdirectories `python` and `test` are removed since they aren't needed by the R package.) The modifications to the original code are minor edits to satisfy the R package checker, and no new functionality is introduced at the momement.

There are two branches at the moment:

- `master` (this is the branch you want): Here we have followed advice given by Dirk Eddelbuettel to the [RcppCAF package](https://github.com/grantbrown/RcppCAF) at [stackoverflow](http://stackoverflow.com/q/31750582/3341769). The original code compiles into several separate shared objects, but we simply build everything into one shared object using the R standard file `Makevars.in` (`Makevars.win` on Windows) together with a configure script. The configure script checks for the system requirement `openssl` and is based on the configure script by Jeroen Ooms in the [openssl R package](https://github.com/jeroenooms/openssl).

- `cmake` (partially working -- now obsolete): Use CMake to compile everything bypassing the standard R compilation procedure using an empty `src/Makefile` as exemplified [here](https://github.com/forexample/rcpp-test).
