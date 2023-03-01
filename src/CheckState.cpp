/**
 * File    :  CheckState.cpp
 * --------------
 *
 * Author  : Brandon L. Barker
 * Purpose : Ensure state is physical
 **/

#include <cmath>
#include <vector>

#include "Abstractions.hpp"
#include "CheckState.hpp"

// placeholder
void CheckState( std::vector<Real> &IonFrac, Real T, Real nk ) {
  assert( T > 0.0 && !std::isnan(T) );
  assert( nk > 0.0 && !std::isnan(nk) );
  // check ionfrac sums correctly
}
