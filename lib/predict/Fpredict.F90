MODULE m_elapsd_predict
!
! #include "../../include/elapsd/predict/cpredict.h"
!

INCLUDE "Felapsd_errors.F90"

    INTERFACE
        FUNCTION predictInit(db, params, KernelID, DeviceID) &
&           BIND(C, NAME="predictInit")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: db
            TYPE(C_PTR), VALUE :: params
            INTEGER(C_INT), VALUE :: KernelID
            INTEGER(C_INT), VALUE :: DeviceID
            TYPE(C_PTR) :: predictInit
        END FUNCTION predictInit

        FUNCTION predictGetNumberOfDistinctMeasurements(p) &
&           BIND(C, NAME="predictGetNumberOfDistinctMeasurements")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: p
            INTEGER(C_INT) :: predictGetNumberOfDistinctMeasurements
        END FUNCTION predictGetNumberOfDistinctMeasurements

        FUNCTION predictGenerateLagrangePolynomial(p) &
&           BIND(C, NAME="predictGenerateLagrangePolynomial")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: p
            INTEGER(C_INT) :: predictGenerateLagrangePolynomial
        END FUNCTION predictGenerateLagrangePolynomial

        FUNCTION  predictMakeRuntimePrediction(p, N) &
&           BIND(C, NAME="predictMakeRuntimePrediction")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT, C_DOUBLE
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: p
            INTEGER(C_INT), VALUE :: N
            REAL(C_DOUBLE) :: predictMakeRuntimePrediction
        END FUNCTION predictMakeRuntimePrediction
    END INTERFACE

END MODULE m_elapsd_predict
