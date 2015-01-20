#ifndef GUARD_Matrix_h
#define GUARD_Matrix_h

#include <iostream>	//used std::ostream
#include <utility>	//use std::pair
#include <vector>	//used in the implementation of Matrix class

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
	const double& operator() (std::vector<double>::size_type, std::vector<double>::size_type) const;

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
	inline int rows () const;

	/*
	Returns the number of columns of the matrix.
	*/
	inline int columns () const;

	/*
	Returns true if the matrix is empty, false otherwise.
	It is inlined to optimize performance.
	*/
	inline bool empty() const { return _matrix.empty(); }

	/*
	Returns an iterator pointing to the beginning of the matrix.
	It is inlined to optimize performance.
	*/
	inline iterator begin() {
		return _matrix.begin();
	}

	/*
	Returns an iterator pointing to the element right after the end of the matrix.
	It is inlined to optimize performance.
	*/
	inline iterator end() {
		return _matrix.end();
	}

	/*
	Returns a const iterator pointing to the beginning of the matrix.
	It is inlined to optimize performance.
	*/
	inline const_iterator cbegin() const {
		return _matrix.cbegin();
	}

	/*
	Returns a const iterator pointing to the element right after the end of the matrix.
	It is inlined to optimize performance.
	*/
	inline const_iterator cend() const { return _matrix.cend(); }

	/*
	Returns an iterator pointing to the beginning of the row "row" of the matrix.
	It is inlined to optimize performance.
	*/
	inline iterator row_begin (std::vector<double>::size_type row) {
		return this->begin() + row * (std::vector<double>::size_type) this->columns();
	}

	/*
	Returns an iterator pointing to the element right after the end of the row "row" of the matrix.
	It is inlined to optimize performance.
	*/
	inline iterator row_end (std::vector<double>::size_type row) {
		return this->begin() + (row + 1) * (std::vector<double>::size_type) this->columns();
	}

	/*
	Returns a const iterator pointing to the beginning of the row "row" of the matrix.
	It is inlined to optimize performance.
	*/
	inline const_iterator row_cbegin (std::vector<double>::size_type row) const {
		return this->cbegin() + row * (std::vector<double>::size_type) this->columns();
	}

	/*
	Returns a const iterator pointing to the element right after the end of the row "row" of the matrix.
	It is inlined to optimize performance.
	*/
	inline const_iterator row_cend (std::vector<double>::size_type row) const {
		return this->cbegin() + (row + 1) * (std::vector<double>::size_type) this->columns();
	}

	/*
	Returns a pair with the iterators returned by row_begin and row_end. 
	Returning a pair avoids calling row_begin and row_end with different arguments and comparing them.
	It is inlined to optimize performance.
	*/
	inline std::pair<iterator,iterator> row_itr(std::vector<double>::size_type row) {
		return std::make_pair<iterator,iterator>(this->row_begin(row), this->row_end(row));
	}

	/*
	Returns a pair with the const_iterators returned by row_cbegin and row_cend.
	Returning a pair avoids calling row_begin and row_end with different arguments and comparing them.
	It is inlined to optimize performance.
	*/
	inline std::pair<const_iterator, const_iterator> row_citr(std::vector<double>::size_type row) {
		return std::make_pair<const_iterator, const_iterator>( this->row_cbegin(row), this->row_cend(row) );
	}

	/*
	Exchanges the rows row1 and row2. Expects both of them to be non-negative integers.
	Returns invalid_argument error if one of them does not exists.
	*/
	void exchangeRows (std::vector<double>::size_type, std::vector<double>::size_type);

	/*
	Multiplies a row by a double. Expects row to be a non-negative integer.
	Returns invalid_argument error if row does not exists.
	*/
	void multiplyRow(std::vector<double>::size_type, double);

	/*
	Sums (scalar * row1) to row2. It doesn't change the values at row1.
	Expects both rows to be non-negative integers.
	Returns invalid_argument error if one of the rows doesn't exist.
	*/
	void linearCombination(std::vector<double>::size_type, double, std::vector<double>::size_type);
	
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

std::ostream& operator<< (std::ostream&, const Matrix&);

#endif