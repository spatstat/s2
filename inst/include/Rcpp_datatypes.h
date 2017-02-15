#ifndef S2PACKAGE_TYPES_H_
#define S2PACKAGE_TYPES_H_

#include <RcppCommon.h>
#include <vector>
#include "s2/s2.h"

namespace Rcpp {
  template<> std::vector<S2Point> as( SEXP );
  template <> SEXP wrap(const std::vector<S2Point> &p);
  // Wrappers for S2Points aka Vector3<double> if not exposed as class by module
  // namespace traits{
  //   template <typename T> SEXP wrap(const Vector3<T> & obj);
  //   template <typename T> class Exporter< Vector3<T> >;
  // }
}

#endif  // S2PACKAGE_TYPES_H_