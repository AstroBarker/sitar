#ifndef SAHA_HPP_
#define SAHA_HPP_

#include "abstractions.hpp"
#include "atom.hpp"
#include "constants.hpp"

Real f( Real T, Atom atom, int n );
Real Target( Real Zbar, Real T, Atom atom, Real nh );

void SahaSolve( std::vector<Real>& ion_frac, Real Zbar, Real Temp, 
                Atom atom, Real nk );

template <int p>
inline Real IonFrac( Real Zbar, Real Temp, Atom atom, Real nh );

/* neutral state case */
template <>
inline Real IonFrac<0>( Real Zbar, Real T, Atom atom, Real nh ) {
  int Z            = atom.Z;
  Real denominator = 0.0;
  for ( int i = 1; i <= Z; i++ ) {
    Real inner_num = 1.0;
    for ( int j = 1; j <= i; j++ )
      inner_num *= ( i * f( T, atom, j ) );
    denominator += inner_num / std::pow( Zbar * nh, i );
  }
  return Zbar / denominator;
}

/* recursion */
template <int p>
inline Real IonFrac( Real Zbar, Real T, Atom atom, Real nh ) {
  return IonFrac<p - 1>( Zbar, T, atom, nh ) * f( T, atom, p ) / ( Zbar * nh );
}

#endif // SAHA_HPP_
