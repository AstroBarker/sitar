#include <cstdio>
#include <math.h>
#include <numeric>

#include "atom.hpp"
#include "saha.hpp"
#include "solve.hpp"
#include "timer.hpp"
#include "utils.hpp"

int main( int argc, char *argv[] ) {

  // Atom H;
  // H.Z = 1;
  // H.g.push_back( 2 );
  // H.g.push_back( 1 );
  // H.chi.push_back( 13.6 );

  /**
   * Hydrogen test
   **/

  // fully ionized. works?
  // Real T = 3.35e9;
  // Real nk = 0.3;

  // should be neutral. works?
  // Real T  = 3339.0;
  // Real nk = 547118881533259.0;

  // Try to force the iterative solver
  // Real T = 6960.0;
  // Real nk = std::pow(10.0, 19.0);

  /**
   * Calcium test
   **/
  Atom Ca( 20, 20 );
  // Ca.Z = 20;
  Ca.g[0]  = 1.0;
  Ca.g[1]  = 2.0;
  Ca.g[2]  = 1.0;
  Ca.g[3]  = 6.0;
  Ca.g[4]  = 9.0;
  Ca.g[5]  = 4.0;
  Ca.g[6]  = 9.0;
  Ca.g[7]  = 6.0;
  Ca.g[8]  = 1.0;
  Ca.g[9]  = 2.0;
  Ca.g[10] = 1.0;
  Ca.g[11] = 6.0;
  Ca.g[12] = 9.0;
  Ca.g[13] = 4.0;
  Ca.g[14] = 9.0;
  Ca.g[15] = 6.0;
  Ca.g[16] = 1.0;
  Ca.g[17] = 2.0;
  Ca.g[18] = 1.0;
  Ca.g[19] = 2.0;
  Ca.g[20] = 1.0;

  Ca.chi[0]  = 6.11316;
  Ca.chi[1]  = 11.8717;
  Ca.chi[2]  = 50.9131;
  Ca.chi[3]  = 67.27;
  Ca.chi[4]  = 84.50;
  Ca.chi[5]  = 108.78;
  Ca.chi[6]  = 127.2;
  Ca.chi[7]  = 147.24;
  Ca.chi[8]  = 188.54;
  Ca.chi[9]  = 211.275;
  Ca.chi[10] = 591.9;
  Ca.chi[11] = 657.2;
  Ca.chi[12] = 726.6;
  Ca.chi[13] = 817.6;
  Ca.chi[14] = 894.5;
  Ca.chi[15] = 974.0;
  Ca.chi[16] = 1087.0;
  Ca.chi[17] = 1157.8;
  Ca.chi[18] = 5128.8;
  Ca.chi[19] = 5469.864;

  // set up temperature, nk grid
  // Fix Pe = n_e k T = 0.1
  const Real Pe     = 0.1;
  const Real n_grid = powint<14>( 2 );

  std::vector<Real> temps( n_grid, 0.0 );
  std::vector<Real> nks( n_grid, 0.0 );

  const Real T_start = 0.1 * powint<4>( 10.0 );
  const Real T_end   = 1.2 * powint<4>( 10.0 );
  const Real dT      = ( T_end - T_start ) / n_grid;

  // grid
  for ( int i = 0; i < n_grid; i++ ) {
    temps[i] = T_start + i * dT;
    nks[i]   = number_from_pressure_temperature( Pe, temps[i] );
    // std::printf("%e %e\n", temps[i], nks[i]);
  }

  const Real T  = 0.15 * powint<4>( 10.0 );
  const Real nk = number_from_pressure_temperature( Pe, T );
  std::printf( "T, nk = %e %e\n", T, nk );

  std::vector<Real> ion_frac( Ca.Z + 1, 0.0 );
  Real Zbar = Ca.Z;

  Timer timer;
  timer.start( );
  //SahaSolve( ion_frac, Zbar, T, &Ca, nk );
  std::printf("# T [K], nk, CaI, CaII, CaIII");
  for (int i = 0; i < n_grid; i++) {
    SahaSolve( ion_frac, Zbar, temps[i], &Ca, nks[i] );
    std::printf("%e %e %f %f %f\n", temps[i], nks[i], ion_frac[0],
    ion_frac[1], ion_frac[2]);
    std::fill(ion_frac.begin(), ion_frac.end(), 0); // reset ion_frac
  }
  timer.stop( );
  std::printf( " ~ Elapsed Time: %e ns\n", timer.elapsedNanoseconds( ) );
  std::printf( "%e\n", ion_frac[0] );
  std::printf( "sum ion fracs %e\n",
               std::reduce( ion_frac.begin( ), ion_frac.end( ) ) );
  return 0;
}
