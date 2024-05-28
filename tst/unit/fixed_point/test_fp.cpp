#include <limits>
#include <math.h>

#include <catch2/catch_test_macros.hpp>

#include "test_utils.hpp"

#include "solve.hpp"
#include "funcs.hpp"

TEST_CASE( "Fixed Point", "Solvers" ) {
  const double truth = 0.607102;
  const double x0 = 0.5;
  double ans = FixedPointSolve( func, x0 );
  std::printf("%f\n", ans);
  REQUIRE( SoftEqual( ans, truth, 1.0e-5 ) );
}
