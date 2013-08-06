MODULE m_elapsd_params

INCLUDE "Felapsd_errors.F90"
!
! #include "../../include/elapsd/cparams.h"
!
    IMPLICIT NONE

    ENUM, BIND(C)
        ENUMERATOR ::   E_OK = 0,   & ! No error, success
&                       E_DUPK,     & ! Kernel already exists
&                       E_DUPD,     & ! Device already exists
&                       E_NOK,      & ! Kernel not found
&                       E_NOD,      & ! Device not found
&                       E_TMR,      & ! Timer error
&                       E_RES,      & ! Error in results
&                       E_EQU         ! Two ID's were equal
    END ENUM

!    elapsdParams* elapsdParamsInit();

!    int elapsdParamsAddParam(elapsdParams *p, const char *key, int value);

    INTERFACE
        FUNCTION elapsdParamsInit() &
&           BIND(C, NAME="elapsdParamsInit")
        
            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR
            IMPLICIT NONE
            TYPE(C_PTR) :: elapsdParamsInit
        END FUNCTION elapsdParamsInit

        FUNCTION elapsdParamsAddParam(p, key, val) &
&           BIND(C, NAME="elapsdParamsAddParam")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_CHAR, C_INT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: p
            CHARACTER(LEN=1, KIND=C_CHAR), DIMENSION(*), INTENT(IN) :: key
            INTEGER(C_INT), VALUE :: val
            INTEGER(C_INT) :: elapsdParamsAddParam
        END FUNCTION elapsdParamsAddParam
    END INTERFACE

END MODULE m_elapsd_params
