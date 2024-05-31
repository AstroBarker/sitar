#ifndef SAHA_HPP_
#define SAHA_HPP_

#include "abstractions.hpp"
#include "atom.hpp"
#include "constants.hpp"

Real f( const Real T, const Atom *atom, const int n );
Real Target( const Real Zbar, const Real T, const Atom *atom, const Real nh,
             const int min_state, const int max_state );
Real dTarget( const Real Zbar, const Real T, const Atom *atom, const Real nh,
              const int min_state, const int max_state );

void SahaSolve( std::vector<Real> &ion_frac, Real Zbar, const Real Temp,
                const Atom *atom, const Real nk );

Real IonFrac( const int p, const Real Zbar, const Real Temp, const Atom *atom,
              const Real nh, const int min_state, const int max_state );

#endif // SAHA_HPP_
