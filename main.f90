program test_mpi_netcdf
    use mpi
    use netcdf
    implicit none
    integer :: rank, ierr, ncid

    call MPI_INIT(ierr)
    call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierr)

    if (rank == 0) then
        print *, "NetCDF Fortran version: ", nf90_inq_libvers()
    end if

    call MPI_FINALIZE(ierr)
end program