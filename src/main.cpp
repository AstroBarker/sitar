#include <math.h>
#include <cstdio>

#include "Atom.hpp"
#include "Saha.hpp"
#include "Solve.hpp"

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
