#include "netcdf_io.hpp"
#include <netcdf.h>
#include <mpi.h>
#include <vector>
#include <iostream>

int write_matrix_netcdf(const std::string &filename, const double *data, std::size_t rows, std::size_t cols, MPI_Comm comm) {
    if (!data || rows == 0 || cols == 0) return -1;

    int rank = 0, size = 1;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    const std::size_t elems = rows * cols;
    // gather all local matrices to rank 0
    std::vector<double> gathered;
    if (rank == 0) gathered.resize(elems * static_cast<std::size_t>(size));

    // MPI_Gather requires int counts
    int sendcount = static_cast<int>(elems);
    int recvcount = sendcount;
    int mpi_err = MPI_Gather(const_cast<double*>(data), sendcount, MPI_DOUBLE,
                             rank == 0 ? gathered.data() : nullptr, recvcount, MPI_DOUBLE,
                             0, comm);
    if (mpi_err != MPI_SUCCESS) {
        if (rank == 0) std::cerr << "MPI_Gather failed with code " << mpi_err << std::endl;
        return mpi_err;
    }

    if (rank == 0) {
        int ncid = -1;
        int retval = nc_create(filename.c_str(), NC_CLOBBER, &ncid);
        if (retval != NC_NOERR) {
            std::cerr << "nc_create failed: " << nc_strerror(retval) << std::endl;
            return retval;
        }

        int dim_proc, dim_rows, dim_cols;
        retval = nc_def_dim(ncid, "nprocs", static_cast<size_t>(size), &dim_proc);
        if (retval != NC_NOERR) { std::cerr << nc_strerror(retval) << std::endl; nc_close(ncid); return retval; }
        retval = nc_def_dim(ncid, "rows", rows, &dim_rows);
        if (retval != NC_NOERR) { std::cerr << nc_strerror(retval) << std::endl; nc_close(ncid); return retval; }
        retval = nc_def_dim(ncid, "cols", cols, &dim_cols);
        if (retval != NC_NOERR) { std::cerr << nc_strerror(retval) << std::endl; nc_close(ncid); return retval; }

        int dims[3] = {dim_proc, dim_rows, dim_cols};
        int varid = -1;
        retval = nc_def_var(ncid, "matrix", NC_DOUBLE, 3, dims, &varid);
        if (retval != NC_NOERR) { std::cerr << nc_strerror(retval) << std::endl; nc_close(ncid); return retval; }

        retval = nc_enddef(ncid);
        if (retval != NC_NOERR) { std::cerr << nc_strerror(retval) << std::endl; nc_close(ncid); return retval; }

        // write gathered data (layout: rank0 block, rank1 block, ...)
        retval = nc_put_var_double(ncid, varid, gathered.data());
        if (retval != NC_NOERR) { std::cerr << nc_strerror(retval) << std::endl; nc_close(ncid); return retval; }

        retval = nc_close(ncid);
        if (retval != NC_NOERR) { std::cerr << nc_strerror(retval) << std::endl; return retval; }
    }

    return 0;
}
