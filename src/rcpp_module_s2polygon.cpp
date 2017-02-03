#include <RcppCommon.h>
#include "s2/s2cap.h"
#include "s2/s2region.h"
#include "s2/s2loop.h"
#include "s2/s2polygon.h"
#include "s2/s2polygonbuilder.h"

#include <vector>
using std::vector;
#include <string>

namespace Rcpp {
  template<> std::vector<S2Point> as( SEXP );
  template<> S2PolygonBuilder::EdgeList* as( SEXP ) ;
  template <> SEXP wrap(const std::vector<S2Point> &p);
  // namespace traits{
  //   template <typename T> SEXP wrap(const Vector3<T> & obj);
  //   template <typename T> class Exporter< Vector3<T> >;
  // }
}

#include <Rcpp.h>

namespace Rcpp {

  // namespace traits {
  // 
  // // Defined wrap case
  // template <typename T> SEXP wrap(const Vector3<T> & obj){
  //   const int RTYPE = Rcpp::traits::r_sexptype_traits<T>::rtype ;
  //   Rcpp::Vector< RTYPE > x(3);
  //   x[0] = obj->x();
  //   x[1] = obj->y();
  //   x[2] = obj->z();
  //   return x;
  // };
  // 
  // // Defined as< > case
  // template<typename T> class Exporter< Vector3<T> > {
  // 
  //   // Convert the type to a valid rtype. 
  //   const static int RTYPE = Rcpp::traits::r_sexptype_traits< T >::rtype ;
  //   Rcpp::Vector<RTYPE> vec;
  //   
  //   public:
  //     Exporter(SEXP x) : vec(x) {
  //       Rcout << TYPEOF(x) << " , " << RTYPE << std::endl;
  //       if (TYPEOF(x) != RTYPE)
  //         throw std::invalid_argument("Internal type error: Make sure you supply double. I.e. don't use integers like 1L or 1:2");
  //     }
  //     Vector3<T> get() {
  //       
  //       // Need to figure out a way to perhaps do a pointer pass?
  //       Vector3<T> x(vec[0], vec[1], vec[2]);
  //       
  //       return x;
  //     }
  //   };
  // }

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

template <> SEXP wrap(const std::vector<S2Point> &p){
  Rcpp::XPtr<std::vector<S2Point>> ptr( new std::vector<S2Point>(p), true );
  return ptr;
}
}

S2LatLng S2LatLngFromDegrees(S2LatLng* x, double lat_degrees, double lng_degrees) {
  return S2LatLng::FromDegrees(lat_degrees, lng_degrees);
}

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
Rcpp::NumericVector latlng(SEXP p) {
  S2Point x = Rcpp::as<S2Point>(p);
  S2LatLng y(x);
  Rcpp::NumericVector rslt(2);
  rslt[0] = y.lat().degrees();
  rslt[1] = y.lng().degrees();
  return rslt;
}

//[[Rcpp::export]]
Rcpp::NumericVector ll2point(Rcpp::NumericVector p) {
  S2LatLng y = S2LatLng::FromDegrees(p[0], p[1]);
  Rcpp::NumericVector rslt(3);
  S2Point x = y.ToPoint();
  rslt[0] = x.x();
  rslt[1] = x.y();
  rslt[2] = x.z();
  return rslt;
}

bool S2Loop_IsValid(S2Loop* loop){
  return loop->IsValid();
}
  
bool S2Polygon_IsValid(S2Polygon* poly){
  return poly->IsValid();
}

S2Cap S2Cap_FromAxisHeight(S2Cap* x, S2Point const& axis, double height) {
  return S2Cap::FromAxisHeight(axis, height);
}

S2Cap S2Cap_FromAxisAngle(S2Cap* x, S2Point const& axis, S1Angle const& angle) {
  return S2Cap::FromAxisAngle(axis, angle);
}

S2Cap S2Cap_FromAxisArea(S2Cap* x, S2Point const& axis, double area) {
  return S2Cap::FromAxisArea(axis, area);
}

RCPP_EXPOSED_CLASS(S1Angle);
RCPP_EXPOSED_CLASS(S2Cap);
RCPP_EXPOSED_CLASS(S2LatLng);
RCPP_EXPOSED_CLASS(S2Loop);
RCPP_EXPOSED_CLASS_NODECL(S2Point);
RCPP_EXPOSED_CLASS(S2Polygon);
RCPP_EXPOSED_CLASS(S2PolygonBuilder);
RCPP_EXPOSED_CLASS(S2PolygonBuilderOptions);
RCPP_EXPOSED_CLASS(S2LatLngRect);
RCPP_EXPOSED_CLASS(S2Region);

RCPP_MODULE(S2Polygon_module){
  using namespace Rcpp;
  
  class_<S2Cap>("S2Cap")
  .constructor()
  .method("axis", &S2Cap::axis)
  .method("area", &S2Cap::area)
  .method("angle", &S2Cap::angle)
  .method("height", &S2Cap::height)
  .method("FromAxisHeight", &S2Cap_FromAxisHeight)
  .method("FromAxisAngle", &S2Cap_FromAxisAngle)
  .method("FromAxisArea", &S2Cap_FromAxisArea)
  ;
  
  class_<S2Point>("S2Point")
  .constructor()
  .constructor<double,double,double>()
  .constructor<int,int,int>()
  .method("Fabs", &S2Point::Fabs)
  ;

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
  
  class_<S2LatLngRect>("S2LatLngRect")
  .constructor()
  .constructor<S2LatLng,S2LatLng>()
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
