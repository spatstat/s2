#include "Rcpp_datatypes.h"
#include <Rcpp.h>
#include <s2/s2polygon.h>
#include <s2/s2polygonbuilder.h>

using namespace Rcpp;

//' Create an object of class s2polygon
//'
//' At the moment this function is very rough around the edges...
//' @param x List of loops
//' @param validate logical to add validate the
//' @export s2polygon
//[[Rcpp::export]]
List s2polygon(List x, bool validate = true){
  int n = x.size();
  S2PolygonBuilderOptions pbo;
  pbo.set_validate(validate);
  S2PolygonBuilder pb(pbo);
  bool tmp;
  for(int i = 0; i < n; i++){
    std::vector<S2Point> points = as<std::vector<S2Point>>(x[i]);
    S2Loop tmp;
    tmp.Init(points);
    pb.AddLoop(&tmp);
  }
  S2PolygonBuilder::EdgeList unused_edges;
  S2Polygon poly;
  pb.AssemblePolygon(&poly, &unused_edges);
  int nn = poly.num_loops();
  List out_loops( nn );
  NumericVector areas(nn);
  LogicalVector holes(nn);
  
  for(int i = 0; i < nn; i++){
    areas[i] = poly.loop(i)->GetArea();
    holes[i] = poly.loop(i)->is_hole();
    int mm = poly.loop(i)->num_vertices();
    NumericMatrix loop_i(mm, 3);
    for(int j = 0; j < mm; j++){
      S2Point v = poly.loop(i)->vertex(j);
      loop_i(j, 0) = v.x();
      loop_i(j, 1) = v.y();
      loop_i(j, 2) = v.z();
    }
    out_loops[i] = loop_i;
  }
  bool valid = poly.IsValid();
  auto rslt = List::create(Named("loops") = out_loops,
                           Named("areas") = areas,
                           Named("holes") = holes);
  rslt.attr( "class" ) = "s2polygon" ;
  return rslt;
}
