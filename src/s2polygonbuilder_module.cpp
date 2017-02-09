#include <RcppCommon.h>
#include "s2/s2polygon.h"
#include "s2/s2polygonbuilder.h"
#include "s2package_types.h"
#include <Rcpp.h>

//[[Rcpp::export]]
SEXP makeEdgeList(){
  const S2PolygonBuilder::EdgeList e;
  return Rcpp::wrap(e);
}

//[[Rcpp::export]]
int sizeEdgeList(SEXP e){
  S2PolygonBuilder::EdgeList* ee = Rcpp::as<S2PolygonBuilder::EdgeList*>(e);
  return ee->size();
}

//[[Rcpp::export]]
void fillEdgeList(SEXP e){
  S2PolygonBuilder::EdgeList* ee = Rcpp::as<S2PolygonBuilder::EdgeList*>(e);
  S2Point v1, v2;
  ee->push_back(std::make_pair(v1, v2));
}

RCPP_MODULE(S2PolygonBuilder_module){
  using namespace Rcpp;
  
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
