#include <Rcpp.h>
#include "s2/s2.h"

//' @export S2kMinWidthGetMaxLevel
//[[Rcpp::export]]
int S2kMinWidthGetMaxLevel(double x){
  return S2::kMinWidth.GetMaxLevel(x);
}
