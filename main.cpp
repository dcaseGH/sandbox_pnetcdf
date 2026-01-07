#include <mpi.h>
#include <netcdf.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::cout << "NetCDF Version: " << nc_inq_libvers() << std::endl;
    }

    // Create matrix of zeros
    

    MPI_Finalize();
    return 0;
}