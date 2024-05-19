#ifndef SOLVEROPTS_HPP_
#define SOLVEROPTS_HPP_

/**
 * This file contains various solver options
 **/

#include "abstractions.hpp"

namespace Opts {

constexpr unsigned int MAX_ITERS = 200;
constexpr Real FPTOL = 1.0e-11;
constexpr Real ZBARTOL = 1.0e-13;
constexpr Real ZBARTOLINV = 1.0e13;

} // namespace Opts

#endif // SOLVEROPTS_HPP_
