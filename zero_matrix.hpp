
#ifndef ZERO_MATRIX_HPP
#define ZERO_MATRIX_HPP

#include <vector>
#include <cstddef>
#include <algorithm>

// Simple fixed-size 2D matrix container that stores values in row-major
// order and initializes all elements to zero.
struct ZeroMatrix {
    // Number of rows in the matrix
    std::size_t rows;
    // Number of columns in the matrix
    std::size_t cols;
    // Flat storage holding rows * cols elements in row-major order
    std::vector<double> data;

    // Construct a rows-by-cols matrix with all elements initialized to 0.0.
    // If either dimension is zero, the data vector will be empty.
    ZeroMatrix(std::size_t r = 0, std::size_t c = 0)
        : rows(r), cols(c), data(r * c, 0.0) {}

    // Mutable element access (row i, column j). No bounds checking performed.
    double &operator()(std::size_t i, std::size_t j) {
        return data[i * cols + j];
    }

    // Const element access (row i, column j). No bounds checking performed.
    const double &operator()(std::size_t i, std::size_t j) const {
        return data[i * cols + j];
    }

    // Set all elements to zero.
    void fill_zero() {
        std::fill(data.begin(), data.end(), 0.0);
    }

    //destructor
    ~ZeroMatrix() = default;
};

#endif // ZERO_MATRIX_HPP
