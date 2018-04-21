#' Test of Containment in S2Cap
#'
#' Test whether the given object(s) on the sphere are contained in a cap. At the
#' moment only point containment can be tested.
#'
#' @param cap Named list containing axis and height of cap.
#' @param x Object(s) to test for containment in `cap`. At the moment only
#' points are handled and they must be specified as a three-column matrix.
#' @examples
#' cap <- list(axis = c(0,1,0), height = 0.1)
#' S2Cap_Contains(cap, matrix(c(0,1,0),ncol=3))
#' S2Cap_Contains(cap, matrix(c(1,0,0),ncol=3))
#' @export
S2Cap_Contains <- function(cap, x){
  S2Cap_contains_point(x, cap)
}
