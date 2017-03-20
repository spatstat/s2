#include "Rcpp_datatypes.h"
// Manual functions to interface C++ and R types for when Rcpp::as and Rcpp::wrap
// Don't work (the compiler can't automatically figure out what to do, and I
// can't figure out how to fix this right now. Not a big concern.)

// Get a vector of `S2Point`s from a three column matrix from R.
std::vector<S2Point> S2PointVecFromR(Rcpp::NumericMatrix mat){
  if(mat.ncol() != 3)
    Rcpp::stop("Can't interpret input as points on the sphere - must be a three column matrix.");
  Rcpp::NumericVector coord1 = mat( Rcpp::_, 0);
  Rcpp::NumericVector coord2 = mat( Rcpp::_, 1);
  Rcpp::NumericVector coord3 = mat( Rcpp::_, 2);
  const int n = coord1.size();
  std::vector<S2Point> rslt(n);
  for(int i = 0; i < n; i++){
    rslt[i] = S2Point(coord1[i], coord2[i], coord3[i]);
    // rslt[i] = S2Point();
  }
  return rslt;
} 

Rcpp::NumericMatrix S2PointVecToR(std::vector<S2Point> points){
  int n = points.size();
  Rcpp::NumericMatrix rslt(n,3);
  for(int i=0; i<n; i++){
    rslt(i,0) = points[i].x();
    rslt(i,1) = points[i].y();
    rslt(i,2) = points[i].z();
  }
  return rslt;
}