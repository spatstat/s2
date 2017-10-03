#include "Rcpp_datatypes.h"
#include <Rcpp.h>
#include <s2/s2.h>
#include <s2/s2cell.h>
#include <s2/s2polygon.h>
#include <s2/s2polygonbuilder.h>
#include <s2/s2polyline.h>

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
  // bool valid = poly.IsValid();
  S2LatLngRect bound = poly.GetRectBound();
  auto rslt = List::create(Named("loops") = out_loops,
                           Named("areas") = areas,
                           Named("holes") = holes,
                           Named("bound") = S2LatLngRectToR(bound));
  rslt.attr( "class" ) = "S2Polygon" ;
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
  // if(S3class == "S2Polygon"){ list = list[1]; }
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

// Construct a S2Polygon from a list of three column matrices from R
// void S2PolygonNoBuilderInitFromR(List list, S2Polygon& poly, std::vector<S2Loop*>& looplist){
//   int n = list.size();
//   for(int i = 0; i < n; i++){
//     // std::vector<S2Point> points = as<std::vector<S2Point>>(list[i]);
//     std::vector<S2Point> points = S2PointVecFromR(list[i]);
//     looplist[i]->Init(points);
//   }
//   poly.Init(&looplist);
// }

//[[Rcpp::export]]
List S2PolygonBuild(List x, bool validate = true, bool xor_edges = true,
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
    // std::vector<S2Point> points = as<std::vector<S2Point>>(x[i]);
    std::vector<S2Point> points = S2PointVecFromR(x[i]);
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
    rslt.attr("unused") = List::create( Named("start") = S2PointVecToR(start_end.first),
                                        Named("end")   = S2PointVecToR(start_end.second));
  }
  return rslt;
}

//' Union and intersection of two s2polygons
//'
//' Given two lists of three column matrices representing S2Polygons calculate
//' the union or intersection. Assumes the polygons have already been validated
//' and put into the correct format by [`S2Polygon`]. Note the input is only the
//' loops/rings of two polygons not the entire objects with areas and hole
//' indicators.
//' 
//' @param x List of loops represented by three-column matrices.
//' @param y List of loops represented by three-column matrices.
//' @aliases S2Polygon_intersection
//' @export S2Polygon_union
//[[Rcpp::export]]
List S2Polygon_union(List x, List y){
  S2Polygon poly1,poly2,poly12;
  S2PolygonInitFromR(x, poly1);
  S2PolygonInitFromR(y, poly2);
  poly12.InitToUnion(&poly1, &poly2);
  return S2PolygonWrapForR(poly12);
}

// //' @describeIn S2Polygon_union Union of two S2Polygons.
// //' @describeIn S2Polygon_union Intersection of two S2Polygons.

//' @export S2Polygon_intersection
//[[Rcpp::export]]
List S2Polygon_intersection(List x, List y){
  S2Polygon poly1,poly2,poly12;
  S2PolygonInitFromR(x, poly1);
  S2PolygonInitFromR(y, poly2);
  poly12.InitToIntersection(&poly1, &poly2);
  return S2PolygonWrapForR(poly12);
}

//' intersection of sets of s2polygons
//'
//' this function generalizes S2Polygon_intersection, by allowing two lists of
//' polygons, each list element equal to a polygon of that function.
//' 
//' @param x List of list of loops represented by three-column matrices.
//' @param y List of list of loops represented by three-column matrices.
//' @aliases S2Polygons_intersection
//' @export S2Polygons_intersection
//[[Rcpp::export]]
List S2Polygons_intersection(List x, List y){
  std::vector<S2Polygon> polx(x.size());
  std::vector<S2Polygon> poly(y.size());
  List rslt(x.size() * y.size());
  for (size_t i = 0; i < x.size(); i++)
    S2PolygonInitFromR(x[i], polx[i]);
  for (size_t j = 0; j < y.size(); j++)
    S2PolygonInitFromR(y[j], poly[j]);
  size_t n_empty = 0;
  for (size_t i = 0; i < x.size(); i++) {
    for (size_t j = 0; j < y.size(); j++) {
      S2Polygon poly12;
      if (polx[i].Intersects(&(poly[j])))
        poly12.InitToIntersection(&(polx[i]), &(poly[j]));
      if (poly12.num_loops() == 0)
        n_empty++;
      rslt[(i * y.size()) + j] = S2PolygonWrapForR(poly12);
    }
  }
  rslt.attr( "class" ) = "S2Polygons";
  rslt.attr( "n_empty" ) = n_empty;
  return rslt;
}

//' for two sets of s2polygons, which ones intersect?
//'
//' this function is equivalent to \link[sf]{st_intersects}, in that it
//' returns a sparse matrix with indexes for pairs of intersecting polygons
//' 
//' @param x List of list of loops represented by three-column matrices.
//' @param y List of list of loops represented by three-column matrices.
//' @aliases S2Polygons_intersect
//' @export S2Polygons_intersect
//[[Rcpp::export]]
List S2Polygons_intersect(List x, List y) {
  std::vector<S2Polygon> polx(x.size());
  std::vector<S2Polygon> poly(y.size());
  List rslt(x.size());
  for (size_t i = 0; i < x.size(); i++)
    S2PolygonInitFromR(x[i], polx[i]);
  for (size_t j = 0; j < y.size(); j++)
    S2PolygonInitFromR(y[j], poly[j]);
  for (size_t i = 0; i < x.size(); i++) {
    std::vector<int> indx;
    for (size_t j = 0; j < y.size(); j++) {
      if (polx[i].Intersects(&(poly[j])))
        indx.push_back(j + 1);
    }
    rslt[i] = indx;
  }
  return rslt;
}

//' compute centroids for a list of s2polygons
//'
//' this function is equivalent to \link[sf]{st_centroid}, in that it
//' returns a numeric vector with polygon centroids
//' 
//' @param x List of list of loops represented by three-column matrices.
//' @aliases S2Polygons_centroid
//' @export S2Polygons_centroid
//[[Rcpp::export]]
NumericMatrix S2Polygons_centroid(List x) {
  std::vector<S2Point> points(x.size());
  S2Polygon s2poly;
  for (size_t i = 0; i < x.size(); i++) {
    S2PolygonInitFromR(x[i], s2poly);
    points[i] = s2poly.GetCentroid() / s2poly.GetArea();
  }
  return S2PointVecToR(points);
}

//' compute areas for a list of s2polygons
//'
//' this function is equivalent to \link[sf]{st_area}, in that it
//' returns a numeric vector with polygon areas
//' 
//' @param x List of list of loops represented by three-column matrices.
//' @aliases S2Polygons_area
//' @details the area is on the unit sphere, in [0, 4 * pi]
//' @export S2Polygons_area
//[[Rcpp::export]]
NumericVector S2Polygons_area(List x) {
  NumericVector rslt(x.size());
  S2Polygon s2poly;
  for (size_t i = 0; i < x.size(); i++) {
    S2PolygonInitFromR(x[i], s2poly);
    rslt[i] = s2poly.GetArea();
  }
  return rslt;
}

//[[Rcpp::export]]
LogicalVector S2Polygon_contains_point(NumericMatrix points, List poly,
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

std::vector<S2Point> S2PointVectorFromS2Polygon(S2Polygon& poly, int loopindex){
  int n = poly.loop(loopindex)->num_vertices();
  std::vector<S2Point> rslt(n);
  // Notice "<=" here to repeat first vertex at end.
  for(int i = 0; i <= n; i++){
    rslt[i] = poly.loop(loopindex)->vertex(i%n);
  }
  return rslt;
}

double S2PointDist(S2Point& x, S2Point& y){
  S2Point cross = x.CrossProd(y);
  double dot = x.DotProd(y);
  return atan2(cross.Norm(), dot);
}

// //' @export S2Polygon_border_dist
// //[[Rcpp::export]]
// NumericVector S2Polygon_border_dist(NumericMatrix points, List poly){
//   int junk;
//   S2Polygon s2poly;
//   S2PolygonInitFromR(poly, s2poly);
//   auto s2points = S2PointVecFromR(points);
//   int npoints = s2points.size();
//   int nloops = s2poly.num_loops();
//   NumericVector rslt(npoints);
//   for(int i = 0; i < nloops; i++){
//     double dist = 3.14;
//     std::vector<S2Point> vertices = S2PointVectorFromS2Polygon(s2poly, i);
//     S2Polyline line(vertices);
//     for(int j = 0; j < npoints; j++){
//       S2Point projected = line.Project(s2points[j], &junk);
//       dist = min(dist, S2PointDist(s2points[j], projected));
//     }
//     rslt(i) = dist;
//   }
//   return rslt;
// }

//' Distance from points to line on sphere
//'
//' Distance from points to line on sphere.
//' 
//' @param line Line represented by a sequence of vertices given as a single
//' three-column matrices with one line per vertex.
//' @param x Points represented by three-column matrices.
//' @export S2Polyline_dist
//[[Rcpp::export]]
NumericVector S2Polyline_dist(NumericMatrix line, NumericMatrix x){
  // Read loop as a vector of points (adding the closing vertex)
  auto vertices = S2PointVecFromR(line);
  auto points = S2PointVecFromR(x);
  S2Polyline polyline(vertices);
  // Loop through points, project to line and calculate dist.
  int junk;
  double dist;
  int npoints = points.size();
  NumericVector rslt(npoints);
  for(int i = 0; i < npoints; i++){
    S2Point projected = polyline.Project(points[i], &junk);
    rslt(i) = S2PointDist(points[i], projected);
  }
  return rslt;
}
