#include <mpi.h>
#include <vector>
#include <iostream>
#include <cstddef>
#include "halo_exchange.hpp"


//extern void halo_exchange(double* data, std::size_t rows, std::size_t cols, MPI_Comm comm);

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank = 0, size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const std::size_t rows = 4;
    const std::size_t cols = 3;
    std::vector<double> data(rows * cols, -1.0);

    // Initialize top and bottom rows with identifiable values
    for (std::size_t j = 0; j < cols; ++j) {
        data[0 * cols + j] = rank * 100 + static_cast<double>(j);
        data[(rows - 1) * cols + j] = rank * 1000 + static_cast<double>(j);
    }

    // perform halo exchange
    halo_exchange(data.data(), rows, cols, MPI_COMM_WORLD);

    int prev = (rank == 0) ? MPI_PROC_NULL : rank - 1;
    int next = (rank == size - 1) ? MPI_PROC_NULL : rank + 1;

    bool ok = true;

    // Check top row: should equal prev's bottom row if prev exists
    for (std::size_t j = 0; j < cols; ++j) {
        double expected;
        if (prev != MPI_PROC_NULL) expected = prev * 1000 + static_cast<double>(j);
        else expected = rank * 100 + static_cast<double>(j);
        double actual = data[0 * cols + j];
        if (actual != expected) {
            std::cerr << "Rank " << rank << ": top row mismatch at j=" << j
                      << " expected=" << expected << " got=" << actual << "\n";
            ok = false;
        }
    }

    // Check bottom row: should equal next's top row if next exists
    for (std::size_t j = 0; j < cols; ++j) {
        double expected;
        if (next != MPI_PROC_NULL) expected = next * 100 + static_cast<double>(j);
        else expected = rank * 1000 + static_cast<double>(j);
        double actual = data[(rows - 1) * cols + j];
        if (actual != expected) {
            std::cerr << "Rank " << rank << ": bottom row mismatch at j=" << j
                      << " expected=" << expected << " got=" << actual << "\n";
            ok = false;
        }
    }

    MPI_Finalize();
    return ok ? 0 : 1;
}
