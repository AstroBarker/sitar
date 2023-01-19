#include <math.h>
#include <cstdio>

#include "Saha.hpp"

Real f( Real T, Real Ur, Real Urp1, Real chi ) {
  Real prefix = 2.0 * ( Urp1 / Ur ) * constants::k_saha * std::pow( T, 3./2. );
  Real suffix = std::exp( -chi / ( constants::k_Bev * T ) );
  
  std::printf("%e\n", prefix);
  std::printf("%.10f\n", suffix);
  return prefix * suffix;
}

int main( int argc, char *argv[] ) {

  Real result = f( 7000.0, 1.0, 2.0, 13.6 );
  std::printf( "%e\n", result );
  return 0;
}
