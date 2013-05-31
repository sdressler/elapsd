MODULE m_elapsd
!
! #include "../include/elapsd/celapsd.h"
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

    INTERFACE
        FUNCTION elapsdInit(dbFile, expName) &
&           BIND(C, NAME="elapsdInit")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_CHAR
            IMPLICIT NONE
            CHARACTER(LEN=1, KIND=C_CHAR), DIMENSION(*), INTENT(IN) :: dbFile
            CHARACTER(LEN=1, KIND=C_CHAR), DIMENSION(*), INTENT(IN) :: expName
            TYPE(C_PTR) :: elapsdInit
        END FUNCTION elapsdInit
       
        FUNCTION elapsdInitKernelConf() &
&           BIND(C, NAME="elapsdInitKernelConf")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_CHAR
            IMPLICIT NONE
            TYPE(C_PTR) :: elapsdInitKernelConf
        END FUNCTION elapsdInitKernelConf

        FUNCTION elapsdAddKernel(e, ID, kName) &
&           BIND(C, NAME="elapsdAddKernel")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT, C_CHAR
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: e
            INTEGER(C_INT), VALUE :: ID
            CHARACTER(LEN=1, KIND=C_CHAR), DIMENSION(*), INTENT(IN) :: kName
            INTEGER(C_INT) :: elapsdAddKernel
        END FUNCTION elapsdAddKernel
        
        FUNCTION elapsdAddDevice(e, ID, dName) &
&           BIND(C, NAME="elapsdAddDevice")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT, C_CHAR
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: e
            INTEGER(C_INT), VALUE :: ID
            CHARACTER(LEN=1, KIND=C_CHAR), DIMENSION(*), INTENT(IN) :: dName
            INTEGER(C_INT) :: elapsdAddDevice
        END FUNCTION elapsdAddDevice

        FUNCTION elapsdAddSubDeviceToDevice(e, ID, sID) &
&           BIND(C, NAME="elapsdAddSubDeviceToDevice")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: e
            INTEGER(C_INT), VALUE :: ID
            INTEGER(C_INT), VALUE :: sID
            INTEGER(C_INT) :: elapsdAddSubDeviceToDevice
        END FUNCTION elapsdAddSubDeviceToDevice

        FUNCTION elapsdStartTimer(e, KernelID, DeviceID) &
&           BIND(C, NAME="elapsdStartTimer")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: e
            INTEGER(C_INT), VALUE :: KernelID
            INTEGER(C_INT), VALUE :: DeviceID
            INTEGER(C_INT) :: elapsdStartTimer
        END FUNCTION elapsdStartTimer
        
        FUNCTION elapsdStopTimer(e, KernelID, DeviceID)&
&           BIND(C, NAME="elapsdStopTimer")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: e
            INTEGER(C_INT), VALUE :: KernelID
            INTEGER(C_INT), VALUE :: DeviceID
            INTEGER(C_INT) :: elapsdStopTimer
        END FUNCTION elapsdStopTimer

        FUNCTION elapsdAddKernelDataVolumes(e, KernelID, DeviceID, inBytes, outBytes) &
&           BIND(C, NAME="elapsdAddKernelDataVolumes")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT, C_LONG_LONG
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: e
            INTEGER(C_INT), VALUE :: KernelID
            INTEGER(C_INT), VALUE :: DeviceID
            INTEGER(C_LONG_LONG), VALUE :: inBytes
            INTEGER(C_LONG_LONG), VALUE :: outBytes
            INTEGER(C_INT) :: elapsdAddKernelDataVolumes
        END FUNCTION elapsdAddKernelDataVolumes

        FUNCTION elapsdInsertKernelConfPair(c, cKey, cValue) &
&           BIND(C, NAME="elapsdInsertKernelConfPair")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR, C_INT, C_CHAR
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: c
            CHARACTER(LEN=1, KIND=C_CHAR), DIMENSION(*), INTENT(IN) :: cKey
            CHARACTER(LEN=1, KIND=C_CHAR), DIMENSION(*), INTENT(IN) :: cValue
            INTEGER(C_INT) :: elapsdInsertKernelConfPair
        END FUNCTION elapsdInsertKernelConfPair

        SUBROUTINE elapsdCommitToDB(e) &
&           BIND(C, NAME="elapsdCommitToDB")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: e
        END SUBROUTINE elapsdCommitToDB

        SUBROUTINE elapsdPrintResults(e) &
&           BIND(C, NAME="elapsdPrintResults")

            USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR
            IMPLICIT NONE
            TYPE(C_PTR), VALUE :: e
        END SUBROUTINE elapsdPrintResults
    END INTERFACE

END MODULE m_elapsd
