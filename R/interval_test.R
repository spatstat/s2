interval_test <- function(x, lower = -pi, upper = pi){
    stopifnot(lower<=upper && lower>=-pi && upper <= pi)
    as.logical(.C("interval_test_c", as.double(x), as.double(lower), as.double(upper), as.integer(0))[[4]])
}
