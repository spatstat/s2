#include "Rcpp_datatypes.h"
#include "s2/s2latlng.h"
#include <Rcpp.h>

using namespace Rcpp;

// Get a vector of `S2LatLng`s from a two column matrix from R.
std::vector<S2LatLng> S2LatLngVecFromR(NumericMatrix mat){
  if(mat.ncol() != 2)
    stop("Can't interpret input as lat,lng - must be a two column matrix.");
  NumericVector lat = mat( _, 0);
  NumericVector lng = mat( _, 1);
  const int n = lat.size();
  std::vector<S2LatLng> rslt(n);
  for(int i = 0; i < n; i++){
    rslt[i] = S2LatLng::FromDegrees(lat[i], lng[i]);
  }
  return rslt;
} 

// Wrap a vector of `S2LatLng`s to a two column matrix in R.
NumericMatrix S2LatLngVecToR(std::vector<S2LatLng> points){
  int n = points.size();
  NumericMatrix rslt(n,2);
  for(int i=0; i<n; i++){
    rslt(i,0) = points[i].lat().degrees();
    rslt(i,1) = points[i].lng().degrees();
  }
  return rslt;
}
