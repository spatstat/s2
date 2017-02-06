#include "s2package_types.h"
#include "s2/s1angle.h"
#include <Rcpp.h>

RCPP_MODULE(S1Angle_module){
  using namespace Rcpp;
  class_<S1Angle>("S1Angle")
  .constructor()
  .constructor<S2Point,S2Point>()
  .method("degrees", &S1Angle::degrees)
  ;
  
}
