#ifndef S2PACKAGE_TYPES_H_
#define S2PACKAGE_TYPES_H_

#include "s2/s2.h"
#include "s2/s2cap.h"

#include <Rcpp.h>

// Manual functions to interface C++ and R types for when Rcpp::as and Rcpp::wrap
// Don't work (the compiler can't automatically figure out what to do, and I
// can't figure out how to fix this right now. Not a big concern.)

// Get a vector of `S2Point`s from a three column matrix from R.
std::vector<S2Point> S2PointVecFromR(Rcpp::NumericMatrix mat);
// Export a vector of `S2Point`s to a three column matrix in R.
Rcpp::NumericMatrix S2PointVecToR(std::vector<S2Point> points);

// Get a S2Cap from a list in R.
S2Cap S2CapFromR(Rcpp::List list);
// Export S2Cap to list in R.
Rcpp::List S2CapToR(const S2Cap& cap);

// Get a vector of `S2LatLng`s from a two column matrix from R.
std::vector<S2LatLng> S2LatLngVecFromR(Rcpp::NumericMatrix mat);
// Wrap a vector of `S2LatLng`s to a two column matrix in R.
Rcpp::NumericMatrix S2LatLngVecToR(std::vector<S2LatLng> points);

// Get a S2LatLngRect from a list in R.
S2LatLngRect S2LatLngRectFromR(Rcpp::List list);
// Export S2LatLngRect to list in R.
Rcpp::List S2LatLngRectToR(const S2LatLngRect& cap);

#endif  // S2PACKAGE_TYPES_H_