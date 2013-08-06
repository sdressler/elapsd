IMPLICIT NONE

ENUM, BIND(C)
    ENUMERATOR ::   E_OK = 0,   & ! No error, success
&                   E_DUPK,     & ! Kernel already exists
&                   E_DUPD,     & ! Device already exists
&                   E_NOK,      & ! Kernel not found
&                   E_NOD,      & ! Device not found
&                   E_TMR,      & ! Timer error
&                   E_RES,      & ! Error in results
&                   E_EQU,      & ! Two ID's were equal
&                   E_PMAP,     & ! Parameter map error
&                   E_PRED        ! Prediction error
END ENUM
