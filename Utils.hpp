#ifndef _UTILS_HPP
#define _UTILS_HPP

/* templated power function for ints. faster */

template <int p>
double powint( double base );

template<>
double powint<0>( double ){
  return 1.0;
}

template<>
double powint<1>( double base ){
  return base;
}

template< int p >
double powint( double base ) {
  if constexpr ( ( p % 2 ) != 0 ) {
    return powint<p-1>( base ) * base;
  } else {
    double x = powint<p / 2>( base );
    return x * x;
  }
  
}

#endif // _UTILS_HPP
