#ifndef GUARD_matrix_h
#define GUARD_matrix_h

#include <ostream>	//used std::ostream
#include <utility>	//used std::pair
#include <vector>	//used in the implementation of Matrix class

class Matrix {
public:

	/*
	The typedefs below create the interface for matrix iterators.
	It goes through one row at a time (e.g.: starts at the first row, reads all elements in it;
	then goes to the beginning of the second row, and so on).
	*/
	typedef std::vector<double>::iterator iterator;
	typedef std::vector<double>::const_iterator const_iterator;

	//constructors

	/*
	We do not define a default constructor on purpose. 
	*/

	/*
	Initializes a Matrix by copying from a vector of vector of doubles.
	Expects the vector< vector<double> > to be non-empty.
	This constructor *can* be used to cast.
	*/
	Matrix (const std::vector< std::vector<double> >&);

	/*
	Initializes a Matrix of dimensions rows x columns with the defaultValue.
	Expects rows and columns to be (size_t and) positive. 
	If no defaultValue is passed to the function, it initializes the matrix
	with 0.0 in all entries.
	*/
	Matrix (size_t, size_t, double);

	/*
	Copy constructor. Makes a deep copy of the input matrix.
	*/
	Matrix (const Matrix&);
	//end of constructors

	/*
	Overloads () to access an element [i,j] of the matrix. Start counting at 0.
	It is inlined to optimize performance.
	*/
	const double& operator() (std::vector<double>::size_type, std::vector<double>::size_type) const;
	double& operator() (std::vector<double>::size_type, std::vector<double>::size_type);
	
	/*
	Overloads * to multiply a matrix on the right by a vector.
	Returns a vector<double> of dimension equal to the number of rows of the matrix.
	If the dimension of the vector is different from the number of columns of the matrix,
	then it throws a ValueError.
	*/
	std::vector<double> operator* (std::vector<double>&);

	/*
	Returns the number of rows of the matrix.
	It is inlined to optimize performance.
	*/
	size_t rows() const {
		return const_cast<const size_t&>(_rows);
	}

	/*
	Returns the number of columns of the matrix.
	It is inlined to optimize performance.
	*/
	size_t columns() const {
		return const_cast<const size_t&>(_columns);
	};

	/*
	Returns true if the matrix is empty, false otherwise.
	It is inlined to optimize performance.
	*/
	bool empty() const { return _matrix.empty(); }


	//iterators
	/*
	Returns an iterator pointing to the beginning of the matrix.
	It is inlined to optimize performance.
	*/
	iterator begin() {
		return _matrix.begin();
	}

	/*
	Returns an iterator pointing to the element right after the end of the matrix.
	It is inlined to optimize performance.
	*/
	iterator end() {
		return _matrix.end();
	}

	/*
	Returns a const iterator pointing to the beginning of the matrix.
	It is inlined to optimize performance.
	*/
	const_iterator cbegin() const {
		return _matrix.cbegin();
	}

	/*
	Returns a const iterator pointing to the element right after the end of the matrix.
	It is inlined to optimize performance.
	*/
	const_iterator cend() const { 
		return _matrix.cend();
	}

	/*
	Returns an iterator pointing to the beginning of the row "row" of the matrix.
	It is inlined to optimize performance.
	*/
	iterator row_begin (std::vector<double>::size_type row) {
		return this->begin() + row * static_cast<std::vector<double>::size_type>(this->columns());
	}

	/*
	Returns an iterator pointing to the element right after the end of the row "row" of the matrix.
	It is inlined to optimize performance.
	*/
	iterator row_end (std::vector<double>::size_type row) {
		return this->begin() + (row + 1) * static_cast<std::vector<double>::size_type>(this->columns());
	}

	/*
	Returns a const iterator pointing to the beginning of the row "row" of the matrix.
	It is inlined to optimize performance.
	*/
	const_iterator row_cbegin (std::vector<double>::size_type row) const {
		return this->cbegin() + row * static_cast<std::vector<double>::size_type>(this->columns());
	}

	/*
	Returns a const iterator pointing to the element right after the end of the row "row" of the matrix.
	It is inlined to optimize performance.
	*/
	const_iterator row_cend (std::vector<double>::size_type row) const {
		return this->cbegin() + (row + 1) * static_cast<std::vector<double>::size_type>(this->columns());
	}

	/*
	Returns a pair with the iterators returned by row_begin and row_end. 
	Returning a pair avoids calling row_begin and row_end with different arguments and comparing them.
	It is inlined to optimize performance.
	*/
	std::pair<iterator,iterator> row_itr(std::vector<double>::size_type row) {
		return std::make_pair<iterator,iterator>(this->row_begin(row), this->row_end(row));
	}

	/*
	Returns a pair with the const_iterators returned by row_cbegin and row_cend.
	Returning a pair avoids calling row_begin and row_end with different arguments and comparing them.
	It is inlined to optimize performance.
	*/
	std::pair<const_iterator, const_iterator> row_citr(std::vector<double>::size_type row) {
		return std::make_pair<const_iterator, const_iterator>( this->row_cbegin(row), this->row_cend(row) );
	}


	//elementary row operations
	/*
	Exchanges the rows row1 and row2.
	Returns invalid_argument error if one of them does not exist, or *this if successful.
	*/
	Matrix exchangeRows (std::vector<double>::size_type, std::vector<double>::size_type);

	/*
	Multiplies a row by a double.
	Returns invalid_argument error if row does not exists, or *this if successful.
	*/
	Matrix multiplyRow(std::vector<double>::size_type, double);

	/*
	Sums (scalar * row1) to row2. It doesn't change the values at row1.
	Returns invalid_argument error if one of the rows doesn't exist, or *this if successful.
	*/
	Matrix linearCombination(double, std::vector<double>::size_type, std::vector<double>::size_type);
	
	// TODO: implement a swap function

private:
	/*
	Returns the total number of entries in _matrix.
	*/
	std::vector<double>::size_type size() const {
		return _matrix.size();
	}

	//data structures
	std::vector<double> _matrix;
	size_t _rows;
	size_t _columns;
	// TODO: maybe create another atribute to hold the number of digits of the largest entry.
	// This can make printing easier by padding. Also implement a function that receives the
	// matrix as argument and finds out the number of digits of the largest entry.
};

/*
Overloads << so we can print a matrix.
*/
std::ostream& operator<< (std::ostream&, const Matrix&);

#endif
