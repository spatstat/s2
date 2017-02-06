#include <RcppCommon.h>
#include "s2/s2polygon.h"
#include "s2package_types.h"
#include <Rcpp.h>

bool S2Polygon_IsValid(S2Polygon* poly){
  return poly->IsValid();
}

RCPP_MODULE(S2Polygon_module){
  using namespace Rcpp;
  class_<S2Polygon>("S2Polygon")
  .constructor()
  .method("num_loops", &S2Polygon::num_loops)
  .method("num_vertices", &S2Polygon::num_vertices)
  .method("loop", &S2Polygon::loop)
  .method("IsValid", &S2Polygon_IsValid)
  .method("GetArea", &S2Polygon::GetArea)
  .method("IsNormalized", &S2Polygon::IsNormalized)
  .method("Project", &S2Polygon::Project)
  .method("GetCentroid", &S2Polygon::GetCentroid)
  .method("VirtualContainsPoint", &S2Polygon::VirtualContainsPoint)
  .method("InitToIntersection", &S2Polygon::InitToIntersection)
  .method("InitToUnion", &S2Polygon::InitToUnion)
  ;
}
