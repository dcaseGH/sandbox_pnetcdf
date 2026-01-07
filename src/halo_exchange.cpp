#include "halo_exchange.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

// Exchange halo rows with neighboring ranks:
// - each rank sends its top row to the previous rank and receives the
//   previous rank's bottom row into its top row position
// - each rank sends its bottom row to the next rank and receives the
//   next rank's top row into its bottom row position
// Matrices are assumed stored in row-major order as a contiguous array
// with dimensions (rows x cols). No bounds checking is performed.

void halo_exchange(double *data, std::size_t rows, std::size_t cols, MPI_Comm comm) {
    if (!data || rows == 0 || cols == 0) return;

    int rank = 0, size = 1;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int prev = (rank == 0) ? MPI_PROC_NULL : rank - 1;
    int next = (rank == size - 1) ? MPI_PROC_NULL : rank + 1;

    std::vector<double> send_top(cols), send_bottom(cols);
    std::vector<double> recv_top(cols), recv_bottom(cols);

    // copy current top and bottom rows into send buffers
    for (std::size_t j = 0; j < cols; ++j) {
        send_top[j] = data[0 * cols + j];
        send_bottom[j] = data[(rows - 1) * cols + j];
    }

    // Exchange with previous rank: send our top row, receive prev's bottom row
    MPI_Sendrecv(send_top.data(), static_cast<int>(cols), MPI_DOUBLE, prev, 0,
                 recv_top.data(), static_cast<int>(cols), MPI_DOUBLE, prev, 0,
                 comm, MPI_STATUS_IGNORE);

    // Exchange with next rank: send our bottom row, receive next's top row
    MPI_Sendrecv(send_bottom.data(), static_cast<int>(cols), MPI_DOUBLE, next, 1,
                 recv_bottom.data(), static_cast<int>(cols), MPI_DOUBLE, next, 1,
                 comm, MPI_STATUS_IGNORE);

    // Overwrite top row with received data from prev (if any)
    if (prev != MPI_PROC_NULL) {
        for (std::size_t j = 0; j < cols; ++j)
            data[0 * cols + j] = recv_top[j];
    }

    // Overwrite bottom row with received data from next (if any)
    if (next != MPI_PROC_NULL) {
        for (std::size_t j = 0; j < cols; ++j)
            data[(rows - 1) * cols + j] = recv_bottom[j];
    }
}
