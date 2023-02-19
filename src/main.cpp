#include <math.h>
#include <cstdio>

#include "Atom.hpp"
#include "Saha.hpp"
#include "Solve.hpp"

int main( int argc, char *argv[] ) {

  Atom H;
  H.Z = 1;
  H.g.push_back( 1 );
  H.g.push_back( 2 );
  H.chi.push_back( 13.6 );

  // Real T = 3.35e9;
  // Real nh = 0.3;
  Real T  = 3339.0;
  Real nk = 547118881533259.0;
  //Real Zbar = FixedPointSolve( Target, 0.9, T, H, nk );
  //Real y    = IonFrac<0>( Zbar, T, H, nk );
  std::vector<Real> ion_frac(H.Z+2, 0.0);
  Real Zbar = 1.0;
  SahaSolve( ion_frac, Zbar, T, H, nk );
  std::printf( "%f\n", ion_frac[0] );
  return 0;
}
