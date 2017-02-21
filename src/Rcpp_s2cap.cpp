#include "Rcpp_datatypes.h"
#include <Rcpp.h>
#include <s2/s2cap.h>

using namespace Rcpp;

// Wrap an S2Cap as a list in R with:
// [[1]] The axis as a 3D unit vector
// [[2]] The height as a numeric
List S2CapWrapForR(const S2Cap& cap){
  NumericVector axis(3);
  S2Point a = cap.axis();
  axis[0] = a.x();
  axis[1] = a.y();
  axis[2] = a.z();
  double height = cap.height();
  auto rslt = List::create(Named("axis") = axis, Named("height") = height);
  rslt.attr( "class" ) = "s2cap" ;
  return rslt;
}

S2Cap R_S2CapFromAxisHeight(NumericVector axis, double height){
  S2Point a = S2Point(axis[0], axis[1], axis[2]);
  return S2Cap::FromAxisHeight(a, height);
}

S2Cap R_S2CapFromList(List list){
  NumericVector axis = list["axis"];
  double height = list["height"];
  S2Point a = S2Point(axis[0], axis[1], axis[2]);
  return S2Cap::FromAxisHeight(a, height);
}

//' Construct a S2Cap using axis and height from R
//' 
//' Constructs a S2Cap
//' 
//' @param axis A numeric vector with three entries represtenting the direction axis.
//' @param height Single numeric representing the height of the cap.
//' @export s2cap
//[[Rcpp::export]]
List s2cap(NumericVector axis, double height){
  S2Cap cap = R_S2CapFromAxisHeight(axis, height);
  return S2CapWrapForR(cap);
}

//' Point in s2cap test
//'
//' Test whether points on the sphere are contained in a cap
//'  
//' @param points A three column matrix represtenting the points.
//' @param cap Named list containing axis and height of cap.
//' @export s2cap_contains_point
//[[Rcpp::export]]
LogicalVector s2cap_contains_point(NumericMatrix points, List cap){
  S2Cap c = R_S2CapFromList(cap);
  auto s2points = S2PointVecFromR(points);
  int n = s2points.size();
  LogicalVector rslt(n);
  for(int i = 0; i < n; i++){
    rslt(i) = c.Contains(s2points[i]);
  }
  return rslt;
}

//' Area of s2cap
//'
//' Area of a cap
//'  
//' @param cap Named list containing axis and height of cap.
//' @export s2cap_area
//[[Rcpp::export]]
double s2cap_area(List cap){
  S2Cap c = R_S2CapFromList(cap);
  return c.area();
}