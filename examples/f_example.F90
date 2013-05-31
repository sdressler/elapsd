!
! The fibonacci number generator for elapsd
!
program fibonacci

      !
      ! This is a FORTRAN test for elapsd
      !


      ! Use the elapsd module
      USE m_elapsd
      USE, INTRINSIC :: ISO_C_BINDING

      implicit none                     ! No implicit declarations

      ! Interface for the fibonacci function
      interface
          function fib(n)               ! Fibonacci function
              integer, intent(in) :: n  ! Input number
              integer :: fib            ! Fibonacci number
          end function fib
      end interface

      ! Integer for count
      TYPE(C_PTR) :: elapsd
      integer :: i, err, fNum,f
      character*256 :: s

      ! Call some C
      elapsd = elapsdInit("elapsd.db" // C_NULL_CHAR , &
      &                 "Fibonacci FORTRAN" // C_NULL_CHAR)

      ! Add kernels
      print '(a)', "Adding kernel."
      err = elapsdAddKernel(elapsd, 1_C_INT, "Fibonacci" // C_NULL_CHAR )

      if ( err /= E_OK ) then
         print '(a,a)', "Error", err
         stop 'Error in elapsdAddKernel'
      end if

      ! Add device
      print '(a)', "Adding device"
      err = elapsdAddDevice(elapsd, 0, "CPU" // C_NULL_CHAR );

      ! Generate some numbers
      print '(a)', "Generating timings and datavolumes"
      do i = 1,4
! Fib number to generate
        f = i  * 10
! Start Timer      
        err = elapsdStartTimer(elapsd, 1, 0)
! Run Kernel
        fNum = fib(f)  
! Stop Timer
        err = elapsdStopTimer(elapsd, 1, 0)
! Set KD    
        err = elapsdAddKernelDataVolumes(elapsd, 1, 0, 8_8, 8_8)

        write(*,'(A,I2,A,I12)')'Fibonacci',f,': ',fNum
      end do

      print '(a)', "Printing content:"
      call elapsdPrintResults(elapsd)
      print '(a)', "Commiting to DB:"
      call elapsdCommitToDB(elapsd)
      stop
end program fibonacci

!
! Now we're defining the fibonacci function itself
!
recursive function fib (n) result (fNum)
    integer, intent(in) :: n
    integer :: fNum

    if (n < 2) then
        fNum = n
    else
        fNum = fib(n - 1) + fib(n - 2)
    endif
end function fib

