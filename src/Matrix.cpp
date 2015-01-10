#include "matrix.h"

Matrix::Matrix(const std::vector< std::vector<double> > &matrix) {
	if (matrix.empty()) {
		throw std::invalid_argument("Matrix: Cannot create an empty matrix.");
	}

	//used to check if the input matrix has all lines of same size
	std::vector< std::vector<double> >::size_type length = matrix[0].size();

	_lines = matrix.size();
	_columns = length;

	//copies the entries of the input into the private data structure
	for (auto &line : matrix) {
		if (line.size() != length) {
			_lines = 0;
			_columns = 0;
			_matrix = std::vector<double>();
			throw std::invalid_argument("Matrix: All the lines in a matrix must have the same length.");
		}
		for (auto &entry : line) {
			_matrix.push_back(entry);
		}
	}
}

Matrix::Matrix(int line, int column, double defaultValue) {
	if (line <= 0 || column <= 0) {
		_lines = 0;
		_columns = 0;
		_matrix = std::vector<double>();
		throw std::invalid_argument("Matrix: both dimensions must be positive.");
	}
	_lines = line;
	_columns = column;
	_matrix = std::vector<double>(line*column, defaultValue);
}

Matrix::Matrix(const Matrix &matrix) : 
	_lines(matrix._lines),
	_columns(matrix._columns) {
	_matrix = std::vector<double>(matrix._matrix);
}

std::ostream& operator<< (std::ostream& os, const Matrix &matrix) {
	auto itr = matrix._matrix.begin();
	auto end = matrix._matrix.end();
	int j = 0; //counts the columns, so we can know when to end the line

	os << "Matrix " << matrix._lines << "x" << matrix._columns << std::endl;
	for (; itr != end; ++itr) {
		++j;
		if (j >= matrix._columns) {
			j = 0;
			os << *itr << std::endl;
			continue;		
		}
		os << *itr << ", ";
	}
	return os;
}