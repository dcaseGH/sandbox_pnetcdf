cmake_minimum_required(VERSION 3.15)
project(ParallelProject LANGUAGES CXX Fortran)

# 1. Find dependencies
find_package(MPI REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(NETCDF REQUIRED IMPORTED_TARGET netcdf)
pkg_check_modules(NETCDFF REQUIRED IMPORTED_TARGET netcdf-fortran)

# 2. CREATE the targets (Executables) first
add_executable(mpi_netcdf_cpp main.cpp)
add_executable(mpi_netcdf_fortran main.f90)

# 3. NOW specify the link libraries
# For C++
target_link_libraries(mpi_netcdf_cpp PRIVATE 
    MPI::MPI_CXX 
    PkgConfig::NETCDF
)

# For Fortran
target_link_libraries(mpi_netcdf_fortran PRIVATE 
    MPI::MPI_Fortran 
    PkgConfig::NETCDFF
)

# Enable tests and add a small unit test for ZeroMatrix
enable_testing()

add_executable(test_zero_matrix test/test_zero_matrix.cpp)
target_include_directories(test_zero_matrix PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME zero_matrix_test COMMAND test_zero_matrix)
