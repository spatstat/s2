#include "Rcpp_datatypes.h"
#include <Rcpp.h>
#include <s2/s2.h>
#include <s2/s2cell.h>
#include <s2/s2polygon.h>
#include <s2/s2polygonbuilder.h>

using namespace Rcpp;

// Wrap an S2Polygon as a list in R with:
// [[1]] All the loops/rings as three column matrices.
// [[2]] The areas as numerics.
// [[3]] Indicators of holes as logicals
List S2PolygonWrapForR(const S2Polygon& poly){
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
      auto v = poly.loop(i)->vertex(j);
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

// Utility function to split a vector of point pairs into two vectors or points.
std::pair<std::vector<S2Point>,std::vector<S2Point>> splitEdgeList(S2PolygonBuilder::EdgeList x){
  int n = x.size();
  std::vector<S2Point> first(n), second(n);
  for(int i=0; i<n; i++){
    first[i] = x[i].first;
    second[i] = x[i].second;
  }
  std::pair<std::vector<S2Point>,std::vector<S2Point>> rslt(first, second);
  return rslt;
}

// Construct a S2Polygon from a list of three column matrices from R
void S2PolygonInitFromR(List list, S2Polygon& poly){
  // std::string S3class = list.attr("class");
  // if(S3class == "s2polygon"){ list = list[1]; }
  int n = list.size();
  S2PolygonBuilderOptions pbo;
  S2PolygonBuilder pb(pbo);
  for(int i = 0; i < n; i++){
    // std::vector<S2Point> points = as<std::vector<S2Point>>(list[i]);
    std::vector<S2Point> points = S2PointVecFromR(list[i]);
    S2Loop tmp;
    tmp.Init(points);
    pb.AddLoop(&tmp);
  }
  S2PolygonBuilder::EdgeList unused_edges;
  pb.AssemblePolygon(&poly, &unused_edges);
}

//' Coerce list to s2polygon format
//'
//' This is a lowlevel function with no checking of parameters. Use at own risk!
//' 
//' It builds an `S2Polygon` using the C++ polygon builder. See the header file
//' inst/include/s2/s2polygonbuilder.h for details on how to use the arguments.
//' 
//' @param x List of loops
//' @param validate Logical to validate the s2polygon. Default is `TRUE`.
//' @param xor_edges Logical to indicate that edges should be 'xor'ed to avoind
//' multiple loops with common edges. Default is `TRUE`.
//' @param vertex_merge_radius Numeric indicating that vertices within this
//' distance should be merged. Defaults to zero (i.e. no merging).
//' @param edge_splice_fraction Determines when edges are spliced. See C++
//' header as indicated in the description of this function. Default is 0.866.
//' @param undirected_edges Logical to indicate that input edges should be
//' considered undirected.  Default is `FALSE`.
//' multiple loops with common edges. Default is `TRUE`.
//' @export s2polygon
//[[Rcpp::export]]
List s2polygon(List x, bool validate = true, bool xor_edges = true,
               double vertex_merge_radius = 0, double edge_splice_fraction = 0.866,
               bool undirected_edges = false){
  int n = x.size();
  S2PolygonBuilderOptions pbo;
  pbo.set_validate(validate);
  pbo.set_xor_edges(xor_edges);
  pbo.set_vertex_merge_radius(S1Angle::Radians(vertex_merge_radius));
  pbo.set_edge_splice_fraction(edge_splice_fraction);
  pbo.set_undirected_edges(undirected_edges);
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
  auto rslt = S2PolygonWrapForR(poly);
  if(unused_edges.size()>0){
    Rcout << "There were unused loops/edges due to inconsistencies such as self-intersection." << std::endl;
    Rcout << "See 'unused' attribute of output." << std::endl;
    std::pair<std::vector<S2Point>,std::vector<S2Point>> start_end = splitEdgeList(unused_edges);
    rslt.attr("unused") = List::create( Named("start") = wrap(start_end.first),
                                        Named("end")   = wrap(start_end.second));
  }
  return rslt;
}

//' Union and intersection of two s2polygons
//'
//' Given two lists of three column matrices representing S2Polygons calculate
//' the union or intersection. Assumes the polygons have already been validated
//' and put into the correct format by [`s2polygon`]. Note the input is only the
//' loops/rings of two polygons not the entire objects with areas and hole
//' indicators.
//' 
//' @param x List of loops represented by three column matrices.
//' @param y List of loops represented by three column matrices.
//' @aliases s2polygon_intersection
//' @export s2polygon_union
//[[Rcpp::export]]
List s2polygon_union(List x, List y){
  S2Polygon poly1,poly2,poly12;
  S2PolygonInitFromR(x, poly1);
  S2PolygonInitFromR(y, poly2);
  poly12.InitToUnion(&poly1, &poly2);
  return S2PolygonWrapForR(poly12);
}

// //' @describeIn s2polygon_union Union of two s2polygons.
// //' @describeIn s2polygon_union Intersection of two s2polygons.

//' @export s2polygon_intersection
//[[Rcpp::export]]
List s2polygon_intersection(List x, List y){
  S2Polygon poly1,poly2,poly12;
  S2PolygonInitFromR(x, poly1);
  S2PolygonInitFromR(y, poly2);
  poly12.InitToIntersection(&poly1, &poly2);
  return S2PolygonWrapForR(poly12);
}

//' Point in s2polygon test
//'
//' Test whether points on the sphere are contained in a polygon on the sphere
//' 
//' @param points A three column matrix represtenting the points.
//' @param poly List of polygon loops represented by three column matrices.
//' @param approx Logical to use approximate testing of point in polygon (allows
//' points very slightly outside the polygon). Useful for allowing points
//' directly on the border.
//' @export s2polygon_contains_point
//[[Rcpp::export]]
LogicalVector s2polygon_contains_point(NumericMatrix points, List poly,
                                       bool approx = true){
  S2Polygon s2poly;
  S2PolygonInitFromR(poly, s2poly);
  auto s2points = S2PointVecFromR(points);
  int n = s2points.size();
  LogicalVector rslt(n);
  for(int i = 0; i < n; i++){
    if(approx){
      rslt(i) = s2poly.MayIntersect(S2Cell(s2points[i]));
    } else{
      rslt(i) = s2poly.Contains(s2points[i]);
    }
  }
  return rslt;
}
