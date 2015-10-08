#include <algorithm>	//used std::max
#include <ostream>		//used std::ostream
#include <stdexcept>	//used std::invalid_arguments errors
#include <utility>		//used std::pair and std::swap
#include <vector>		//used in the implementation of Matrix class
#include "matrix.h"

/*
Initializes a Matrix by copying from a vector of vector of doubles.
Expects the vector< vector<double> > to be non-empty.
This constructor *can* be used to cast.
*/
Matrix::Matrix (const std::vector< std::vector<double> > &matrix) {
	if (matrix.empty()) {
		throw std::invalid_argument("Matrix: Cannot create an empty matrix.");
	}

	_rows = matrix.size();
	_columns = matrix[0].size();
	_matrix.reserve(_rows * _columns);	//reserves needed space for the whole matrix
	// Note that _rows and _columns are unsigned long long, while reserve asks for a
	// size_type argument. 

	// TODO: check what happens when an overflow happens above! PS: if reserve's argument is greater
	// than vector::max_size (which has size_type type), it throws a length_error. Nonetheless,
	// this comparison is probably done by casting the argument to size_type, which may throw away
	// leading bits and give us undefined behavior.

	//copies the entries of the input vector into the private data structure
	for (const auto &line : matrix) {
		if (line.size() != _columns) {
			throw std::invalid_argument("Matrix: All the rows in a matrix must have the same length.");
		}
		for (const auto &entry : line) {
			_matrix.push_back(entry);
		}
	}
}

/*
Initializes a Matrix of dimensions rows x columns with the defaultValue.
Expects rows and columns to be (unsigned and) positive. 
If no defaultValue is passed to the function, it initializes the matrix
with 0.0 in all entries.
*/
Matrix::Matrix(unsigned rows, unsigned columns, double defaultValue = 0.0) {
	if (!rows || !columns) {
		throw std::invalid_argument("Matrix: Both dimensions must be positive.");
	}
	_rows = rows;
	_columns = columns;
	_matrix = std::vector<double>(rows * columns, defaultValue);
}

/*
Copy constructor. Makes a deep copy of the input matrix.
*/
Matrix::Matrix (const Matrix &matrix) : _matrix(matrix._matrix), _rows(matrix.rows()), _columns(matrix.columns()){
} //Note that, since _matrix is a vector<double>, matrix is doing a deep copy.

/*
Overloads () to access an element [i,j] of the matrix.
*/
const double& Matrix::operator() (std::vector<double>::size_type i,
	std::vector<double>::size_type j) const {
	return _matrix[i * static_cast<std::vector<double>::size_type>(_columns) + j];
}
double& Matrix::operator() (std::vector<double>::size_type i, std::vector<double>::size_type j) {
	return const_cast<double&>((*(const_cast<Matrix* const>(this)))(i, j));
}

/*
Overloads * to multiply a matrix on the right by a vector.
Returns a vector<double> of dimension equal to the number of rows of the matrix.
If the matrix is empty, throws an invalid_argument error.
If the dimension of the vector is different from the number of columns of the matrix,
it throws an invalid_argument error.
*/
std::vector<double> Matrix::operator* (std::vector<double> &vector){
	if ( this->empty() )
		throw std::invalid_argument("Matrix: the matrix must be non-empty.");

	if ( vector.size() != static_cast<std::vector<double>::size_type>(this->columns() ))
		throw std::invalid_argument("Matrix: the dimension of the vector must be the equal to the number of columns of the matrix.");

	//output vector which we will grow in a loop
	std::vector<double> ret;
	ret.reserve( static_cast<std::vector<double>::size_type>(this->rows()) );

	//vector and row iterators
	std::vector<double>::const_iterator vec_iter;
	const_iterator row_iter, row_end;

	// Number of rows is calculated beforehand to avoid calling in the loop check.
	auto number_of_rows = this->rows();

	for (unsigned i{ 0 }; i != number_of_rows; ++i) {
		vec_iter = vector.cbegin();
		row_iter = this->row_cbegin(i);
		row_end = this->row_cend(i);
		double sum{ 0.0 };
		//it is not necessary to check vec_iter because we are guaranteed that
		//row_iter and vec_iter will iterate over the same number of elements 
		for (; row_iter != row_end; ++row_iter, ++vec_iter) {
			sum += (*row_iter)*(*vec_iter);
		}
		ret.push_back(sum);
	}

	return ret;
}

//elementary row operations
/*
Exchanges the rows row1 and row2.
Returns invalid_argument error if one of them does not exists, or *this if successful.
*/
Matrix Matrix::exchangeRows (std::vector<double>::size_type row1, std::vector<double>::size_type row2) {
	if ( std::max(row1, row2) >= static_cast<std::vector<double>::size_type>(this->rows()) ) 
		//we used >= because row starts counting at zero.
		throw std::invalid_argument("Matrix: both rows must be smaller than the matrix's dimensions.");

	if (row1 == row2)
		return *this; //saves time when the rows are the same.

	//reads the rows from beginning to end and exchange the entries between themselves.
	for (iterator itr1 = this->row_begin(row1), itr2 = this->row_begin(row2); itr1 != this->row_end(row1); ++itr1, ++itr2){
		std::swap(*itr1, *itr2);
	}
	return *this;
}


/*
Multiplies a row by a double.
Returns invalid_argument error if row does not exists, or *this if successful.
*/
Matrix Matrix::multiplyRow(std::vector<double>::size_type row, double scalar){
	if ( row >= static_cast<std::vector<double>::size_type>(this->rows()) )
		//we used >= because row starts counting at zero.
		throw std::invalid_argument("Matrix: row must exist.");

	for (iterator itr = this->row_begin(row); itr != this->row_end(row); ++itr){
		*itr *= scalar;
	}
	return *this;
}

/*
Sums (scalar * row1) to row2. It doesn't change the values at row1.
Returns invalid_argument error if one of the rows doesn't exist, or *this if successful.
*/
Matrix Matrix::linearCombination(double scalar, std::vector<double>::size_type row1, std::vector<double>::size_type row2){
	if ( std::max(row1, row2) >= static_cast<std::vector<double>::size_type>(this->rows()) )
		//we used >= because row starts counting at zero.
		throw std::invalid_argument("Matrix: both rows must exist.");

	if (scalar == 0.0){ //saves time
		return *this;
	}

	if (row1 == row2){ //saves time and memory when the rows are the same.
		return this->multiplyRow(row2, scalar + 1.0);
	}
	
	for (iterator itr1 = this->row_begin(row1), itr2 = this->row_end(row2); itr1 != row_end(row1); ++itr1, ++itr2){
		*itr2 += scalar * (*itr1);
	}
	return *this;
}


/*
Multiplies a scalar by a matrix on the right.
Throws invalid_argument error if the matrix is empty.
*/
Matrix operator* (double scalar, Matrix matrix) {

	if (matrix.empty())
		throw std::invalid_argument("Matrix: the matrix on the right must be non-empty.");

	for (Matrix::iterator itr = matrix.begin(); itr != matrix.end(); ++itr)
	{
		*itr *= scalar;
	}
	return matrix;
}

/*
Overloads << so we can print a matrix.
*/
std::ostream& operator<< (std::ostream& os, const Matrix& matrix) {
	
	os << "Matrix " << matrix.rows() << "x" << matrix.columns() << std::endl;

	unsigned j{ 1 }; //counts the columns, so we can know when to end the line

	//loop invariant: j == column number of the entry being read
	for (auto itr = matrix.cbegin(); itr != matrix.cend(); ++itr) {
		if (j == 1) {
			os << "[ " << *itr << ", ";
			++j;
		} else if ( j == matrix.columns() ) {
			os << *itr << " ]" << std::endl;
			j = 1;	
		} else {
			os << *itr << ", ";
			++j;
		}

		//TODO: include padding to make reading easier. 
		//First must implement a function that receives the matrix and finds out the number of digits of the largest entry.
		//Then use it to see how much padding we should use.
	}
	return os;
}
