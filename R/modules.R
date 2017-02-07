#' Wrapper classes for corresponding classes in Google's S2 geometry library
#' 
#' @description These classes are reference classes that wrap C++ classes of the
#'   same name.
#'   
#' @name S2-classes
#' @aliases S1Angle S2Cap S2LatLng S2LatLngRect S2Loop S2Point S2Polygon S2PolygonBuilder S2PolygonBuilderOptions S1Angle-class S2Cap-class S2LatLng-class S2LatLngRect-class S2Loop-class S2Point-class S2Polygon-class S2PolygonBuilder-class S2PolygonBuilderOptions-class
#' @rdname S2-classes
#' @exportClass S1Angle S2Cap S2LatLng S2LatLngRect S2Loop S2Point S2Polygon S2PolygonBuilder S2PolygonBuilderOptions
#' @export S1Angle S2Cap S2LatLng S2LatLngRect S2Loop S2Point S2Polygon S2PolygonBuilder S2PolygonBuilderOptions
setRcppClass("S1Angle", module = "S1Angle_module")
setRcppClass("S2Cap", module = "S2Cap_module")
setRcppClass("S2LatLng", module = "S2LatLng_module")
setRcppClass("S2LatLngRect", module = "S2LatLngRect_module")
setRcppClass("S2Loop", module = "S2Loop_module")
setRcppClass("S2Point", module = "S2Point_module")
setRcppClass("S2Polygon", module = "S2Polygon_module")
setRcppClass("S2PolygonBuilder", module = "S2PolygonBuilder_module")
setRcppClass("S2PolygonBuilderOptions", module = "S2PolygonBuilder_module")
# Rcpp::loadModule("S1Angle_module", TRUE)
# Rcpp::loadModule("S2Cap_module", TRUE)
# Rcpp::loadModule("S2LatLng_module", TRUE)
# Rcpp::loadModule("S2LatLngRect_module", TRUE)
# Rcpp::loadModule("S2Loop_module", TRUE)
# Rcpp::loadModule("S2Point_module", TRUE)
# Rcpp::loadModule("S2Polygon_module", TRUE)
# Rcpp::loadModule("S2PolygonBuilder_module", TRUE)
# setRcppClass("S2Polygon", module = "S2Polygon_module",
#              methods = list(
#                test = function(x) print(x),
#                show = function() cat("Custom print message."))
#              )
# exposeClass("S2Cap",
#             constructors = list(""),
#             fields = list(""),
#             methods = "area",
#             header = '#include "s2/s2cap.h')
