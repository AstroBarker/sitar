#include <math.h>
#include <cstdio>

#include "atom.hpp"
#include "saha.hpp"
#include "solve.hpp"
#include "timer.hpp"

int main( int argc, char *argv[] ) {

  Atom H;
  H.Z = 1;
  H.g.push_back( 1 );
  H.g.push_back( 2 );
  H.chi.push_back( 13.6 );

  // fully ionized. works?
  //Real T = 3.35e9;
  //Real nk = 0.3;

  // should be neutral. works?
  //Real T  = 3339.0;
  //Real nk = 547118881533259.0;

  // Try to force the iterative solver
  Real T = 6960.0;
  Real nk = std::pow(10.0, 19.0);

  std::vector<Real> ion_frac(H.Z+1, 0.0);
  Real Zbar = 1.0;

  Timer timer;
  timer.start();
  SahaSolve( ion_frac, Zbar, T, H, nk );
  timer.stop();
  std::printf( "%f %e\n", ion_frac[0], timer.elapsedNanoseconds() );
  return 0;
}
