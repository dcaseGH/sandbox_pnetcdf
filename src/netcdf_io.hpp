#pragma once

#include <string>
#include <cstddef>
#include <mpi.h>

// Writes a matrix from each rank into a single NetCDF file.
// On entry, `data` points to a local matrix of size `rows x cols`.
// This routine gathers all ranks' matrices to rank 0 and writes a 3D
// variable of shape (nprocs, rows, cols). Returns 0 on success or
// a non-zero netCDF error code on failure (on rank 0) or MPI error code on other ranks.
int write_matrix_netcdf(const std::string &filename, const double *data, std::size_t rows, std::size_t cols, MPI_Comm comm);
