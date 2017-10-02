#include "Rcpp_datatypes.h"
// Manual functions to interface C++ and R types for when Rcpp::as and Rcpp::wrap
// Don't work (the compiler can't automatically figure out what to do, and I
// can't figure out how to fix this right now. Not a big concern.)

using namespace Rcpp;

// Get a vector of `S2Point`s from a three column matrix from R.
std::vector<S2Point> S2PointVecFromR(NumericMatrix mat){
  if(mat.ncol() != 3)
    stop("Can't interpret input as points on the sphere - must be a three column matrix.");
  NumericVector coord1 = mat( _, 0);
  NumericVector coord2 = mat( _, 1);
  NumericVector coord3 = mat( _, 2);
  const int n = coord1.size();
  std::vector<S2Point> rslt(n);
  for(int i = 0; i < n; i++){
    rslt[i] = S2Point(coord1[i], coord2[i], coord3[i]);
    // rslt[i] = S2Point();
  }
  return rslt;
} 

NumericMatrix S2PointVecToR(std::vector<S2Point> points){
  int n = points.size();
  NumericMatrix rslt(n,3);
  for(int i=0; i<n; i++){
    rslt(i,0) = points[i].x();
    rslt(i,1) = points[i].y();
    rslt(i,2) = points[i].z();
  }
  return rslt;
}
