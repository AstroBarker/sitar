#ifndef SAHA_HPP_
#define SAHA_HPP_

#include "abstractions.hpp"
#include "atom.hpp"
#include "constants.hpp"

Real f( const Real T, const Atom* atom, const int n );
Real Target( const Real Zbar, const Real T, const Atom* atom, const Real nh );

void SahaSolve( std::vector<Real>& ion_frac, Real Zbar, const Real Temp, 
                const Atom* atom, const Real nk );

template <int p>
inline Real IonFrac( const Real Zbar, const Real Temp, const Atom* atom, const Real nh );

/* neutral state case */
template <>
inline Real IonFrac<0>( const Real Zbar, const Real T, const Atom* atom, const Real nh ) {
  int Z            = atom->Z;
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
inline Real IonFrac( const Real Zbar, const Real T, const Atom* atom, const Real nh ) {
  return IonFrac<p - 1>( Zbar, T, atom, nh ) * f( T, atom, p ) / ( Zbar * nh );
}

#endif // SAHA_HPP_
