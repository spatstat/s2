#include <RcppCommon.h>
#include "s2/s2latlng.h"
#include "s2package_types.h"
#include <Rcpp.h>

#include <vector>
using std::vector;

//[[Rcpp::export]]
Rcpp::NumericVector latlng(SEXP p) {
  S2Point x = Rcpp::as<S2Point>(p);
  S2LatLng y(x);
  Rcpp::NumericVector rslt(2);
  rslt[0] = y.lat().degrees();
  rslt[1] = y.lng().degrees();
  return rslt;
}

//[[Rcpp::export]]
Rcpp::NumericVector ll2point(Rcpp::NumericVector p) {
  S2LatLng y = S2LatLng::FromDegrees(p[0], p[1]);
  Rcpp::NumericVector rslt(3);
  S2Point x = y.ToPoint();
  rslt[0] = x.x();
  rslt[1] = x.y();
  rslt[2] = x.z();
  return rslt;
}

RCPP_MODULE(S2Point_module){
  using namespace Rcpp;
  class_<S2Point>("S2Point")
  .constructor()
  .constructor<double,double,double>()
  .constructor<int,int,int>()
  .method("Fabs", &S2Point::Fabs)
  ;
}
