MODULE m_elapsd_params

include "Felapsd_errors.F90"

!
! #include "../../include/elapsd/cparams.h"
!

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
