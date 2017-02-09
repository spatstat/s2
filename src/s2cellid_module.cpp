#include <RcppCommon.h>
#include "s2/s2cellid.h"
#include "s2/s2latlng.h"
#include "s2package_types.h"
#include <Rcpp.h>

// S2CellId S2CellIdFromLatLng(S2LatLng const &ll){
//   return S2CellId::FromLatLng(ll);
// }
// 
// //[[Rcpp::export]]
// std::string cellparent(double lat, double lng, int level){
//   S2LatLng x = S2LatLng::FromDegrees(lat, lng);
//   S2CellId id = S2CellId::FromLatLng(x);
//   return id.parent(level).ToString();
// }

S2CellId S2CellId_parent(S2CellId* c, int level){
  if(c->level() < level) Rcpp::stop("Supplied level larger than current level!");
  return c->parent(level);
}

RCPP_MODULE(S2CellId_module){
  using namespace Rcpp;
  
  class_<S2CellId>("S2CellId")
  .constructor()
  .method("face", &S2CellId::face)
  .method("level", &S2CellId::level)
  .method("parent", &S2CellId_parent)
  .method("ToPoint", &S2CellId::ToPoint)
  .method("ToString", &S2CellId::ToString)
  .method("ToToken", &S2CellId::ToToken)
  ;
  
  function("S2CellIdFromLatLng", &S2CellId::FromLatLng, "Docstring");
}
