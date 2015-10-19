#ifndef GUARD_decomposition_h
#define GUARD_decomposition_h

#include <utility>		//used std::pair
#include "matrix.h"

//TODO: learn about matrix conditioning and matrix preconditioning

/*
Decomposes matrix A into A = LU where L is lower triangular and U is upper triangular.
Returns a std::pair<Matrix,Matrix> = pair(L,U)
Note that this function might permutate the rows of A if necessary.
Throws an invalid_argument if at least one of the matrix dimensions is zero or if A isn't square.
Throws domain_error if the matrix cannot be decomposed in LU.
Still a simple implementation.
*/
std::pair<Matrix, Matrix> lu_decomp(Matrix& A);
//TODO: maybe create a extra parameter in the above function indicating if we can mess with A.
// If not, then we should copy the matrix before messing with it.

#endif