#include "Rcpp_datatypes.h"
#include <Rcpp.h>
#include <s2/s2cap.h>
#include <s2/s2latlngrect.h>

using namespace Rcpp;

// Wrap an S2Cap as a list in R with:
// [[1]] The axis as a 3D unit vector
// [[2]] The height as a numeric
List S2CapToR(const S2Cap& cap){
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

S2Cap S2CapFromR(List list){
  NumericVector axis = list["axis"];
  double height = list["height"];
  S2Point a = S2Point(axis[0], axis[1], axis[2]);
  return S2Cap::FromAxisHeight(a, height);
}

//' Construct a S2Cap using axis and height
//'
//' Constructs a S2Cap from axis and height
//'
//' @param axis A numeric vector with three entries represtenting the direction axis.
//' @param height Single numeric representing the height of the cap.
//' @export S2CapFromAxisHeight
//[[Rcpp::export]]
List S2CapFromAxisHeight(NumericVector axis, double height){
  S2Point a = S2Point(axis[0], axis[1], axis[2]);
  return S2CapToR(S2Cap::FromAxisHeight(a, height));
}

//[[Rcpp::export]]
LogicalVector S2Cap_contains_point(NumericMatrix points, List cap){
  S2Cap c = S2CapFromR(cap);
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
//' @export S2Cap_area
//[[Rcpp::export]]
double S2Cap_area(List cap){
  S2Cap c = S2CapFromR(cap);
  return c.area();
}

//' Bounding latitude and longitude rectangle for spherical cap
//'
//' Bounding latitude and longitude rectangle for spherical cap
//' 
//' @param x cap
//' @export
//[[Rcpp::export]]
List S2Cap_GetRectBound(List x){
  S2Cap cap = S2CapFromR(x);
  S2LatLngRect rect = cap.GetRectBound();
  return S2LatLngRectToR(rect);
}
