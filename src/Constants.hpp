#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include <math.h> /* atan */

#include "Abstractions.hpp"

namespace constants {

constexpr Real PI( ) { return std::atan( 1 ) * 4; }
constexpr Real k_B   = 1.380649e-16;          // cgs
constexpr Real k_Bev   = 8.61733326e-5;       // eV / K
constexpr Real m_e   = 9.1093837e-28;         // cgs
constexpr Real h     = 6.62607015e-27;        // cgs
constexpr Real hbar  = 1.05457182e-27;        // cgs
constexpr Real c_cgs = 2.99792458e+10;        // cgs   
constexpr Real c = 1.0;                       // natural
constexpr Real k_saha = std::pow( 2. * PI() * m_e * k_B / (h * h), 1.5 );
 
} // namespace constants
#endif // _CONSTANTS_HPP_
