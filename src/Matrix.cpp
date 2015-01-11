#include <iostream>
#include <stdexcept>
#include <vector>
#include "matrix.h"

/*
Initializes a Matrix by copying from a vector of vector of doubles.
Expects the vector< vector<double> > to be nonempty.
This constructor can be used to cast.
*/
Matrix::Matrix (const std::vector< std::vector<double> > &matrix) {
	if (matrix.empty()) {
		throw std::invalid_argument("Matrix: Cannot create an empty matrix.");
	}

	//used to check if the input matrix has all rows of same size
	std::vector< std::vector<double> >::size_type length = matrix[0].size();

	_rows = matrix.size();
	_columns = length;

	//copies the entries of the input into the private data structure
	for (const auto &line : matrix) {
		if (line.size() != length) {
			_rows = 0;
			_columns = 0;
			_matrix = std::vector<double>();
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
Matrix::Matrix (const Matrix &matrix) { 
	_rows( matrix.rows() );
	_columns( matrix.columns() );
	_matrix = std::vector<double>(matrix._matrix);
}

/*
Overloads << so we can print a matrix.
*/
std::ostream& operator<< (std::ostream& os, const Matrix &matrix) {
	const auto itr = (matrix._matrix).begin();
	const auto end = (matrix._matrix).end();
	int j = 0; //counts the columns, so we can know when to end the line

	os << "Matrix " << matrix.rows() << "x" << matrix.columns() << std::endl;
	for (; itr != end; ++itr) {
		++j;
		
		if ( j == matrix.columns() ) {
			j = 0;
			os << *itr << " ]" << std::endl << "[ ";
			continue;		
		}
		os << *itr << ", ";
	}
	return os;
}

/*
Overloads [] to access an element [i,j] of the matrix.
*/
double& operator[] (std::vector< std::vector<double> >::size_type i, std::vector<double>::size_type j) {
	return _matrix[ (std::vector<double>::size_type)i * (std::vector<double>::size_type)_columns + j ];
}

/*
Returns the number of rows of the matrix.
*/
int Matrix::rows () {
	return _rows;
}

/*
Returns the number of columns of the matrix.
*/
int Matrix::columns () {
	return _columns;
}