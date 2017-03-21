#' Approximate a Region on the Sphere by a Covering of S2Cells
#'
#' Approximate a region on the sphere by a (possibly interior) covering of
#' S2Cells.
#'
#' @param x Region to cover. Currently it must be a polygon or cap.
#' @param max_cells Positive integer. Maximal number of cells to use in the
#' covering.
#' @param min_level Integer between 0 and 30 specifying the lowest cell level to
#' use. Must be less than or equal to `max_level`.
#' @param max_level Integer between 0 and 30 specifying the highest cell level to
#' use. Must be greater than or equal to `min_level`.
#' @param interior Logical to get an interior covering.
#' @return A list containing an entry `ids` with the ids of the S2Cells used to
#' cover the region and possibly other entries for internal usage.
#' @export
S2Covering <- function(x, max_cells, min_level, max_level, interior = FALSE){
  if(missing(max_cells))
    max_cells <- -1
  if(missing(min_level))
    min_level <- -1
  if(missing(max_level))
    max_level <- -1
  type <- tolower(class(x))
  if(type == "s2polygon")
    x <- x$loops
  return(S2Covering_internal(x, max_cells, min_level, max_level, type = type, interior = interior))
}

#' Make a list of S2Cells
#'
#' Make a list of S2Cells
#'
#' @param x Input to create cells from. Currently only an object of class
#' S2CellId is supported.
#'
#' @export
S2Cell <- function(x){
  # if(inherits(x, "S2Covering"))
  #   x <- x$id
  v <- S2Cell_vertices_from_token(x$id)
  rslt <- append(x, list(vertices = v))
  class(rslt) <- "S2Cell"
  return(rslt)
}
