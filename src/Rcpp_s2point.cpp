#include "Rcpp_datatypes.h"
#include "s2/s2.h"
#include "s2/s2polyline.h"
#include <Rcpp.h>

using namespace Rcpp;
// Find break points to split polyline with vertices `points`  in n pieces of
// equal length (incl. start and end vertex). Only used with two points in our
// code.
std::vector<S2Point> s2point_interpolate_int(std::vector<S2Point> points, int n){
  S2Polyline line(points);
  std::vector<S2Point> output;
  for( int i=0; i<=n; i++ ){
    output.push_back(line.Interpolate(i*1.0/n));
  }
  return output;
}

// Consider `points` as vetices in a polyline and add interpolating vertices if
// necessary so no segment is more than `eps` long.
std::vector<S2Point> s2point_interpolate(std::vector<S2Point> points, double eps){
  int npoints = points.size();
  if(npoints<=1) return points;
  std::vector<S2Point> output;
  output.push_back(points[0]);
  auto it = points.begin();
  for( int i=0; i<npoints-1; i++ ){
    std::vector<S2Point> endpoints(it+i, it+i+2);
    double dist = endpoints[0].Angle(endpoints[1]);
    int n = ceilf(dist/eps);
    auto tmp = s2point_interpolate_int(endpoints, n);
    output.insert(output.end(), tmp.begin()+1, tmp.end());
  }
  return output;
}

//' Interpolation of points on unit sphere
//' 
//' Given a sequence of points on the unit sphere add interpolating points so
//' consecutive points are within distance `eps` of each other.
//' @param x Matrix with three columns representing the points.
//' @param eps Strictly positive real number. Values greater than or equal to pi
//' correspond to no interpolation.
//' @export S2Point_interpolate
//[[Rcpp::export]]
NumericMatrix S2Point_interpolate(NumericMatrix x, double eps){
  std::vector<S2Point> points  = S2PointVecFromR(x);
  return S2PointVecToR(s2point_interpolate(points, eps));
}
