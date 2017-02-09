#include <RcppCommon.h>
#include "s2/s2region.h"
#include "s2package_types.h"
#include <Rcpp.h>

RCPP_MODULE(S2Region_module){
  using namespace Rcpp;
  class_<S2Region>("S2Region")
  ;
}
