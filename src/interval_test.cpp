//// Code to test basic R interface ////

#include <Rcpp.h>
#include "s2/s1interval.h"

//[[Rcpp::export]]
bool interval_test(double x, double a, double b) {
  S1Interval my_int(a, b);
  bool inside = my_int.Contains(x);
  return inside;
}
