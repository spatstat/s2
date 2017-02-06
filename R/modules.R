Rcpp::loadModule("S1Angle_module", TRUE)
Rcpp::loadModule("S2Cap_module", TRUE)
Rcpp::loadModule("S2LatLng_module", TRUE)
Rcpp::loadModule("S2LatLngRect_module", TRUE)
Rcpp::loadModule("S2Loop_module", TRUE)
Rcpp::loadModule("S2Point_module", TRUE)
Rcpp::loadModule("S2Polygon_module", TRUE)
Rcpp::loadModule("S2PolygonBuilder_module", TRUE)
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
