#include <iostream>
#include <stdexcept>
#include <vector>
#include "matrix.h"

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
Matrix::Matrix (int rows, int columns, double defaultValue = 0.0) {
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
Matrix::Matrix (const Matrix &matrix) : _rows(matrix.rows()), _columns(matrix.columns()), _matrix(matrix._matrix){
}

/*
Overloads << so we can print a matrix.
*/
std::ostream& operator<< (std::ostream& os, const Matrix &matrix) {
	
	int j = 1; //counts the columns, so we can know when to end the line

	os << "Matrix " << matrix.rows() << "x" << matrix.columns() << std::endl << std::endl;

	//loop invariant: j == column number of the entry being read
	for (auto itr = matrix.begin(); itr != matrix.end(); ++itr) {
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
	ret.reserve( (std::vector<double>::size_type) _rows);

	//double to store the temporary value of the row's product
	double sum = 0.0;

	std::vector<double>::const_iterator vector_itr = vector.begin(); //const_iterator over vector

	for (const_iterator matrix_itr = this->begin(); matrix_itr != this->end(); ++matrix_itr){
		if ( vector_itr == vector.end() ) { //if we have calculated the product over a whole row
			ret.push_back(sum);
			vector_itr = vector.begin();
			sum = (*matrix_itr) * (*vector_itr);
			++vector_itr;
		}
		else{
		sum += (*matrix_itr) * (*vector_itr); 
		++vector_itr;
		}
	}
	ret.push_back(sum); //push_back the product of the last row
	
	return ret;
}

/*
Returns the number of rows of the matrix.
*/
int Matrix::rows () const {
	return _rows;
}

/*
Returns the number of columns of the matrix.
*/
int Matrix::columns () const {
	return _columns;
}

/*
Returns the total number of entries in _matrix.
*/
std::vector<double>::size_type Matrix::size() const
{
	return _matrix.size();
}