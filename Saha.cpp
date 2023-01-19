#include <math.h>
#include <cstdio>

#include "Atom.hpp"
#include "Saha.hpp"

Real f( Real T, Atom atom, int n ) {
  Real prefix = 2.0 * ( atom.g[n+1] / atom.g[n] ) 
    * constants::k_saha * std::pow( T, 3./2. );
  Real suffix = std::exp( - atom.chi[n] / ( constants::k_Bev * T ) );
  
  return prefix * suffix;
}

int main( int argc, char *argv[] ) {

  Atom H;
  H.Z = 1;
  H.g.push_back(1);
  H.g.push_back(2);
  H.chi.push_back(13.6);

  Real Result = f( 7000.0, H, 0 );
  std::printf("%f\n", Result);
  return 0;
}
