#ifndef HALO_EXCHANGE_HPP
#define HALO_EXCHANGE_HPP

#include <cstddef>
#include <mpi.h>

// Exchange halo rows with neighboring ranks.
// - `data` points to a rows-by-cols array in row-major order.
// - Sends top row to previous rank and receives previous rank's bottom row
//   into our top row. Sends bottom row to next rank and receives next
//   rank's top row into our bottom row.
// - Uses blocking `MPI_Sendrecv` and treats MPI_PROC_NULL for boundaries.
// - No bounds checking is performed; rows and cols must match the layout.
void halo_exchange(double* data, std::size_t rows, std::size_t cols, MPI_Comm comm);

#endif // HALO_EXCHANGE_HPP
