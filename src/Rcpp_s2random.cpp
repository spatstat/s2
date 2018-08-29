#include "Rcpp_datatypes.h"
#include <Rcpp.h>
#include <s2/s2.h>
#include <s2/s2cellid.h>
#include <s2/s2cell.h>
#include <s2/s2latlngrect.h>

S2Point SamplePoint(const S2LatLngRect& rect) {
  double lng = rect.lng().lo() + Rcpp::runif(1)[0] * rect.lng().GetLength();
  double lat = asin(Rcpp::runif(1, sin(rect.lat().lo()), sin(rect.lat().hi()))[0]);
  return S2LatLng::FromRadians(lat, lng).Normalized().ToPoint();
}

Rcpp::List S2Cell_random_points_from_id(std::vector<S2CellId> cellids,
                                        Rcpp::IntegerVector n_points,
                                        int giveup){
  int n = cellids.size();
  Rcpp::List rslt(n);
  for(int i=0; i<n; i++){
    S2Cell cell(cellids[i]);
    const S2LatLngRect rect = cell.GetRectBound();
    int j = 0;
    Rcpp::NumericMatrix points_i(n_points[i], 3);
    int n_tries = 0;
    while(j<n_points[i] && n_tries < giveup){
      const auto p = SamplePoint(rect);
      if(cell.Contains(p)){
        points_i(j, 0) = p.x();
        points_i(j, 1) = p.y();
        points_i(j, 2) = p.z();
        j++;
      } else{
        n_tries++;
      }
    }
    if(j<n_points[i]){
      Rcpp::warning(
        "Gave up simulation due to too many tries. Increase argument giveup to try harder."
        );
      while(j<n_points[i]){
        points_i(j, 0) = NA_REAL;
        points_i(j, 1) = NA_REAL;
        points_i(j, 2) = NA_REAL;
        j++;
      }
    }
    rslt[i] = points_i;
  }
  return rslt;
}

// Declare function from Rcpp_s2cell.cpp
std::vector<S2CellId> R_S2CellIdFromTokens(std::vector<std::string> tokens);

//[[Rcpp::export]]
Rcpp::List S2Cell_random_points_from_token(std::vector<std::string> tokens,
                                           Rcpp::IntegerVector n_points,
                                           int giveup){
  auto cellids = R_S2CellIdFromTokens(tokens);
  return S2Cell_random_points_from_id(cellids, n_points, giveup);
}
