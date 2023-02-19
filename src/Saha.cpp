#include <assert.h>
#include <cstdio>
#include <math.h>
#include <vector>

#include "Atom.hpp"
#include "Saha.hpp"
#include "Solve.hpp"

/* ionization function f */
Real f( Real T, Atom atom, int n ) {
  Real prefix = 2.0 * ( atom.g[n] / atom.g[n - 1] ) * constants::k_saha *
                std::pow( T, 1.5 );
  Real suffix = std::exp( -atom.chi[n - 1] / ( constants::k_Bev * T ) );

  return prefix * suffix;
}

Real Target( Real Zbar, Real T, Atom atom, Real nh ) {
  /* atom data */
  int Z = atom.Z;

  // TODO: no need for two outer loops
  Real result    = Zbar;
  Real numerator = 1.0;
  for ( int i = 1; i <= Z; i++ ) {
    Real inner_num = 1.0;
    for ( int j = 1; j <= i; j++ )
      inner_num *= f( T, atom, j );
    numerator += inner_num / std::pow( Zbar * nh, i );
  }

  Real denominator = 0.0;
  for ( int i = 1; i <= Z; i++ ) {
    Real inner_num = 1.0;
    for ( int j = 1; j <= i; j++ )
      inner_num *= ( i * f( T, atom, j ) );
    denominator += inner_num / std::pow( Zbar * nh, i );
  }

  result *= ( numerator / denominator );
  result = 1 - result + Zbar;
  return result;
}

void SahaSolve( std::vector<Real>& ion_frac, Real Zbar, Real Temp, 
                Atom atom, Real nk ) {

  // formality
  assert( Temp > 0.0 );

  const int Z = atom.Z;

  //const int Z = atom.Z;
  const Real num_states = Z + 1; //TODO: plus 1 correct?
  int min_state = 1;
  int max_state = num_states;

  const Real Zbar_nk_inv = 1.0 / ( Zbar * nk );
  
  for ( int i = 1; i < Z; i++ ) {
    const Real saha_f = std::fabs( f( Temp, atom, i ) );
    if ( saha_f * Zbar_nk_inv > Opts::ZBARTOLINV ) { 
      min_state = i + 1;
      ion_frac[i] = 0.0;
    }
    if ( saha_f * Zbar_nk_inv < Opts::ZBARTOL ) { 
      max_state = i; 
      for ( int j = i + 1; j <= num_states; j++ ) ion_frac[j+1] = 0.0;
    }
  }

  if ( max_state == 1 ) {
    ion_frac[1] = 1.0; // neutral
    Zbar = 1.0e-6; // uncharged (but don't want division by 0)
  } else if ( min_state == num_states ) { // TODO:
    ion_frac[num_states] = 1.0; // full ionization
    Zbar = Z;
  } else if ( min_state == max_state ) {
    Zbar = min_state - 1.0;
    ion_frac[min_state] = 1.0; // only one state possible
  } else { // fixed point solver
    // TODO: will need to extend FPS
    Zbar        = FixedPointSolve( Target, 0.9, Temp, atom, nk );
    ion_frac[0] = IonFrac<0>( Zbar, Temp, atom, nk ); // TODO: array
    
  }

}
