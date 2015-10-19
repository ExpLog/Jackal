#ifndef GUARD_linear_solve_h
#define GUARD_linear_solve_h

#include <vector>
#include "decomposition.h"
#include "matrix.h"

// Check for throwable conditions in linear_solve.

/*
Solves a linear system Ax = b by decomposing A = LU, then solving Ly = b, followed by Ux = y.
Returns b.
*/
std::vector<double> linear_solve(const Matrix& A, const std::vector<double> b);

#endif