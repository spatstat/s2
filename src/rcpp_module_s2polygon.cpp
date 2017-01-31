#include <RcppCommon.h>
#include "s2/s2loop.h"
#include "s2/s2polygon.h"
#include "s2/s2polygonbuilder.h"

#include <vector>
using std::vector;
#include <string>

namespace Rcpp {
template<> S2Point as( SEXP ) ;
template<> std::vector<S2Point> as( SEXP );
template<> S2PolygonBuilder::EdgeList* as( SEXP ) ;
template <> SEXP wrap(const S2Point &p);
}

#include <Rcpp.h>

namespace Rcpp {

template <> S2Point as(SEXP x) {
  Rcpp::XPtr<S2Point> ptr(x);
  return *ptr;
}

template<> std::vector<S2Point> as(SEXP matsexp) {
  Rcpp::NumericMatrix mat(matsexp);
  Rcpp::NumericVector coord1 = mat( _, 0);
  Rcpp::NumericVector coord2 = mat( _, 1);
  Rcpp::NumericVector coord3 = mat( _, 2);
  const int n = coord1.size();
  std::vector<S2Point> rslt(n);
  for(int i = 0; i < n; i++){
    rslt[i] = S2Point(coord1[i], coord2[i], coord3[i]);
  }
  return rslt;
}

template <> S2PolygonBuilder::EdgeList* as(SEXP x) {
  S2PolygonBuilder::EdgeList* rslt;
  return rslt;
}

template <> SEXP wrap(const S2Point &p){
  Rcpp::XPtr<S2Point> ptr( new S2Point(p), true );
  return ptr;
}
}

//[[Rcpp::export]]
Rcpp::NumericVector S2Point_coords(SEXP p) {
  Rcpp::XPtr<S2Point> ptr(p);
  Rcpp::NumericVector rslt(3);
  rslt[0] = ptr->x();
  rslt[1] = ptr->y();
  rslt[2] = ptr->z();
  return rslt;
}

RCPP_EXPOSED_CLASS(S2Loop);
RCPP_EXPOSED_CLASS(S2Polygon);
RCPP_EXPOSED_CLASS(S2PolygonBuilder);
RCPP_EXPOSED_CLASS(S2PolygonBuilderOptions);

RCPP_MODULE(S2Polygon_module){
  using namespace Rcpp;
  
  class_<S2Loop>("S2Loop")
  .constructor()
  .constructor<std::vector<S2Point>>()
  .method("Init", &S2Loop::Init)
  .method("GetArea", &S2Loop::GetArea)
  .method("Intersects", &S2Loop::Intersects)
  .method("BoundaryEquals", &S2Loop::BoundaryEquals)
  .method("vertex", &S2Loop::vertex)
  ;
  
  class_<S2Polygon>("S2Polygon")
  .constructor()
  .method("num_loops", &S2Polygon::num_loops)
  .method("num_vertices", &S2Polygon::num_vertices)
  .method("loop", &S2Polygon::loop)
  .method("GetCentroid", &S2Polygon::GetCentroid)
  .method("VirtualContainsPoint", &S2Polygon::VirtualContainsPoint)
  ;
  
  class_<S2PolygonBuilder>("S2PolygonBuilder")
  .constructor<S2PolygonBuilderOptions>()
  .method("AddLoop", &S2PolygonBuilder::AddLoop)
  .method("AssemblePolygon", &S2PolygonBuilder::AssemblePolygon)
  // .method("AddEdge", &S2PolygonBuilder::AddEdge)
  ;
  
  class_<S2PolygonBuilderOptions>("S2PolygonBuilderOptions")
  .constructor()
  ;
}

RCPP_MODULE(test){
  using namespace Rcpp;
}
