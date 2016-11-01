#' @details
#' This package aims at providing bindings for the C++ library s2 for geometry
#' on the sphere. The C++ library was originally developed by Google under the
#' Apache license. At the moment very few things are exposed and the main
#' concern is to make the C++ code compile with the R toolchain on the three
#' major platforms and have the package accepted on CRAN. Once this proof of
#' concept has been achieved more of the underlying features will be made
#' available to the useR.
#'
#' @useDynLib s2
#' @importFrom Rcpp sourceCpp
"_PACKAGE"