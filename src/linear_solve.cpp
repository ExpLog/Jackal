#include <stdexcept>    //used std::invalid_argument
#include <tuple>      //used std::tie
#include <vector>
#include "decomposition.h"
#include "linear_solve.h"
#include "matrix.h"


namespace {
    /*
    Auxiliary function to solve linear systems when the coefficient matrix is lower-triangular.
    */
    static std::vector<double> linear_solve_lower(const Matrix& L, const std::vector<double> b){
        const size_t length = b.size();
        std::vector<double> y(length);
        // The values of y are calculated from top to bottom.
        // The variables row_index and column_index are the indices from the matrix' point of view.
        for (long row_index = 0; row_index < static_cast<long>(length); ++row_index) {
            y[row_index] = b[row_index];
            for (long column_index = 0; column_index < row_index; ++column_index){
                y[row_index] -= L(row_index, column_index) * y[column_index];
            }
            y[row_index] /= L(row_index, row_index); // TODO: should have a special throw for zero division?
        }
        return y;
    }

    /*
    Auxiliary function to solve linear systems when the coefficient matrix is upper-triangular.
    */
    static std::vector<double> linear_solve_upper(const Matrix& U, const std::vector<double> b){
        const size_t length = b.size();
        std::vector<double> x(length);
        // The values of x are calculated from bottom to top.
        // The variables row_index and column_index are the indices from the matrix' point of view.
        for (long row_index = static_cast<long>(length) - 1; row_index >= 0; --row_index) {
            x[row_index] = b[row_index];
            for (long column_index = static_cast<long>(length) - 1; column_index > row_index; --column_index){
                x[row_index] -= U(row_index, column_index) * x[column_index];
            }
            x[row_index] /= U(row_index, row_index); // TODO: should have a special throw for zero division?
        }
        return x;
    }
} // namespace


// Check for throwable conditions in linear_solve.


/*
Solves a linear system Ax = b by decomposing A = LU, then solving Ly = b, followed by Ux = y.
Returns b.
Throws std::invalid_argument if the number of rows of A is different from the length of b, if the system
is empty or under or overdetermined.
Throws std::domain_error if the system is unsolvable.
*/
std::vector<double> linear_solve(Matrix& A, const std::vector<double> b){
    if (A.rows() != b.size()){
        throw std::invalid_argument("linear_solve: the matrix' number of rows must be equal to the length of the vector.");
    }
    try {
        std::pair<Matrix, Matrix> lu_pair = lu_decomp(A);
        std::vector<double> y = linear_solve_lower(lu_pair.first, b);
        return linear_solve_upper(lu_pair.second, y);
    } catch (std::invalid_argument){
        throw std::invalid_argument("linear_solve: the system is over- or underdetermined, or is empty.");
    } catch (std::domain_error){
        throw std::domain_error("linear_solve: the system is unsolvable (the rows are not linear independent or the system has no solution).");
    }
    
}
