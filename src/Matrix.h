#ifndef GUARD_Matrix_h
#define GUARD_Matrix_h

#include <stdexcept>
#include <vector>

class Matrix {
public:
	Matrix(const std::vector< std::vector<double> > &matrix) {
		if (matrix.empty()) {
			throw std::invalid_argument("Matrix: Cannot create an empty matrix.");
		}

		//used to check if the input matrix has all lines of same size
		std::vector< std::vector<double> >::size_type length = matrix[0].size();
		
		//copies the entries of the input into the private data structure
		for (auto &line : matrix) {
			if (line.size() != length) {
				_matrix = std::vector<double>();
				throw std::invalid_argument("Matrix: All the lines in a matrix must have the same length.");
			}
			for (auto &entry : line) {
				_matrix.push_back(entry);
			}
		}
	};
private:
	//data structures
	std::vector<double> _matrix;
};

#endif