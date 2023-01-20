#include <math.h>
#include <cstdio>

#include "Atom.hpp"
#include "Saha.hpp"
#include "Solve.hpp"

Real f( Real T, Atom atom, int n ) {
  Real prefix = 2.0 * ( atom.g[n] / atom.g[n-1] ) 
    * constants::k_saha * std::pow( T, 1.5  );
  Real suffix = std::exp( - atom.chi[n-1] / ( constants::k_Bev * T ) );
  
  return prefix * suffix;
}

Real Target( Real Zbar, Real T, Atom atom, Real nh ) {
  /* atom data */
  int Z = atom.Z;

  // TODO: no need for two outer loops
  Real result = Zbar;
  Real numerator = 1.0;
  for ( int i = 1; i <= Z; i++ ) {
    Real inner_num = 1.0;
    for ( int j = 1; j <= i; j++ ) inner_num *= f( T, atom, j );
    numerator += inner_num / std::pow( Zbar * nh, i );
  }

  Real denominator = 0.0;
  for ( int i = 1; i <= Z; i++ ) {
    Real inner_num = 1.0;
    for ( int j = 1; j <= i; j++ ) inner_num *= (i * f( T, atom, j ));
    denominator += inner_num / std::pow( Zbar * nh, i );
  }

  result *= ( numerator / denominator );
  result = 1 - result + Zbar;
  return result;
}

int main( int argc, char *argv[] ) {

  Atom H;
  H.Z = 1;
  H.g.push_back(1);
  H.g.push_back(2);
  H.chi.push_back(13.6);

  //Real T = 3.35e9;
  //Real nh = 0.3;
  Real T = 3339.0;
  Real nh = 547118881533259.0;
  //std::printf("%f\n", test );
  Real Zbar = FixedPointSolve( Target,  0.9, T, H, nh );
  Real y = IonFrac<0>( Zbar, T, H, nh );
  std::printf("%f\n", y);
  return 0;
}
