#include <RcppCommon.h>
#include "s2/s2cap.h"
#include "s2package_types.h"
#include <Rcpp.h>

S2Cap S2Cap_FromAxisHeight(S2Cap* x, S2Point const& axis, double height) {
  return S2Cap::FromAxisHeight(axis, height);
}

S2Cap S2Cap_FromAxisAngle(S2Cap* x, S2Point const& axis, S1Angle const& angle) {
  return S2Cap::FromAxisAngle(axis, angle);
}

S2Cap S2Cap_FromAxisArea(S2Cap* x, S2Point const& axis, double area) {
  return S2Cap::FromAxisArea(axis, area);
}

RCPP_MODULE(S2Cap_module){
  using namespace Rcpp;
  class_<S2Cap>("S2Cap")
  .constructor()
  .method("axis", &S2Cap::axis)
  .method("area", &S2Cap::area)
  .method("angle", &S2Cap::angle)
  .method("height", &S2Cap::height)
  .method("FromAxisHeight", &S2Cap_FromAxisHeight)
  .method("FromAxisAngle", &S2Cap_FromAxisAngle)
  .method("FromAxisArea", &S2Cap_FromAxisArea)
  ;
}
