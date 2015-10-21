#include <cfloat>		//used std::DBL_EPSILON
#include <stdexcept>	//used std::invalid_argument
#include <utility>		//used std::pair and std::make_pair
#include "decomposition.h"
#include "matrix.h"

/*
Decomposes matrix A into A = LU where L is lower triangular and U is upper triangular.
Returns a std::pair<Matrix,Matrix> = pair(L,U)
Note that this function might permutate the rows of A if necessary.
Throws an invalid_argument if at least one of the matrix dimensions is zero or if A isn't square.
Throws domain_error if the matrix cannot be decomposed in LU.
Still a simple implementation.
*/
std::pair<Matrix, Matrix> lu_decomp(Matrix& A){
	if (!A.rows() || !A.columns())
		throw std::invalid_argument("lu_decomp: the matrix must have positive dimensions.");
	if (A.rows() != A.columns())
		throw std::invalid_argument("lu_decomp: the matrix must be square.");

	const size_t dimension = A.rows();
	Matrix L(A.rows(), A.columns(), 0.0);
	for (size_t i = 0; i < dimension; ++i) {
		L(i, i) = 1.0;
	}
	Matrix U(A.rows(), A.columns(), 0.0);

	// Variables used in the loop below. They are defined outside the loop to avoid creating and
	// destroying them at every iteration.
	// i will be the looping variable used in the inner loops.
	// k will be an index used in a matrix multiplication inside the second inner the loop
	// max_index will hold the index of the row with the current greatest entry in column j
	//(as long as it is greater than j).
	// max_entry will be the largest entry in the column j.
	size_t i, k, max_index;
	double max_entry;

	for (size_t j = 0; j < dimension; ++j){
		//we want to pivot matrix A, that is, exchange the current row j
		//with the row k>j that has the greatest entry in column j
		max_index = j;
		max_entry = A(j, j);
		for (k = j + 1; k < dimension; ++k){
			if (max_entry < A(k, j)){
				max_entry = A(k, j);
				max_index = k;
			}
		}
		if (abs(max_entry) < DBL_EPSILON)
			throw std::domain_error("lu_decomp: A cannot be decomposed into LU. The matrix is not diagonizable or its entries are too small.");

		A.exchangeRows(max_index, j);
		L.exchangeRows(max_index, j);

		U(0, j) = A(0, j);	//U(0,j) = A(0,j), always!
		for (i = 1; i <= j; ++i){	//these two nested loops are performing U(i,j) = A(i,j) - \sum_{k=0}^{j-1} L(j,k)U(k,j)
			double *entry = &U(i, j);
			*entry = A(i, j);
			for (k = 0; k <= j - 1; ++k){
				*entry -= L(i, k) * U(k, j);
			}
		}

		L(j, j) = 1.0;
		// let's define the variable quotient below so that U(j, j) is not calculated in every iteration of the loop below.
		double quocient = U(j, j); // it is never zero because U(j,j) = A(j,j) which is non-zero (otherwise would have thrown above).
		for (i = j + 1; i < dimension; ++i){	//this continues from where the other loop left off
			double *entry = &L(i, j);
			*entry = A(i, j);
			for (k = 0; k <= j - 1; ++k){
				*entry -= L(i, k) * U(k, j);
			}
			*entry = *entry / quocient;
		}
	}
	return std::make_pair(L, U);
	//TODO: correct mistakes in algorithm.
}