#ifndef GUARD_Matrix_h
#define GUARD_Matrix_h

#include <iostream>
#include <vector>

class Matrix {
public:
	//constructors

	/*
	Default constructor, not to be used. 
	If it is, we value initialize the int variables, otherwise they would have garbage in them.
	This may avoid future bugs.
	*/
	Matrix(): _rows = 0, _columns = 0 {}

	/*
	Initializes a Matrix by copying from a vector of vector of doubles.
	Expects the vector< vector<double> > to be nonempty.
	This constructor can be used to cast.
	*/
	Matrix (const std::vector< std::vector<double> >&);

	/*
	Initializes a Matrix of dimensions rows x columns with the defaultValue.
	Expects rows and columns to be positive integers. 
	If no defaultValue is passed to the function, it initializes the matrix
	with 0.0 in all entries.
	*/
	Matrix (int rows, int columns, double defaultValue = 0.0);

	/*
	Copy constructor. Makes a deep copy of the input matrix.
	*/
	Matrix (const Matrix&);
	//end of constructors

	//printing
	/*
	Overloads << so we can print a matrix.
	*/
	friend std::ostream& operator<< (std::ostream&, const Matrix&);
	//end of printing

	/*
	Overloads [] to acess an element [i,j] of the matrix.
	*/
	double& operator[] (std::vector< std::vector<double> >::size_type, std::vector<double>::size_type);

	/*
	Returns the number of rows of the matrix.
	*/
	int rows ();

	/*
	Returns the number of columns of the matrix.
	*/
	int columns ();
	

	
private:
	//data structures
	std::vector<double> _matrix;
	int _rows;
	int _columns;
};



#endif
