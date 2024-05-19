/**
 * File    :  atom.cpp
 * --------------
 *
 * Author  : Brandon L. Barker
 * Purpose : Atom class
 **/

#include "abstractions.hpp"
#include "atom.hpp"
#include "constants.hpp"

/**
 * Atom constructor
 **/
Atom::Atom( const int Z_, const int A_ ) : Z(Z_), A(A_), g(Z+1, 0.0), chi(Z, 0.0){}
