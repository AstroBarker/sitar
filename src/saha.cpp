#include <assert.h>
#include <cstdio>
#include <math.h>
#include <vector>

#include "atom.hpp"
#include "check_state.hpp"
#include "constants.hpp"
#include "saha.hpp"
#include "solve.hpp"

/* ionization function f */
Real f( const Real T, const Atom *atom, const int n ) {
  Real prefix = 2.0 * ( atom->g[n] / atom->g[n - 1] ) * constants::k_saha *
                std::pow( T, 1.5 );
  Real suffix = std::exp( -atom->chi[n - 1] / ( constants::k_Bev * T ) );

  return prefix * suffix;
}

Real Target( const Real Zbar, const Real T, const Atom *atom, const Real nh,
             const int min_state, const int max_state ) {
  /* atom data */
  int Z = atom->Z;

  // TODO: no need for two outer loops
  Real result    = Zbar;
  Real numerator = 1.0;
  for ( int i = min_state; i < max_state; i++ ) { // TODO: <= or <?
    Real inner_num = 1.0;
    for ( int j = min_state; j <= i; j++ )
      inner_num *= f( T, atom, j );
    numerator += inner_num / std::pow( Zbar * nh, i );
  }

  Real denominator = 0.0;
  for ( int i = min_state; i < max_state; i++ ) {
    Real inner_num = 1.0;
    for ( int j = min_state; j <= i; j++ ) {
      inner_num *= ( i * f( T, atom, j ) );
    }
    denominator += inner_num / std::pow( Zbar * nh, i );
  }
  denominator += ( min_state - 1.0 );

  result *= ( numerator / denominator );
  result = 1 - result; // + Zbar;
  return result;
}

Real dTarget( const Real Zbar, const Real T, const Atom *atom, const Real nh,
              const int min_state, const int max_state ) {
  const int Z = atom->Z;

  Real product = 1.0;
  Real sigma0  = 0.0;
  Real sigma1  = 0.0;
  Real sigma2  = 0.0;
  Real sigma3  = 0.0;

  for ( int i = min_state; i <= max_state; i++ ) { // TODO: <= or <?
    product *= f( T, atom, i ) / ( Zbar * nh );
    sigma0 += product;
    sigma1 += i * product;
    sigma2 += ( i - min_state + 1.0 ) * product;
    sigma3 += i * ( i - min_state + 1.0 ) * product;
  }

  const Real denom = 1.0 / ( min_state - 1.0 + sigma1 );
  return ( sigma2 - ( 1.0 + sigma1 ) * ( 1.0 + sigma3 * denom ) ) * denom;
}

void SahaSolve( std::vector<Real> &ion_frac, Real Zbar, const Real temperature,
                const Atom *atom, const Real nk ) {

  // formality
  CheckState( ion_frac, temperature, nk );

  const int Z = atom->Z;

  // const int Z = atom.Z;
  const int num_states = Z + 1; // TODO: plus 1 correct?
  int min_state        = 1;
  int max_state        = num_states;

  const Real Zbar_nk_inv = 1.0 / ( Zbar * nk );

  // TODO: check indices..
  for ( int i = 1; i < num_states; i++ ) {
    const Real saha_f = std::fabs( f( temperature, atom, i ) );

    if ( saha_f * Zbar_nk_inv > Opts::ZBARTOLINV ) {
      min_state   = i + 1;
      ion_frac[i] = 0.0;
    }
    if ( saha_f * Zbar_nk_inv < Opts::ZBARTOL ) {
      max_state = i;
      for ( int j = i + 1; j < num_states; j++ )
        ion_frac[j] = 0.0;
      break;
    }
  }

  if ( max_state == 1 ) {
    ion_frac[0] = 1.0;    // neutral
    Zbar        = 1.0e-6; // uncharged (but don't want division by 0)
  } else if ( min_state == num_states ) { // TODO:
    ion_frac[0] = IonFrac<0>( Zbar, temperature, atom, nk, min_state,
                              max_state ); // TODO: array
    ion_frac[num_states - 1] = 1.0;        // full ionization
    Zbar                     = Z;
  } else if ( min_state == max_state ) {
    Zbar                = min_state - 1.0;
    ion_frac[min_state] = 1.0; // only one state possible
  } else {                     // fixed point solver
    // const Real guess =
    //     ( temperature > 7000.0 )
    //         ? 0.75 * Z
    //         : 0.25; // arbitrary guess: "high" Zbar above H ionization temp
    const Real guess = 0.5 * Z;
    // Zbar = FixedPointAA( Target, guess, temperature, atom, nk, min_state,
    // max_state ); // FAILS
    Zbar = AANewton( Target, dTarget, guess, temperature, atom, nk, min_state,
                     max_state );
    //  TODO: loop over ionization states below
    //  Need template loop. or un-template IonFrac
    ion_frac[0] = IonFrac<0>( Zbar, temperature, atom, nk, min_state,
                              max_state ); // TODO: array
    ion_frac[1] =
        IonFrac<1>( Zbar, temperature, atom, nk, min_state, max_state );
    ion_frac[2] =
        IonFrac<2>( Zbar, temperature, atom, nk, min_state, max_state );
  }
}
