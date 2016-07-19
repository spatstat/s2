//// Code to test basic R interface ////

#include <R.h>
#include "s2/s1interval.h"

extern "C" {
void interval_test_c(double* x, double* a, double* b, int* res) {
  S1Interval my_int(*a, *b);
  bool inside = my_int.Contains(*x);
  if(inside) {
    *res = 1;
  } else {
    *res = 0;
  }
}
}
