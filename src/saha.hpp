#ifndef SAHA_HPP_
#define SAHA_HPP_

#include "abstractions.hpp"
#include "atom.hpp"
#include "constants.hpp"

Real f( const Real T, const Atom *atom, const int n );
Real Target( const Real Zbar, const Real T, const Atom *atom, const Real nh,
             const int min_state, const int max_state );
Real dTarget( const Real Zbar, const Real T, const Atom *atom, const Real nh,
              const int min_state, const int max_state );

void SahaSolve( std::vector<Real> &ion_frac, Real Zbar, const Real Temp,
                const Atom *atom, const Real nk );

template <int p>
inline Real IonFrac( const Real Zbar, const Real Temp, const Atom *atom,
                     const Real nh, const int min_state, const int max_state );

/* neutral state case */
template <>
inline Real IonFrac<0>( const Real Zbar, const Real T, const Atom *atom,
                        const Real nh, const int min_state,
                        const int max_state ) {
  int Z            = atom->Z;
  Real denominator = 0.0;
  for ( int i = min_state; i < max_state; i++ ) {
    Real inner_num = 1.0;
    for ( int j = min_state; j <= i; j++ )
      inner_num *= ( i * f( T, atom, j ) );
    denominator += inner_num / std::pow( Zbar * nh, i );
  }
  denominator += ( min_state - 1.0 );
  return Zbar / denominator;
}

/* recursion */
template <int p>
inline Real IonFrac( const Real Zbar, const Real T, const Atom *atom,
                     const Real nh, const int min_state, const int max_state ) {
  return IonFrac<p - 1>( Zbar, T, atom, nh, min_state, max_state ) *
         f( T, atom, p ) / ( Zbar * nh );
}

#endif // SAHA_HPP_
