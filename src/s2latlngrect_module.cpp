#include <RcppCommon.h>
#include "s2/s2latlngrect.h"
#include "s2package_types.h"
#include <Rcpp.h>

RCPP_MODULE(S2LatLngRect_module){
  using namespace Rcpp;
  class_<S2LatLngRect>("S2LatLngRect")
  .constructor()
  .constructor<S2LatLng,S2LatLng>()
  ;
}
