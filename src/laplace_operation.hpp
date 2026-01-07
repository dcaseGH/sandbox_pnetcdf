#ifndef LAPLACE_OPERATION_HPP
#define LAPLACE_OPERATION_HPP

#include <cstddef>

// Perform a single Jacobi-style Laplace stencil update on a 2D grid.
// - `in` and `out` are arrays of size rows*cols in row-major order.
// - Interior points are updated using the 5-point stencil average.
// - Boundary values are copied from `in` to `out` unchanged.
void laplace_step(const double* in, double* out, std::size_t rows, std::size_t cols);

#endif // LAPLACE_OPERATION_HPP
