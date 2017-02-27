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
  rslt.attr( "class" ) = "S2Cap" ;
  return rslt;
}

//' Construct a S2Cap using axis and height
//'
//' Constructs a S2Cap from axis and height
//'
//' @param axis A numeric vector with three entries represtenting the direction axis.
//' @param height Single numeric representing the height of the cap.
//' @export S2Cap_FromAxisHeight
//[[Rcpp::export]]
List S2Cap_FromAxisHeight(NumericVector axis, double height){
  S2Point a = S2Point(axis[0], axis[1], axis[2]);
  return S2CapWrapForR(S2Cap::FromAxisHeight(a, height));
}

S2Cap R_S2CapFromList(List list){
  NumericVector axis = list["axis"];
  double height = list["height"];
  S2Point a = S2Point(axis[0], axis[1], axis[2]);
  return S2Cap::FromAxisHeight(a, height);
}

//[[Rcpp::export]]
LogicalVector S2Cap_contains_point(NumericMatrix points, List cap){
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
//' @export S2Cap_Area
//[[Rcpp::export]]
double S2Cap_Area(List cap){
  S2Cap c = R_S2CapFromList(cap);
  return c.area();
}