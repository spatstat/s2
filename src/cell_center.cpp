#include <Rcpp.h>
#include "s2/s2latlng.h"
#include "s2/s2cellid.h"

//' @title
//' cell_center
//' @description
//' Get center of S2 cell containing a specified point
//' 
//' @param lat latitude of interest (between -90 and +90)
//' 
//' @param lng longitude of interest (between -180 and +180)
//' 
//' @details
//' \code{cell_center} takes a single (lat,lng) point on the sphere, finds the
//' S2 leaf cell that contains this point and returns the center of this cell.
//' In S2 leaf cells have a very fine resolution, so the output is expected to
//' be very close to the input, and differences are typically on something like
//' the 5th decimal place.
//' 
//' @examples
//' cell_center(57.0139595, 9.988967)
//' 
//' @export
//[[Rcpp::export]]
Rcpp::NumericVector cell_center(double lat, double lng) {
  S2LatLng p = S2LatLng::FromDegrees(lat, lng);
  S2CellId id = S2CellId::FromLatLng(p);
  S2LatLng center = id.ToLatLng();
  Rcpp::NumericVector rslt(2);
  rslt[0] = center.lat().degrees();
  rslt[1] = center.lng().degrees();
  return rslt;
}
