#ifndef GUARD_Matrix_h
#define GUARD_Matrix_h

#include <iostream>
#include <stdexcept>
#include <vector>

class Matrix {
public:
	//constructors
	/*
	Initializes a Matrix by copying from a vector of vector of doubles.
	Expects that the vector<vector<double>> to be nonempty.
	*/
	Matrix(const std::vector< std::vector<double> > &matrix);

	/*
	Initializes a Matrix of dimensions line x column with the defaultValue.
	Expects line and column to be positive integers
	*/
	Matrix(int line, int column, double defaultValue);

	/*
	Copy constructor. Makes a deep copy of the input matrix.
	*/
	Matrix(const Matrix &matrix);
	//end of constructors

	//printing
	/*
	Overloads << so we can print a matrix.
	*/
	friend std::ostream& operator<< (std::ostream& outs, const Matrix &matrix);
	//end of printing
private:
	//data structures
	std::vector<double> _matrix;
	int _lines;
	int _columns;
};



#endif