#ifndef S2PACKAGE_TYPES_H_
#define S2PACKAGE_TYPES_H_

#include <RcppCommon.h>
#include <vector>
#include "s2/s2.h"

namespace Rcpp {
  template<> std::vector<S2Point> as( SEXP );
  template <> SEXP wrap(const std::vector<S2Point> &p);
  // Wrappers for S2Points aka Vector3<double> if not exposed as class by module
  namespace traits{
    template <typename T> SEXP wrap(const Vector3<T> & obj);
    template <typename T> class Exporter< Vector3<T> >;
  }
}

// Manual functions to interface C++ and R types for when Rcpp::as and Rcpp::wrap
// Don't work (the compiler can't automatically figure out what to do, and I
// can't figure out how to fix this right now. Not a big concern.)

// Get a vector of `S2Point`s from a three column matrix from R.
std::vector<S2Point> S2PointVecFromR(Rcpp::NumericMatrix mat);

#endif  // S2PACKAGE_TYPES_H_