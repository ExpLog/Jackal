#ifndef GUARD_decomposition_h
#define GUARD_decomposition_h

#include "Matrix.h"

//TODO: learn about matrix conditioning and matrix preconditioning

/*
Decomposes matrix A into A = LU where L is lower triangular and U is upper triangular.
Returns a std::pair<Matrix,Matrix> = pair(L,U)
Note that this function might permutate the rows of A if necessary.
Throws an invalid_argument if matrix dimensions are nonpositive or A isn't square.
Still a simple implementation.
*/
std::pair<Matrix, Matrix> lu_decomp(Matrix& A);

#endif