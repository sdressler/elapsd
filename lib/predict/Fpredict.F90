MODULE m_elapsd_predict
!
! #include "../../include/elapsd/predict/cpredict.h"
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
&                       E_EQU,      & ! Two ID's were equal
&                       E_PMAP,     & ! Parameter map error
&                       E_LAGR        ! Lagrange polynomial error
    END ENUM

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
    END INTERFACE

END MODULE m_elapsd_predict
