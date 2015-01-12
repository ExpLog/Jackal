#ifndef GUARD_Matrix_h
#define GUARD_Matrix_h

#include <iostream>
#include <vector>

class Matrix {
public:

	/*
	The typedefs below create the interface for matrix iterators.
	They are input (i.e., read-only) iterators.
	Goes through one row at a time 
	(e.g.: starts at the first row, reads all elements in it; then goes to the beginning of the second row, and so on).
	*/
	typedef std::vector<double>::iterator iterator;
	typedef std::vector<double>::const_iterator const_iterator;

	//constructors

	/*
	Default constructor, not to be used. 
	If it is, we value initialize the int variables, otherwise they would have garbage in them.
	This may avoid future bugs.
	*/
	Matrix(): _rows(0), _columns(0) {}

	/*
	Initializes a Matrix by copying from a vector of vector of doubles.
	Expects the vector< vector<double> > to be non-empty.
	This constructor can be used to cast.
	*/
	Matrix (const std::vector< std::vector<double> >&);

	/*
	Initializes a Matrix of dimensions rows x columns with the defaultValue.
	Expects rows and columns to be positive integers. 
	If no defaultValue is passed to the function, it initializes the matrix
	with 0.0 in all entries.
	*/
	Matrix (int, int, double);

	/*
	Copy constructor. Makes a deep copy of the input matrix.
	*/
	Matrix (const Matrix&);
	//end of constructors

	/*
	Overloads () to access an element [i,j] of the matrix.
	*/
	double& operator() (std::vector<double>::size_type, std::vector<double>::size_type);

	/*
	Overloads * to multiply a matrix on the right by a vector.
	Returns a vector<double> of dimension equal to the number of rows of the matrix.
	If the dimension of the vector is different from the number of columns of the matrix,
	then it returns a ValueError.
	*/
	std::vector<double> operator* (std::vector<double>&);

	/*
	Returns the number of rows of the matrix.
	*/
	int rows () const;

	/*
	Returns the number of columns of the matrix.
	*/
	int columns () const;

	/*
	Returns true if the matrix is empty, false otherwise.
	It is inlined to optimize performance.
	*/
	inline bool empty() const { return _matrix.empty(); }


	/*
	Returns a const iterator pointing to the beginning of the matrix.
	It is inlined to optimize performance.
	*/
	inline const_iterator begin() const { return _matrix.begin(); }

	/*
	Returns a const iterator pointing to the element right after the end of the matrix.
	It is inlined to optimize performance.
	*/
	inline const_iterator end() const { return _matrix.end(); }
	
private:
	/*
	Returns the total number of entries in _matrix.
	*/
	std::vector<double>::size_type Matrix::size() const;

	//data structures
	std::vector<double> _matrix;
	int _rows;
	int _columns;
};
#endif