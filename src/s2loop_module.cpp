#include <RcppCommon.h>
#include "s2/s2loop.h"

#include <vector>
using std::vector;

#include "s2package_types.h"
#include <Rcpp.h>

Rcpp::DataFrame dumploop_numeric(S2Loop* x){
  int n = x->num_vertices();
  Rcpp::NumericVector lat(n),lng(n);
  S2LatLng tmp;
  for(int i = 0; i < n; i++){
    tmp = S2LatLng(x->vertex(i));
    lat[i] = tmp.lat().degrees();
    lng[i] = tmp.lng().degrees();
  }
  return Rcpp::DataFrame::create(Rcpp::Named("lng")=lng, Rcpp::Named("lat")=lat);
}

bool S2Loop_IsValid(S2Loop* loop){
  return loop->IsValid();
}

RCPP_MODULE(S2Loop_module){
  using namespace Rcpp;
  class_<S2Loop>("S2Loop")
  .constructor()
  .constructor<std::vector<S2Point>>()
  .method("Init", &S2Loop::Init)
  .method("GetArea", &S2Loop::GetArea)
  .method("Intersects", &S2Loop::Intersects)
  .method("IsValid", &S2Loop_IsValid)
  .method("BoundaryEquals", &S2Loop::BoundaryEquals)
  .method("vertex", &S2Loop::vertex)
  .method("sign", &S2Loop::sign)
  .method("dump", &dumploop_numeric)
  ;
}
