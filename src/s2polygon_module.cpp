#include <RcppCommon.h>
#include "s2/s2polygon.h"
#include "s2package_types.h"
#include <Rcpp.h>

bool S2Polygon_IsValid(S2Polygon* poly){
  return poly->IsValid();
}

void S2PolygonInitFromRcppList(S2Polygon * poly, Rcpp::List loops){
  int n = loops.size();
  S2PolygonBuilderOptions pbo;
  S2PolygonBuilder pb(pbo);
  for(int i = 0; i < n; i++){
    std::vector<S2Point> points = Rcpp::as<std::vector<S2Point>>(loops[i]);
    S2Loop tmp;
    tmp.Init(points);
    pb.AddLoop(&tmp);
  }
  S2PolygonBuilder::EdgeList unused_edges;
  pb.AssemblePolygon(poly, &unused_edges);
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
  .method(".internalInit", &S2PolygonInitFromRcppList)
  ;
}
