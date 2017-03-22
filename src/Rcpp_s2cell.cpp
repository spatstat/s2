#include "Rcpp_datatypes.h"
#include <Rcpp.h>
#include <s2/s2.h>
#include <s2/s2cap.h>
#include <s2/s2cellid.h>
#include <s2/s2cell.h>
#include <s2/s2polygon.h>
#include <s2/s2regioncoverer.h>

using namespace Rcpp;

// Declare function from Rcpp_s2polygon.cpp
void S2PolygonInitFromR(List, S2Polygon&);

std::vector<S2CellId> S2CellId_FromS2Point(std::vector<S2Point> x, IntegerVector level){
  int n = x.size();
  int nlev = level.size();
  int lev = level[0];
  std::vector<S2CellId> rslt(n);
  if(nlev == 1 & lev == 30){
    // Default case of single level == 30
    for(int i=0; i<n; i++){
      rslt[i] = S2CellId::FromPoint(x[i]);
    }
  } else{
    // Generic case
    for(int i=0; i<n; i++){
      if(nlev > 1){
        lev = level[i];
      }
      rslt[i] = S2CellId::FromPoint(x[i]);
      if(lev!=30){
        rslt[i] = rslt[i].parent(lev);
      }
    }
  }
  return rslt;
}

List S2CellIdWrapForR(std::vector<S2CellId> ids, IntegerVector levels){
  int n = ids.size();
  CharacterVector tokens(n);
  for(int i=0; i<n; i++){
    tokens[i] = ids[i].ToToken();
  }
  List rslt = List::create(Named("id") = tokens,
                           Named("level") = levels);
  rslt.attr("class") = "S2CellId";
  return rslt;
}

CharacterVector S2CellIdStringWrapForR(std::vector<S2CellId> ids){
  int n = ids.size();
  CharacterVector strings(n);
  for(int i=0; i<n; i++){
    strings[i] = ids[i].ToString();
  }
  return strings;
}

std::vector<S2CellId> R_S2CellIdFromTokens(std::vector<std::string> tokens){
  int n = tokens.size();
  std::vector<S2CellId> ids(n);
  for(int i=0; i<n; i++){
    ids[i] = S2CellId::FromToken(tokens[i]);
  }
  return ids;
}

//' Make a Vector of S2CellIds From Points on the Sphere
//'
//' Create a vector of S2CellIds corresponding to the cells at the given level 
//' containing the given points. The default level (30) corresponds to leaf
//' cells (finest level).
//'
//' @param x Three-column matrix reprensenting the points.
//' @param level Integer between 0 and 30 (incl).
//' @return An object of class `S2CellId`.
//' @export S2CellIdFromPoint
//[[Rcpp::export]]
List S2CellIdFromPoint(NumericMatrix x, IntegerVector level = 30){
  auto points = S2PointVecFromR(x);
  auto ids = S2CellId_FromS2Point(points, level);
  return S2CellIdWrapForR(ids, level);
}

//' Convert S2CellId to a S2Point
//'
//' Convert S2CellId to a S2Point
//'
//' @param x Object of class S2CellId.
//' @return Three-column matrix reprensenting the points..
//' @export S2CellId_ToPoint
//[[Rcpp::export]]
NumericMatrix S2CellId_ToPoint(List x){
  std::vector<std::string> tokens = x["id"];
  auto ids = R_S2CellIdFromTokens(tokens);
  int n = tokens.size();
  std::vector<S2Point> output(n);
  for(int i=0; i<n; i++){
    output[i] = ids[i].ToPoint();
  }
  return S2PointVecToR(output);
}

//' Make a vector of S2CellId strings
//'
//' Make a vector of S2CellId strings
//'
//' @param x A charecter vector with S2CellIds (in token form).
//' @return A character vector with S2CellId strings.
//' @export S2CellId_ToString
//[[Rcpp::export]]
CharacterVector S2CellId_ToString(std::vector<std::string> x){
  auto ids = R_S2CellIdFromTokens(x);
  return S2CellIdStringWrapForR(ids);
}

std::vector<S2CellId> R_S2CellIdFromPoints(NumericMatrix mat, int level){
  auto points = S2PointVecFromR(mat);
  // int n = points.size();
  // std::vector<S2CellId> ids(n);
  // for(int i=0; i<n; i++){
  //   S2CellId tmp = S2CellId::FromPoint(points[i]);
  //   ids[i] = tmp.parent(level);
  // }
  // return ids;
  return S2CellId_FromS2Point(points, level);
}

List S2Cell_vertices_from_id(std::vector<S2CellId> cellids){
  int n = cellids.size();
  List rslt(n);
  for(int i=0; i<n; i++){
    S2Cell cell(cellids[i]);
    // std::vector<S2Point> ver(4);
    NumericMatrix vertices_i(4, 3);
    for(int j=0; j<4; j++){
      auto v = cell.GetVertex(j);
      vertices_i(j, 0) = v.x();
      vertices_i(j, 1) = v.y();
      vertices_i(j, 2) = v.z();
    }
    rslt[i] = vertices_i;
  }
  return rslt;
}

//[[Rcpp::export]]
List S2Cell_vertices_from_token(std::vector<std::string> tokens){
  auto cellids = R_S2CellIdFromTokens(tokens);
  return S2Cell_vertices_from_id(cellids);
}

//[[Rcpp::export]]
List S2Cell_vertices_from_point(NumericMatrix mat, int level){
  auto cellids = R_S2CellIdFromPoints(mat, level);
  return S2Cell_vertices_from_id(cellids);
}

//[[Rcpp::export]]
NumericMatrix S2Cell_grid_centers(int level){
  if(level<0 | level>9){
    stop("The level must be non-negative and not more than 9 for now...");
  }
  long n = 6*pow(4,level);
  NumericMatrix rslt(n,3);
  long i=0;
  for(S2CellId cellid = S2CellId::Begin(level); cellid != S2CellId::End(level); cellid = cellid.next()){
    auto tmp = cellid.ToPoint();
    rslt(i,0) = tmp.x();
    rslt(i,1) = tmp.y();
    rslt(i,2) = tmp.z();
    i++;
  }
  return rslt;
}

//[[Rcpp::export]]
List S2Covering_internal(List x, std::string type, int max_cells, int min_level, int max_level, bool interior){
  // Zero or negative values corresponds to not setting that parameter.
  S2RegionCoverer coverer;
  if(max_cells > 0)
    coverer.set_max_cells(max_cells);
  if(min_level >= 0)
    coverer.set_min_level(min_level);
  if(max_level >= 0)
    coverer.set_max_level(max_level);
  vector<S2CellId> covering;
  if(type == "s2cap"){
    S2Cap region = S2CapFromR(x);
    if(interior){
      coverer.GetInteriorCovering(region, &covering);
    } else{
      coverer.GetCovering(region, &covering);
    }
  } else if(type == "s2polygon"){
    S2Polygon region;
    S2PolygonInitFromR(x, region);
    if(interior){
      coverer.GetInteriorCovering(region, &covering);
    } else{
      coverer.GetCovering(region, &covering);
    }
  } else{
    stop("Type must be s2cap or s2polygon.");
  }
  int n = covering.size();
  IntegerVector level(n);
  for(int i=0; i<n; i++){
    level[i] = covering[i].level();
  }
  return S2CellIdWrapForR(covering, level);
}
