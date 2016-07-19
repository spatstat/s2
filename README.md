# s2
R bindings for Google's s2 library for geometry on the sphere

The core C++ code is in `src/geometry` and it is simply a copy of the corresponding directory in [this repo](https://github.com/micolous/s2-geometry-library). (The subdirectories `python` and `test` are removed since they aren't needed by the R package.) 

There are two branches at the moment:

- `master` (NOT WORKING YET!): Here we try to follow advice given by Dirk Eddelbuettel to the [RcppCAF package](https://github.com/grantbrown/RcppCAF) at [stackoverflow](http://stackoverflow.com/q/31750582/3341769). The goal is to simply build everything into one shared object using the R standard file `Makevars`.

- `cmake` (partially working): Use CMake to compile everything bypassing the standard R compilation procedure using an empty `src/Makefile` as exemplified [here](https://github.com/forexample/rcpp-test).

Using the `cmake` branch we have succeeded in building a working R package on Ubuntu, so it is indeed a viable solution, but it would probably be better to make it work via the standard R compilation procedure in the `master` branch.

Forthcoming tasks:

- Make the basic library compile into a working R package on Linux, OSX and Windows.
-
- Use [Rcpp](http://cran.r-project.org/package=Rcpp) to interface the library either via external pointers or RcppModule.
-
