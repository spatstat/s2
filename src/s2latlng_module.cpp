#include <RcppCommon.h>
#include "s2/s2latlng.h"
#include <vector>
using std::vector;

#include "s2package_types.h"
#include <Rcpp.h>
S2LatLng S2LatLngFromDegrees(S2LatLng* x, double lat_degrees, double lng_degrees) {
  return S2LatLng::FromDegrees(lat_degrees, lng_degrees);
}

RCPP_MODULE(S2LatLng_module){
  using namespace Rcpp;
  
  class_<S2LatLng>("S2LatLng")
  .constructor()
  .constructor<S1Angle,S1Angle>()
  .method("FromDegrees", &S2LatLngFromDegrees)
  .method("ToPoint", &S2LatLng::ToPoint)
  ;
  
  function("S2LatLngFromDegrees", &S2LatLng::FromDegrees);
}
