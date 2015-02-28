//#include <limits>
#include <stdexcept>
#include "Matrix.h"
#include "decomposition.h"

/*
Decomposes matrix A into A = LU where L is lower triangular and U is upper triangular.
Returns a std::pair<Matrix,Matrix> = pair(L,U)
Note that this function might permutate the rows of A if necessary.
Throws an invalid_argument if matrix dimensions are equal or A isn't square.
Still a simple implementation.
*/
std::pair<Matrix, Matrix> lu_decomp(Matrix& A){
	if (A.rows() < 0 || A.columns() < 0)
		throw std::invalid_argument("lu_decomp: A must have positive dimensions.");
	if (A.rows() != A.columns())
		throw std::invalid_argument("lu_decomp: matrix must be square.");

	std::pair<Matrix, Matrix> lu;
	lu.first = Matrix(A.rows(), A.columns(), 0.0);	//L
	lu.second = Matrix(A.rows(), A.columns(), 0.0);	//U

	int i, k, max_idx;
	double max;
	for (int j = 0; j < A.rows(); ++j){
		//we want to pivot matrix A, that is exchange the current row j
		//with the row k>j that has the greatest entry in column j
		max_idx = j;
		max = A(j, j);
		for (k = j + 1; k < A.rows(); ++k){
			if (max < A(k, j)){
				max = A(k, j);
				max_idx = k;
			}
		}
		if (abs(max) < DBL_EPSILON)
			throw std::domain_error("lu_decomp: A cannot be decomposed into LU.");

		A.exchangeRows(max_idx, j);
		lu.first.exchangeRows(max_idx, j);
		//lu.second.exchangeRows(max_idx, k);

		lu.second(0, j) = A(0, j);	//U(0,j) = A(0,j), always
		for (i = 1; i <= j; ++i){	//these two nested loops are performing U(i,j) = A(i,j) - \sum_{k=0}^{j-1} L(j,k)U(k,j)
			lu.second(i, j) = A(i, j);
			double *entry = &lu.second(i, j);
			for (k = 0; k <= j - 1; ++k){
				*entry -= lu.first(i, k)*lu.second(k, j);
			}
		}

		lu.first(j, j) = 1.0;
		for (i = j + 1; i < A.rows(); ++i){	//this continues from where the other loop left off
			lu.first(i, j) = A(i, j);
			double *entry = &lu.first(i, j);
			for (k = 0; k <= j - 1; ++k){
				*entry -= lu.first(i, k)*lu.second(k, j);
			}
			*entry = *entry / lu.second(j, j);
		}
	}

	return lu;
}