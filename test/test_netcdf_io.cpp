#include <mpi.h>
#include <vector>
#include <iostream>
#include "netcdf_io.hpp"
#include <netcdf.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank = 0, size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const std::size_t rows = 2;
    const std::size_t cols = 3;
    std::vector<double> data(rows * cols);

    // fill local matrix with identifiable values
    for (std::size_t i = 0; i < rows; ++i)
        for (std::size_t j = 0; j < cols; ++j)
            data[i * cols + j] = rank * 1000 + static_cast<double>(i * cols + j);

    const std::string fname = "test_output.nc";
    int rc = write_matrix_netcdf(fname, data.data(), rows, cols, MPI_COMM_WORLD);
    if (rc != 0) {
        if (rank == 0) std::cerr << "write_matrix_netcdf failed: " << rc << std::endl;
        MPI_Finalize();
        return 1;
    }

    // only rank 0 verifies the file was created and has expected dims
    if (rank == 0) {
        int ncid = -1;
        int retval = nc_open(fname.c_str(), NC_NOWRITE, &ncid);
        if (retval != NC_NOERR) {
            std::cerr << "nc_open failed: " << nc_strerror(retval) << std::endl;
            MPI_Finalize();
            return 1;
        }
        // verify dimensions
        int dimid_nprocs, dimid_rows, dimid_cols;
        size_t nprocs_len, rows_len, cols_len;
        retval = nc_inq_dimid(ncid, "nprocs", &dimid_nprocs);
        if (retval != NC_NOERR) { std::cerr << "missing nprocs dim" << std::endl; nc_close(ncid); MPI_Finalize(); return 1; }
        retval = nc_inq_dimlen(ncid, dimid_nprocs, &nprocs_len);
        retval = nc_inq_dimid(ncid, "rows", &dimid_rows);
        retval = nc_inq_dimlen(ncid, dimid_rows, &rows_len);
        retval = nc_inq_dimid(ncid, "cols", &dimid_cols);
        retval = nc_inq_dimlen(ncid, dimid_cols, &cols_len);
        if (nprocs_len != static_cast<size_t>(size) || rows_len != rows || cols_len != cols) {
            std::cerr << "dimension mismatch in output file" << std::endl;
            nc_close(ncid);
            MPI_Finalize();
            return 1;
        }
        nc_close(ncid);
    }

    MPI_Finalize();
    return 0;
}
