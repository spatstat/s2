#include <RcppCommon.h>
#include "s2/s2cellid.h"
#include "s2/s2latlng.h"
#include "s2package_types.h"
#include <Rcpp.h>

// S2CellId S2CellIdFromLatLng(S2LatLng const &ll){
//   return S2CellId::FromLatLng(ll);
// }

RCPP_MODULE(S2CellId_module){
  using namespace Rcpp;
  
  class_<S2CellId>("S2CellId")
  .constructor()
  .method("ToPoint", &S2CellId::ToPoint)
  .method("ToString", &S2CellId::ToString)
  .method("ToToken", &S2CellId::ToToken)
  ;
  
  function("S2CellIdFromLatLng", &S2CellId::FromLatLng, "Docstring");
}
