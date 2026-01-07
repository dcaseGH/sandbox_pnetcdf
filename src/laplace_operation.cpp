#include "laplace_operation.hpp"
#include <algorithm>
#include <vector>

// Perform a single Jacobi-style Laplace stencil update on a 2D grid.
// - `in` and `out` are arrays of size rows*cols in row-major order.
// - Interior points (1..rows-2, 1..cols-2) are updated as the average of
//   the four orthogonal neighbors: out[i,j] = 0.25*(in[i-1,j] + in[i+1,j]
//   + in[i,j-1] + in[i,j+1]).
// - Boundary values are copied from `in` to `out` unchanged.
// - If rows < 3 or cols < 3, the function will copy `in` to `out`.

void laplace_step(const double *in, double *out, std::size_t rows, std::size_t cols) {
    if (!in || !out || rows == 0 || cols == 0) return;

    // copy boundaries (top and bottom rows)
    for (std::size_t j = 0; j < cols; ++j) {
        out[0 * cols + j] = in[0 * cols + j];
        out[(rows - 1) * cols + j] = in[(rows - 1) * cols + j];
    }

    // copy left and right boundaries for the remaining rows
    for (std::size_t i = 1; i + 1 < rows; ++i) {
        out[i * cols + 0] = in[i * cols + 0];
        out[i * cols + (cols - 1)] = in[i * cols + (cols - 1)];
    }

    if (rows < 3 || cols < 3) {
        // nothing to stencil; boundaries already copied
        return;
    }

    // interior points: 5-point stencil (Jacobi average)
    for (std::size_t i = 1; i + 1 < rows; ++i) {
        for (std::size_t j = 1; j + 1 < cols; ++j) {
            double up    = in[(i - 1) * cols + j];
            double down  = in[(i + 1) * cols + j];
            double left  = in[i * cols + (j - 1)];
            double right = in[i * cols + (j + 1)];
            out[i * cols + j] = 0.25 * (up + down + left + right);
        }
    }
}
