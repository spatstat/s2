#include "s2package_types.h"
#include <Rcpp.h>
#include "s2/s2loop.h"
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
