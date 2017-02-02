#include <RcppCommon.h>
#include "s2/s2loop.h"
#include "s2/s2polygon.h"
#include "s2/s2polygonbuilder.h"

#include <vector>
using std::vector;
#include <string>

namespace Rcpp {
template<> S2Point as( SEXP ) ;
template<> std::vector<S2Point> as( SEXP );
template<> S2PolygonBuilder::EdgeList* as( SEXP ) ;
template <> SEXP wrap(const S2Point &p);
template <> SEXP wrap(const std::vector<S2Point> &p);
}

#include <Rcpp.h>

namespace Rcpp {

template <> S2Point as(SEXP x) {
  if( Rf_isNumeric(x) ){
    Rcpp::NumericVector coord(x);
    if(coord.size() == 3){
      return S2Point(coord[0], coord[1], coord[2]);
    }
    if(coord.size() == 2){
      return S2LatLng::FromDegrees(coord[0], coord[1]).ToPoint();
    }
  } 
  Rcpp::XPtr<S2Point> ptr(x);
  return *ptr;
}

template<> std::vector<S2Point> as(SEXP x) {
// Lat,lng supplied as data.frame
  if( Rf_inherits(x, "data.frame") ){
    Rcpp::DataFrame df(x);
    Rcpp::NumericVector lat = df["lat"];
    Rcpp::NumericVector lng = df["lng"];
    const int n = lat.size();
    std::vector<S2Point> rslt(n);
    for(int i = 0; i < n; i++){
      rslt[i] = S2LatLng::FromDegrees(lat[i], lng[i]).ToPoint();
    }
    return rslt;
  }
// Unit sphere points supplied as 3 column matrix
  if( Rf_isMatrix(x) ){
    Rcpp::NumericMatrix mat(x);
    Rcpp::NumericVector coord1 = mat( _, 0);
    Rcpp::NumericVector coord2 = mat( _, 1);
    Rcpp::NumericVector coord3 = mat( _, 2);
    const int n = coord1.size();
    std::vector<S2Point> rslt(n);
    for(int i = 0; i < n; i++){
      rslt[i] = S2Point(coord1[i], coord2[i], coord3[i]);
    }
    return rslt;
  } 
  // None of the above. Assume it is a XPtr to the points.
  Rcpp::XPtr<std::vector<S2Point>> ptr(x);
  return *ptr;
}

template <> S2PolygonBuilder::EdgeList* as(SEXP x) {
  S2PolygonBuilder::EdgeList* rslt;
  return rslt;
}

template <> SEXP wrap(const S2Point &p){
  Rcpp::XPtr<S2Point> ptr( new S2Point(p), true );
  return ptr;
}

template <> SEXP wrap(const std::vector<S2Point> &p){
  Rcpp::XPtr<std::vector<S2Point>> ptr( new std::vector<S2Point>(p), true );
  return ptr;
}
}

S2LatLng S2LatLngFromDegrees(S2LatLng* x, double lat_degrees, double lng_degrees) {
  return S2LatLng::FromDegrees(lat_degrees, lng_degrees);
}

// Rcpp::NumericMatrix latlng(SEXP p) {
//   std::vector<S2Point> x = Rcpp::as<std::vector<S2Point>>(p);
//   int n = x.size();
//   Rcpp::NumericMatrix rslt(n,3);
//   for(int i = 0; i < n; i++){
//     rslt(i,0) = x[i].x();
//     rslt(i,1) = x[i].y();
//     rslt(i,2) = x[i].z();
//   }
//   return rslt;
// }


std::vector<std::string> dumploop(S2Loop* x){
  int n = x->num_vertices();
  std::vector<std::string> rslt(n);
  for(int i = 0; i < n; i++){
    rslt[i] = S2LatLng(x->vertex(i)).ToStringInDegrees();
  }
  return rslt;
}

Rcpp::DataFrame dumploop_numeric(S2Loop* x){
  int n = x->num_vertices();
  Rcpp::NumericVector lat(n),lng(n);
  S2LatLng tmp;
  for(int i = 0; i < n; i++){
    tmp = S2LatLng(x->vertex(i));
    lat[i] = tmp.lat().degrees();
    lng[i] = tmp.lng().degrees();
  }
  return Rcpp::DataFrame::create(Rcpp::Named("lng")=lng, Rcpp::Named("lat")=lat);
}

//[[Rcpp::export]]
Rcpp::List makepoly(Rcpp::List x){
  int n = x.size();
  std::vector<S2Point> points;
  // vector<S2Loop*>* loops;
  S2PolygonBuilderOptions pbo;
  S2PolygonBuilder pb(pbo);
  bool tmp;
  for(int i = 0; i < n; i++){
    points = Rcpp::as<std::vector<S2Point>>(x[i]);
    int m = points.size();
    for(int j = 1; j < m; j++){
      tmp = pb.AddEdge(points[j-1], points[j]);
      if(!tmp){
        // Rcpp::Rcout << S2LatLng(points[j]).ToStringInDegrees() << std::endl;
        Rcpp::Rcout << "HIT("<< i << "," << j << "):" << S2LatLng(points[j-1]).ToStringInDegrees() << " to " << S2LatLng(points[j]).ToStringInDegrees() << std::endl;
      }
    }
    // Rcpp::Rcout << "-------- Break -------" << std::endl;
    // loops->push_back(new S2Loop(points));
    // pb.AddLoop(loops[i]);
  }
  S2PolygonBuilder::EdgeList unused_edges;
  S2Polygon poly;
  pb.AssemblePolygon(&poly, &unused_edges);
  int nn = poly.num_loops();
  Rcpp::Rcout << "num_loops = "<< nn << std::endl;
  std::vector<std::string> out, loopinfo;
  Rcpp::NumericVector areas(nn);
  
  for(int i = 0; i < nn; i++){
    areas[i] = poly.loop(i)->GetArea()*poly.loop(i)->sign();
    loopinfo = dumploop(poly.loop(i));
    out.insert(out.end(), loopinfo.begin(), loopinfo.end());
    if(i < (nn-1)){
      out.push_back("loopend");
    }
  }
  bool valid = poly.IsValid();
  return Rcpp::List::create(Rcpp::Named("dump") = out,
                            Rcpp::Named("areas") = areas,
                            Rcpp::Named("valid") = valid);
}

//[[Rcpp::export]]
Rcpp::NumericVector latlng(SEXP p) {
  S2Point x = Rcpp::as<S2Point>(p);
  S2LatLng y(x);
  Rcpp::NumericVector rslt(2);
  rslt[0] = y.lat().degrees();
  rslt[1] = y.lng().degrees();
  return rslt;
}

bool S2Loop_IsValid(S2Loop* loop){
  return loop->IsValid();
}
  
bool S2Polygon_IsValid(S2Polygon* poly){
  return poly->IsValid();
}
  
RCPP_EXPOSED_CLASS(S1Angle);
RCPP_EXPOSED_CLASS(S2LatLng);
RCPP_EXPOSED_CLASS(S2Loop);
RCPP_EXPOSED_CLASS(S2Polygon);
RCPP_EXPOSED_CLASS(S2PolygonBuilder);
RCPP_EXPOSED_CLASS(S2PolygonBuilderOptions);

RCPP_MODULE(S2Polygon_module){
  using namespace Rcpp;
  
  class_<S1Angle>("S1Angle")
  .constructor()
  .constructor<S2Point,S2Point>()
  .method("degrees", &S1Angle::degrees)
  ;
  
  class_<S2LatLng>("S2LatLng")
  .constructor()
  .constructor<S1Angle,S1Angle>()
  .method("FromDegrees", &S2LatLngFromDegrees)
  .method("ToPoint", &S2LatLng::ToPoint)
  ;
  
  class_<S2Loop>("S2Loop")
  .constructor()
  .constructor<std::vector<S2Point>>()
  .method("Init", &S2Loop::Init)
  .method("GetArea", &S2Loop::GetArea)
  .method("Intersects", &S2Loop::Intersects)
  .method("IsValid", &S2Loop_IsValid)
  .method("BoundaryEquals", &S2Loop::BoundaryEquals)
  .method("vertex", &S2Loop::vertex)
  .method("sign", &S2Loop::sign)
  .method("dump", &dumploop_numeric)
  ;
  
  class_<S2Polygon>("S2Polygon")
  .constructor()
  .method("num_loops", &S2Polygon::num_loops)
  .method("num_vertices", &S2Polygon::num_vertices)
  .method("loop", &S2Polygon::loop)
  .method("IsValid", &S2Polygon_IsValid)
  .method("GetArea", &S2Polygon::GetArea)
  .method("IsNormalized", &S2Polygon::IsNormalized)
  .method("Project", &S2Polygon::Project)
  .method("GetCentroid", &S2Polygon::GetCentroid)
  .method("VirtualContainsPoint", &S2Polygon::VirtualContainsPoint)
  .method("InitToIntersection", &S2Polygon::InitToIntersection)
  .method("InitToUnion", &S2Polygon::InitToUnion)
  ;
  
  class_<S2PolygonBuilder>("S2PolygonBuilder")
  .constructor<S2PolygonBuilderOptions>()
  .method("AddLoop", &S2PolygonBuilder::AddLoop)
  .method("AssemblePolygon", &S2PolygonBuilder::AssemblePolygon)
  .method("AddEdge", &S2PolygonBuilder::AddEdge)
  ;
  
  class_<S2PolygonBuilderOptions>("S2PolygonBuilderOptions")
  .constructor()
  ;
}
