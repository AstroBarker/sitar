#ifndef _ATOM_HPP_
#define _ATOM_HPP_

#include <vector>

#include "Abstractcions.hpp"

struct AtomicIon {
  int stage; // ionization stage ( 0 is neutral )
  int g; // statistical weight
  Real chi; // ionization potential, eV
}

#endif // _ATOM_HPP_
