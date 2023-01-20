#ifndef _ATOM_HPP_
#define _ATOM_HPP_

#include <vector>

#include "Abstractions.hpp"

struct Atom {
  int Z;
  std::vector<int> g;    // statistical weights. 0 is ground state
  std::vector<Real> chi; // ionization potential, eV
};

#endif // _ATOM_HPP_
