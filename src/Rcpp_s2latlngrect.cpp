#include "Rcpp_datatypes.h"
#include "s2/s2latlngrect.h"
#include <Rcpp.h>

using namespace Rcpp;

// Wrap an S2LatLngRect as a list in R:
List S2LatLngRectToR(const S2LatLngRect& rect){
  NumericVector lat(2);
  lat[0] = rect.lat_lo().degrees();
  lat[1] = rect.lat_hi().degrees();
  NumericVector lng(2);
  lng[0] = rect.lng_lo().degrees();
  lng[1] = rect.lng_hi().degrees();
  List rslt = List::create(Named("lat") = lat,
                           Named("lng") = lng);
  rslt.attr( "class" ) = "S2LatLngRect" ;
  return rslt;
}

// Get a S2LatLngRect from a list in R.
S2LatLngRect S2LatLngRectFromR(List list){
  NumericVector lat = list["lat"];
  NumericVector lng = list["lng"];
  S2LatLng lo = S2LatLng::FromDegrees(lat[0], lng[0]);
  S2LatLng hi = S2LatLng::FromDegrees(lat[1], lng[1]);
  S2LatLngRect rslt(lo, hi);
  if(!rslt.is_valid())
    stop("Invalid rectangle!");
  return rslt;
}

//' Create a rectangle of latitude and longitude on the sphere
//'
//' Create a rectangle of latitude and longitude on the sphere
//' 
//' @param lo Latitude and longitude (in that order) in degrees of lower left corner.
//' @param hi Latitude and longitude (in that order) in degrees of upper right corner.
//' @export S2LatLngRect
//[[Rcpp::export]]
List S2LatLngRect(NumericVector lo, NumericVector hi){
  NumericVector lat(2),lng(2);
  lat[0] = lo[0];
  lat[1] = hi[0];
  lng[0] = lo[1];
  lng[1] = hi[1];
  List list = List::create(Named("lat") = lat,
                           Named("lng") = lng);
  auto rect = S2LatLngRectFromR(list);
  return S2LatLngRectToR(rect);
}

//' Area of s2latlngrect
//'
//' Area of a S2LatLngRect.
//'
//' @param x Named list containing axis and height of cap.
//' @export S2LatLngRect_area
//[[Rcpp::export]]
double S2LatLngRect_area(List x){
  auto rect = S2LatLngRectFromR(x);
  return rect.Area();
}
