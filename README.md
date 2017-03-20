# s2: R bindings for Google's s2 library for geometry on the sphere

[![Build Status](https://travis-ci.org/spatstat/s2.svg?branch=master)](https://travis-ci.org/spatstat/s2)
[![Build status](https://ci.appveyor.com/api/projects/status/nxau3cf77m2ll5mv?svg=true)](https://ci.appveyor.com/project/rubak/s2)

The package (`master` branch) currently passes `R CMD check` without any errors and warnings on Linux, OSX and Windows.
Only a minor subset of the C++ library is wrapped at the moment.
Simple R data structures such as plain matrices and lists are used.
The API is not stable and changes should be expected.

The core C++ code is in `src/geometry`. This code is a slightly modified copy of the corresponding directory in [this repo](https://github.com/micolous/s2-geometry-library). (The subdirectories `python` and `test` are removed since they aren't needed by the R package.) The modifications to the original code are minor edits to satisfy the R package checker, and no new functionality is introduced at the C++ level.

The original source code was compiled into several shared objects with cmake, but we simply build everything into one shared object using the R standard file `Makevars.in` (`Makevars.win` on Windows) together with a configure script. This decision was based on advice given by Dirk Eddelbuettel to the [RcppCAF package](https://github.com/grantbrown/RcppCAF) at [stackoverflow](http://stackoverflow.com/q/31750582/3341769).
The configure script checks for the system requirement `openssl` and is based on the configure script by Jeroen Ooms in the [openssl R package](https://github.com/jeroenooms/openssl).
