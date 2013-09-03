!
! The fibonacci number generator for elapsd
!
program fibonacci

      !
      ! This is a FORTRAN test for elapsd
      !


      ! Use the elapsd module
      USE m_elapsd
      use m_elapsd_params
      use m_elapsd_predict
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
      TYPE(C_PTR) :: elapsd,elapsd_params,p
      integer :: i, err, fNum,f,pred_err
      character*256 :: s
      real(8) :: FIB_time,FIB_time_pred
print *,'Get Fibonacci Number argument'
      do i=1,len(s)
        s(i:i)=' '
      end do
      call getarg(1,s)
      read(s,'(I5)')f
      print *,'Wants to calculate Fibonacci Number for:',f
print '(a)',"elapsdParamsInit"      
      elapsd_params=elapsdParamsInit()
print '(a)',"elapsdParamsAddParam" 
      err=elapsdParamsAddParam(elapsd_params, 'FIB' // C_NULL_CHAR, f)
      if ( err /= E_OK ) then
         print '(a,a)', "Error", err
         stop 'Error in elapsdParamsAddParam'
      end if      
print '(a)', "elapsdInit: elapsd.db"
      elapsd = elapsdInit("elapsd_predict_F.db" // C_NULL_CHAR , &
      &                 "Fibonacci FORTRAN" // C_NULL_CHAR, elapsd_params)

print '(a)', "Adding kernel."
      err = elapsdAddKernel(elapsd, 0_C_INT, "Fibonacci" // C_NULL_CHAR )
      if ( err /= E_OK ) then
         print '(a,a)', "Error", err
         stop 'Error in elapsdAddKernel'
      end if
print '(a)', "Adding device"
      err = elapsdAddDevice(elapsd, 0_C_INT, "CPU" // C_NULL_CHAR )
      if ( err /= E_OK ) then
         print '(a,a)', "Error", err
         stop 'Error in elapsdAddDevice'
      end if      
print '(a)', "Initialize runtime predictor for Kernel = 0 and Device = 0"
      p = predictInit("elapsd_predict_F.db", elapsd_params, 0_C_INT, 0_C_INT)
print '(a)', "TRY to initialize prediction, if no success, continue collecting&
&             data"
      pred_err = predictCreatePredictionModel_Lagrange(p)     
print '(a)', "Make a prediction, if pred_err == E_OK"  
      FIB_time_pred = -1.0;
      if (pred_err == E_OK)then
        FIB_time_pred = predictGetRuntimePrediction(p, f)
      end if
! Generate some numbers
print '(a)', "Generating timings and datavolumes"

! Start Timer      
        err = elapsdStartTimer(elapsd, 0_C_INT, 0_C_INT)
! Run Kernel
        fNum = fib(f)  
! Stop Timer
        err = elapsdStopTimer(elapsd, 0_C_INT, 0_C_INT)
        write(*,'(A,I2,A,I12)')'Fibonacci for ',f,' = ',fNum

        FIB_time = elapsdGetLastWallTime(elapsd, 0_C_INT, 0_C_INT)
        print *,"Predicted runtime : ", FIB_time_pred
        print *,"Real runtime      : ", FIB_time
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

