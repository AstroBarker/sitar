#ifndef ATOM_HPP_
#define ATOM_HPP_

#include <vector>

#include "abstractions.hpp"

struct Atom {
  int Z;
  std::vector<int> g;    // statistical weights. 0 is ground state
  std::vector<Real> chi; // ionization potential, eV
};

#endif // ATOM_HPP_
