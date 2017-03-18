#' Construct a S2Polygon
#'
#' This function builds an `S2Polygon` using the C++ polygon builder. See the
#' header file inst/include/s2/s2polygonbuilder.h for details on how to use the
#' arguments.
#'
#' @param x Input to construct the polygon from. At the moment the only valid
#' input is a list of loops.
#' @param validate Logical to validate the S2Polygon. Default is `TRUE`.
#' @param xor_edges Logical to indicate that edges should be 'xor'ed to avoind
#' multiple loops with common edges. Default is `TRUE`.
#' @param vertex_merge_radius Numeric indicating that vertices within this
#' distance should be merged. Defaults to zero (i.e. no merging).
#' @param edge_splice_fraction Determines when edges are spliced. See C++
#' header as indicated in the description of this function. Default is 0.866.
#' @param undirected_edges Logical to indicate that input edges should be
#' considered undirected.  Default is `FALSE`.
#' multiple loops with common edges. Default is `TRUE`.
#' @export
S2Polygon <- function(x, validate = TRUE, xor_edges = TRUE, vertex_merge_radius = 0,
                      edge_splice_fraction = 0.866, undirected_edges = FALSE){
  S2PolygonBuild(x, validate, xor_edges, vertex_merge_radius,
                 edge_splice_fraction, undirected_edges)
}

#' Test of Containment in S2Polygon
#'
#' Test whether the given object(s) on the sphere are contained in a polygon. At
#' the moment only point containment can be tested.
#'
#' @param poly Named list containing an entry called `loops` containing a list
#' of polygon loops represented by three-column matrices.
#' @param x Object(s) to test for containment in `poly`. At the moment only
#' points are handled and they must be specified as a three-column matrix.
#' @param approx Logical to use approximate testing of point in polygon (allows
#' points very slightly outside the polygon). Useful for allowing points
#' directly on the border.
#' @export
S2Polygon_Contains <- function(poly, x, approx = TRUE){
  S2Polygon_contains_point(x, poly$loops, approx)
}
