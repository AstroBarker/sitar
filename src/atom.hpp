#ifndef ATOM_HPP_
#define ATOM_HPP_

#include <vector>

#include "abstractions.hpp"

class Atom {
 public:
  Atom( const int Z_, const int A_ );
  int Z;
  int A;
  std::vector<Real> g;   // statistical weights. 0 is ground state
  std::vector<Real> chi; // ionization potential, eV
};

#endif // ATOM_HPP_
