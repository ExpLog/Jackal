#include <iostream>		//used std::ostream
#include <stdexcept>	//used std::invalid_arguments errors
#include <utility>		//used std::pair
#include <vector>		//used in the implementation of Matrix class
#include "Matrix.h"

/*
Initializes a Matrix by copying from a vector of vector of doubles.
Expects the vector< vector<double> > to be non-empty.
This constructor can be used to cast.
*/
Matrix::Matrix (const std::vector< std::vector<double> > &matrix) {
	if (matrix.empty()) {
		throw std::invalid_argument("Matrix: Cannot create an empty matrix.");
	}

	//used to check if the input matrix has all rows of same size
	std::vector<double>::size_type length = matrix[0].size();

	_rows = matrix.size();
	_columns = length;
	_matrix.reserve(_rows*_columns);	//reserves needed space for the whole matrix

	//copies the entries of the input into the private data structure
	for (const auto &line : matrix) {
		if (line.size() != length) {
			_rows = 0;
			_columns = 0;
			_matrix = std::vector<double>();
			_matrix.shrink_to_fit();	//shrinks the allocated capacity back to whatever is needed for an empty vector
			throw std::invalid_argument("Matrix: All the rows in a matrix must have the same length.");
		}
		for (const auto &entry : line) {
			_matrix.push_back(entry);
		}
	}
}

/*
Initializes a Matrix of dimensions rows x columns with the defaultValue.
Expects rows and columns to be positive integers. 
If no defaultValue is passed to the function, it initializes the matrix
with 0.0 in all entries.
*/
Matrix::Matrix(unsigned rows, unsigned columns, double defaultValue = 0.0) {
	if (rows <= 0 || columns <= 0) {
		_rows = 0;
		_columns = 0;
		_matrix = std::vector<double>();
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
}

/*
Overloads << so we can print a matrix.
*/
std::ostream& operator<< (std::ostream& os, const Matrix& matrix) {
	
	int j = 1; //counts the columns, so we can know when to end the line

	os << "Matrix " << matrix.rows() << "x" << matrix.columns() << std::endl;

	//loop invariant: j == column number of the entry being read
	for (auto itr = matrix.cbegin(); itr != matrix.cend(); ++itr) {
		if (j == 1){
			os << "[ " << *itr << ", ";
			++j;
			continue;
		}

		if ( j == matrix.columns() ) {
			os << *itr << " ]" << std::endl;
			j = 1;
			continue;		
		}

		os << *itr << ", ";
		++j;

		//TODO: include padding to make reading easier. 
		//First must implement a function that receives the matrix and finds out the number of digits of the largest entry.
		//Then use it to see how much padding we should use.

		//TODO: Maybe truncating the doubles is also a good idea.

	}
	return os;
}

/*
Overloads () to access an element [i,j] of the matrix.
*/
double& Matrix::operator() (std::vector<double>::size_type i, std::vector<double>::size_type j) {
	return _matrix[ i * (std::vector<double>::size_type)_columns + j ];
}

const double& Matrix::operator() (
	std::vector<double>::size_type i,
	std::vector<double>::size_type j) const {
	return _matrix[i * (std::vector<double>::size_type)_columns + j];
}


/*
Overloads * to multiply a matrix on the right by a vector.
Returns a vector<double> of dimension equal to the number of rows of the matrix.
If the matrix is empty, throws an invalid_argument error.
If the dimension of the vector is different from the number of columns of the matrix,
throws an invalid_argument error.
*/
std::vector<double> Matrix::operator* (std::vector<double> &vector){
	if ( this->empty() )
		throw std::invalid_argument("Matrix: the matrix must be non-empty.");

	if ( vector.size() != (std::vector<double>::size_type)this->columns() )
		throw std::invalid_argument("Matrix: the dimension of the vector must be the equal to the number of columns of the matrix.");

	//output vector which we will grow in a loop
	std::vector<double> ret;
	ret.reserve( static_cast<std::vector<double>::size_type>(_rows) );

	//double to store the temporary value of the row's product
	double sum;

	//vector and row iterators
	std::vector<double>::const_iterator vec_iter;
	const_iterator row_iter, row_end;

	for (int i = 0; i != _rows; ++i){
		vec_iter = vector.cbegin();
		row_iter = this->row_cbegin(i);
		row_end = this->row_cend(i);
		sum = 0.0;
		//it is not necessary to check vec_iter because
		//we are guaranteed that row_iter and vec_iter will iterate over the same number of elements 
		for (; row_iter != row_end; ++row_iter, ++vec_iter){
			sum += (*row_iter)*(*vec_iter);
		}
		ret.push_back(sum);
	}

	return ret;
}

/*
Returns the number of rows of the matrix.
*/
unsigned Matrix::rows () const {
	return _rows;
}

/*
Returns the number of columns of the matrix.
*/
unsigned Matrix::columns() const {
	return _columns;
}

/*
Multiplies a scalar by a matrix on the right.
Throws invalid_argument error if the matrix is empty.
*/
Matrix operator* (double scalar, Matrix matrix){

	if (matrix.empty())
		throw std::invalid_argument("Matrix: the matrix on the right must be non-empty.");

	for (Matrix::iterator itr = matrix.begin(); itr != matrix.end(); ++itr)
	{
		*itr *= scalar;
	}
	return matrix;
}


//elementary row operations
/*
Exchanges the rows row1 and row2. Expects both of them to be non-negative numbers.
Returns invalid_argument error if one of them does not exists.
*/
void Matrix::exchangeRows (std::vector<double>::size_type row1, std::vector<double>::size_type row2) {
	if ( ( row1 >= static_cast<std::vector<double>::size_type>(this->rows()) ) || ( row2 >= static_cast<std::vector<double>::size_type>(this->rows()) ) ) 
		//we used >= because row starts counting at zero.
		throw std::invalid_argument("Matrix: both rows must exist.");

	if (row1 == row2)
		return; //saves time when the rows are the same.

	double temp; //variable to store the temporary value of the entry being exchanged

	//reads the rows from beginning to end and exchange the entries between themselves.
	for (iterator itr1 = this->row_begin(row1), itr2 = this->row_begin(row2); itr1 != this->row_end(row1); ++itr1, ++itr2){
		temp = *itr1;
		*itr1 = *itr2;
		*itr2 = temp;
	}
	return;
}


/*
Multiplies a row by a double. Expects row to be a non-negative number.
Returns invalid_argument error if row does not exists.
*/
void Matrix::multiplyRow(std::vector<double>::size_type row, double scalar){
	if ( row >= static_cast<std::vector<double>::size_type>(this->rows()) ) //we used >= because row starts counting at zero.
		throw std::invalid_argument("Matrix: row must exist.");

	for (iterator itr = this->row_begin(row); itr != this->row_end(row); ++itr){
		*itr *= scalar;
	}
	return;
}

/*
Sums (scalar * row1) to row2. It doesn't change the values at row1.
Expects both rows to be non-negative integers.
Returns invalid_argument error if one of the rows doesn't exist.
*/
void Matrix::linearCombination(std::vector<double>::size_type row1, double scalar, std::vector<double>::size_type row2){
	if ( ( row1 >= static_cast<std::vector<double>::size_type>(this->rows()) ) || ( row2 >= static_cast<std::vector<double>::size_type>(this->rows()) ) ) 
		//we used >= because row starts counting at zero.
		throw std::invalid_argument("Matrix: both rows must exist.");

	if (scalar == 0){ //saves time
		return;
	}

	if (row1 == row2){ //saves time and memory when the rows are the same.
		this->multiplyRow(row2, scalar + 1);
		return; 
	}
	
	for (iterator itr1 = this->row_begin(row1), itr2 = this->row_end(row2); itr1 != row_end(row1); ++itr1, ++itr2){
		*itr2 += scalar * (*itr1);
	}
	return;
}