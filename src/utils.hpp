#ifndef UTILS_HPP_
#define UTILS_HPP_

/* templated power function for ints. faster */

#include "abstractions.hpp"
#include "constants.hpp"

template <int p>
Real powint( Real base );

template<>
Real powint<0>( Real ){
  return 1.0;
}

template<>
Real powint<1>( Real base ){
  return base;
}

template< int p >
Real powint( Real base ) {
  if constexpr ( ( p % 2 ) != 0 ) {
    return powint<p-1>( base ) * base;
  } else {
    Real x = powint<p / 2>( base );
    return x * x;
  }
  
}

// Ideal gas P = n k T, find n given P, T
Real number_from_pressure_temperature( const Real P, const Real T ) {
  return P / (constants::k_B * T);
}

#endif // UTILS_HPP_
