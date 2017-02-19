#include "Rcpp_datatypes.h"
#include "s2/s2.h"
#include "s2/s2latlng.h"
#include <Rcpp.h>
namespace Rcpp {

  namespace traits {

  // Defined wrap case
  template <typename T> SEXP wrap(const Vector3<T> & obj){
    const int RTYPE = Rcpp::traits::r_sexptype_traits<T>::rtype ;
    Rcpp::Vector< RTYPE > x(3);
    x[0] = obj->x();
    x[1] = obj->y();
    x[2] = obj->z();
    return x;
  };

  // Defined as< > case
  template<typename T> class Exporter< Vector3<T> > {

    // Convert the type to a valid rtype.
    const static int RTYPE = Rcpp::traits::r_sexptype_traits< T >::rtype ;
    Rcpp::Vector<RTYPE> vec;

    public:
      Exporter(SEXP x) : vec(x) {
        Rcout << TYPEOF(x) << " , " << RTYPE << std::endl;
        if (TYPEOF(x) != RTYPE)
          throw std::invalid_argument("Internal type error: Make sure you supply double. I.e. don't use integers like 1L or 1:2");
      }
      Vector3<T> get() {

        // Need to figure out a way to perhaps do a pointer pass?
        Vector3<T> x(vec[0], vec[1], vec[2]);

        return x;
      }
    };
  }

template<> std::vector<S2Point> as(SEXP x) {
  // Lat,lng supplied as data.frame
  if( Rf_inherits(x, "data.frame") ){
    Rcpp::DataFrame df(x);
    if(df.size()!=2){
      Rcpp::stop("Expected a two column data.frame with lat,lng.");
    }
    Rcpp::NumericVector lat = df["lat"];
    Rcpp::NumericVector lng = df["lng"];
    if(lng.size()==0){
      lng = df["lon"];
    }
    if(lng.size()==0){
      lng = df["long"];
    }
    if(lng.size()==0){
      Rcpp::stop("Expected a two column data.frame with lat,lng.");
    }
    const int n = lat.size();
    std::vector<S2Point> rslt(n);
    for(int i = 0; i < n; i++){
      rslt[i] = S2LatLng::FromDegrees(lat[i], lng[i]).ToPoint();
    }
    return rslt;
  }
  // Unit sphere points supplied as 3 column matrix
  if( Rf_isMatrix(x) ){
    // Rcpp::Rcout << "It's a matrix!" << std::endl;
    Rcpp::NumericMatrix mat(x);
    Rcpp::NumericVector coord1 = mat( _, 0);
    Rcpp::NumericVector coord2 = mat( _, 1);
    Rcpp::NumericVector coord3 = mat( _, 2);
    const int n = coord1.size();
    std::vector<S2Point> rslt(n);
    for(int i = 0; i < n; i++){
      rslt[i] = S2Point(coord1[i], coord2[i], coord3[i]);
      // rslt[i] = S2Point();
    }
    return rslt;
  } 
  // None of the above. Generate error.
  Rcpp::stop("Can't interpret input as points on the sphere.");
  // None of the above. Assume it is a XPtr to the points.
  // Rcpp::XPtr<std::vector<S2Point>> ptr(x);
  // return *ptr;
}

template <> SEXP wrap(const std::vector<S2Point> &p){
  int n = p.size();
  Rcpp::NumericMatrix rslt(n, 3);
  for(int i = 0; i < n; i++){
    rslt(i,0) = p[i].x();
    rslt(i,1) = p[i].y();
    rslt(i,2) = p[i].z();
  }
  return rslt;
}
}

// Manual functions to interface C++ and R types for when Rcpp::as and Rcpp::wrap
// Don't work (the compiler can't automatically figure out what to do, and I
// can't figure out how to fix this right now. Not a big concern.)

// Get a vector of `S2Point`s from a three column matrix from R.
std::vector<S2Point> S2PointVecFromR(Rcpp::NumericMatrix mat){
  Rcpp::NumericVector coord1 = mat( Rcpp::_, 0);
  Rcpp::NumericVector coord2 = mat( Rcpp::_, 1);
  Rcpp::NumericVector coord3 = mat( Rcpp::_, 2);
  const int n = coord1.size();
  std::vector<S2Point> rslt(n);
  for(int i = 0; i < n; i++){
    rslt[i] = S2Point(coord1[i], coord2[i], coord3[i]);
    // rslt[i] = S2Point();
  }
  return rslt;
} 


// std::vector<S2Point> S2PointVecFromR(Rcpp::DataFrame df){
//   Rcpp::NumericVector lat = df["lat"];
//   Rcpp::NumericVector lng = df["lng"];
//   const int n = lat.size();
//   std::vector<S2Point> rslt(n);
//   for(int i = 0; i < n; i++){
//     rslt[i] = S2LatLng::FromDegrees(lat[i], lng[i]).ToPoint();
//   }
//   return rslt;
// }

