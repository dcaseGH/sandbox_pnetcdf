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
