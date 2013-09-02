MODULE m_elapsd_predict
!
! #include "../../include/elapsd/predict/cpredict.h"
!
!#INCLUDE "Felapsd_errors.F90"

    INTERFACE
        FUNCTION predictInit(db, params, KernelID, DeviceID) &
&           BIND(C, NAME="predictInit")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT, C_CHAR
            IMPLICIT NONE
            CHARACTER(LEN=1, KIND=C_CHAR), DIMENSION(*), INTENT(IN) :: db
            TYPE(C_PTR), VALUE :: params
            INTEGER(C_INT), VALUE :: KernelID
            INTEGER(C_INT), VALUE :: DeviceID
            TYPE(C_PTR) :: predictInit
        END FUNCTION predictInit

        FUNCTION predictCreatePredictionModel_Lagrange(p) &
&           BIND(C, NAME="predictCreatePredictionModel_Lagrange")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: p
            INTEGER(C_INT) :: predictCreatePredictionModel_Lagrange
        END FUNCTION predictCreatePredictionModel_Lagrange

        FUNCTION  predictGetRuntimePrediction(p, N) &
&           BIND(C, NAME="predictGetRuntimePrediction")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT, C_FLOAT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: p
            INTEGER(C_INT), VALUE :: N
            REAL(C_FLOAT) :: predictGetRuntimePrediction
        END FUNCTION predictGetRuntimePrediction
    END INTERFACE

END MODULE m_elapsd_predict
