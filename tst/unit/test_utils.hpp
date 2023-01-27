#ifndef _TEST_UTILS_HPP_
#define _TEST_UTILS_HPP_

#include <cmath>

using Real = double;

/**
 * Test for near machine precision
 **/
bool SoftEqual( const Real &val, const Real &ref, const Real tol = 1.0e-8 ) {
  if ( std::fabs(val) < tol && std::fabs(ref) < tol ) {
    return true;
  } else {
    return false;
  }
}

#endif // _TEST_UTILS_HPP_
