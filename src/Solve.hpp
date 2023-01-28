/**
 * File    : Solve.hpp
 * Author  : Brandon Barker
 * Purpose : Implement Anderson-accelerated fixed point iteration
 * TODO: do the Anderson acceleration...
 **/

#include <cstdio>
#include <algorithm>

#include "SolverOpts.hpp"

template <typename T, typename F, typename... Args>
T Residual( F g, T x0, Args... args ) {
  return g( x0, args... ) - x0;
}

template <typename T, typename F, typename... Args>
T FixedPointSolve( F target, T x0, Args... args ) {

  unsigned int n = 0;
  T error        = 1.0;
  while ( n <= Opts::MAX_ITERS && error >= Opts::FPTOL ) {
    T x1  = target( x0, args... );
    error = std::abs( Residual( target, x0, args... ) );
    printf( " %d %f %f \n", n, x1, error );
    x0 = x1;
    n += 1;

    if ( n == Opts::MAX_ITERS ) {
      std::printf( " ! Not Converged ! \n" );
    }
  }

  return x0;
}

template <typename T, typename F>
T FixedPointSolve( F target, T x0 ) {

  unsigned int n = 0;
  T error        = 1.0;
  while ( n <= Opts::MAX_ITERS && error >= Opts::FPTOL ) {
    T x1  = target( x0 );
    error = std::abs( Residual( target, x0 ) );
    printf( " %d %f %f \n", n, x1, error );
    x0 = x1;
    n += 1;

    if ( n == Opts::MAX_ITERS ) {
      std::printf( " ! Not Converged ! \n" );
    }
  }

  return x0;
}
