#ifndef S2PACKAGE_TYPES_H_
#define S2PACKAGE_TYPES_H_

#include "s2/s2.h"

#include <Rcpp.h>

// Manual functions to interface C++ and R types for when Rcpp::as and Rcpp::wrap
// Don't work (the compiler can't automatically figure out what to do, and I
// can't figure out how to fix this right now. Not a big concern.)

// Get a vector of `S2Point`s from a three column matrix from R.
std::vector<S2Point> S2PointVecFromR(Rcpp::NumericMatrix mat);
// Export a vector of `S2Point`s to a three column matrix in R.
Rcpp::NumericMatrix S2PointVecToR(std::vector<S2Point> points);

#endif  // S2PACKAGE_TYPES_H_