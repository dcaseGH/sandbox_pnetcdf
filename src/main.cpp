#include <mpi.h>
#include <netcdf.h>
#include <iostream>
#include "zero_matrix.hpp"
#include "laplace_operation.hpp"
#include "halo_exchange.hpp"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::cout << "NetCDF Version: " << nc_inq_libvers() << std::endl;
    }

    // Create matrix of zeros within a struct by calling ZeroMatrix
    ZeroMatrix matrix(3, 4); // 3 rows, 4 columns

    for (std::size_t i = 0; i < matrix.rows; ++i) {
        for (std::size_t j = 0; j < matrix.cols; ++j) {
            matrix(i,j) = static_cast<double> ( (i * matrix.cols + j) * rank);
        }
    }

    // halo exchange called from halo_exchange.cpp

    for (int it = 0; it < 10; ++it) {

        //apply laplace operation
        ZeroMatrix temp(matrix.rows, matrix.cols);
        laplace_step(matrix.data.data(), temp.data.data(), matrix.rows, matrix.cols);
        matrix = temp;

        //halo exchange
        halo_exchange(matrix.data.data(), matrix.rows, matrix.cols, MPI_COMM_WORLD);
    }    

    MPI_Finalize();
    return 0;
}
