/**
 * File    : Solve.hpp
 * Author  : Brandon Barker
 * Purpose : Implement Anderson-accelerated fixed point iteration
 * TODO: do the Anderson acceleration...
 **/

#include <cstdio>
#include <algorithm>

template <typename T, typename F, typename... Args>
T Residual( F g, T x0, Args... args ) {
  return g( x0, args... ) - x0;
}

template <typename T, typename F, typename... Args>
T FixedPointSolve( F target, T x0, Args... args ) {

  // Solver parameters
  constexpr unsigned int MAX_ITERS = 20;
  constexpr T TOL                  = 1e-7;

  unsigned int n = 0;
  T error        = 1.0;
  while ( n <= MAX_ITERS && error >= TOL ) {
    T x1  = target( x0, args... );
    error = std::abs( Residual( target, x0, args... ) );
    printf( " %d %f %f \n", n, x1, error );
    x0 = x1;
    n += 1;

    if ( n == MAX_ITERS ) {
      std::printf( " ! Not Converged ! \n" );
    }
  }

  return x0;
}
