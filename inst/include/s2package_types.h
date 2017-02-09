#ifndef S2PACKAGE_TYPES_H_
#define S2PACKAGE_TYPES_H_

#include <RcppCommon.h>
#include <vector>
#include "s2/s2.h"
#include "s2/s2polygonbuilder.h"

RCPP_EXPOSED_CLASS(S1Angle);
RCPP_EXPOSED_CLASS(S2Cap);
RCPP_EXPOSED_CLASS(S2CellId);
RCPP_EXPOSED_CLASS(S2LatLng);
RCPP_EXPOSED_CLASS(S2Loop);
RCPP_EXPOSED_CLASS_NODECL(S2Point);
RCPP_EXPOSED_CLASS(S2Polygon);
RCPP_EXPOSED_CLASS(S2PolygonBuilder);
RCPP_EXPOSED_CLASS(S2PolygonBuilderOptions);
RCPP_EXPOSED_CLASS(S2LatLngRect);
RCPP_EXPOSED_CLASS(S2Region);

namespace Rcpp {
  template<> std::vector<S2Point> as( SEXP );
  template<> S2PolygonBuilder::EdgeList* as( SEXP ) ;
  template <> SEXP wrap(const S2PolygonBuilder::EdgeList &e);
  template <> SEXP wrap(const std::vector<S2Point> &p);
  // Wrappers for S2Points aka Vector3<double> if not exposed as class by module
  // namespace traits{
  //   template <typename T> SEXP wrap(const Vector3<T> & obj);
  //   template <typename T> class Exporter< Vector3<T> >;
  // }
}

#endif  // S2PACKAGE_TYPES_H_