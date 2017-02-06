#include <RcppCommon.h>
#include "s2/s2polygon.h"
#include "s2/s2polygonbuilder.h"
#include "s2package_types.h"
#include <Rcpp.h>

RCPP_MODULE(S2PolygonBuilder_module){
  using namespace Rcpp;
  class_<S2LatLngRect>("S2LatLngRect")
  .constructor()
  .constructor<S2LatLng,S2LatLng>()
  ;
  
  class_<S2PolygonBuilder>("S2PolygonBuilder")
  .constructor<S2PolygonBuilderOptions>()
  .method("AddLoop", &S2PolygonBuilder::AddLoop)
  .method("AssemblePolygon", &S2PolygonBuilder::AssemblePolygon)
  .method("AddEdge", &S2PolygonBuilder::AddEdge)
  ;
  
  class_<S2PolygonBuilderOptions>("S2PolygonBuilderOptions")
  .constructor()
  ;
}
