/**
 *
 * \mainpage test
 *
 * C wrapper for elapsd (which is written in C++).
 *
 * @author	Sebsatian Dressler
 * @date	2012-05-25
 *
 * */

#ifndef C_ELAPSD_H
#define C_ELAPSD_H

#include "Errors.h"
#include "elapsd.h"
#include "Params.h"

#ifdef __cplusplus

using namespace ENHANCE;

extern "C" {
#endif

/**
 *
 * Initialises the elapsd object.
 *
 * @retval elapsd Pointer to a valid elapsd object or NULL if something went wrong
 *
 * */
elapsd* elapsdInit(
    const char *dbName,
    const char *expName,
    const elapsdParams *p
);
elapsd* cpp_callback_elapsdInit(
    const char *dbName,
    const char *expName,
    const elapsdParams *p
);

/**
 * Adds a new kernel to the framework. The kernel is only added if the ID does
 * not yet exist.
 *
 * @param[in] e Pointer to the instantiated elapsd object
 * @param[in] ID The unique ID of the kernel
 * @param[in] kName The (optional) name of the kernel
 * @retval #ERR E_OK for success, E_DUPK if the ID already exists.
 * */
int elapsdAddKernel(elapsd *e, int ID, const char *kName);
int cpp_callback_elapsdAddKernel(elapsd *e, int ID, const char *kName);

/**
 * Adds a new device to the framework.
 *
 * @param[in] e Pointer to the instantiated elapsd object
 * @param[in] ID The unique ID of the device
 * @param[in] dName The (optional) name of the device
 * @retval #ERR E_OK for success, E_DUPD if the ID already exists.
 * */
int elapsdAddDevice(elapsd *e, int ID, const char *dName);
int cpp_callback_elapsdAddDevice(elapsd *e, int ID, const char *dName);

/**
 * Adds a subdevice to an existing device.
 *
 * @param[in] e Pointer to the instantiated elapsd object
 * @param[in] ID The unique ID of the device where the subdevice shall be added
 * @param[in] sID The unique ID of the subdevice to be added
 * @retval #ERR
 * 			E_OK for success,
 * 			E_NOD if one of the devices does not exist,
 * 			E_EQU if both ID's are the same
 * */
int elapsdAddSubDeviceToDevice(elapsd *e, const int ID, const int sID);
int cpp_callback_elapsdAddSubDeviceToDevice(elapsd *e, const int ID, const int sID);

/**
 * Starts the time measurement for a specific kernel / device combination
 *
 * @param[in] e Pointer to the instantiated elapsd object
 * @param[in] KernelID The ID of the kernel
 * @param[in] DeviceID The ID of the device
 * @retval #ERR
 * 		E_OK for success,
 * 		E_NOK if the kernel was not found,
 * 		E_NOD if the device was not found
 * */
int elapsdStartTimer(elapsd *e, int KernelID, int DeviceID);
int cpp_callback_elapsdStartTimer(elapsd *e, int KernelID, int DeviceID);

/**
 * Stops the time measurement for a specific kernel / device combination
 *
 * @param[in] e Pointer to the instantiated elapsd object
 * @param[in] KernelID The ID of the kernel
 * @param[in] DeviceID The ID of the device
 * @retval #ERR
 * 		E_OK for success,
 * 		E_NOK if the kernel was not found,
 * 		E_NOD if the device was not found,
 * 		E_TMR if the start time was not found,
 * 		E_RES if the result is wrong (i.e. time difference negative)
 * */
int elapsdStopTimer(elapsd *e, int KernelID, int DeviceID);
int cpp_callback_elapsdStopTimer(elapsd *e, int KernelID, int DeviceID);

/**
 * Registers the data to be transferred for a specific kernel / device combination
 *
 * @param[in] e Pointer to the instantiated elapsd object
 * @param[in] KernelID The ID of the kernel
 * @param[in] DeviceID The ID of the device
 * @param[in] inBytes  The number of Bytes to be transferred from <b>host to device</b>
 * @param[in] outBytes The number of Bytes to be transferred from <b>device to host</b>
 * @retval #ERR
 * 		E_OK for success,
 * 		E_NOK if the kernel was not found,
 * 		E_NOD if the device was not found
 * */
int elapsdAddKernelDataVolumes(elapsd *e, int KernelID, int DeviceID, int64_t inBytes, int64_t outBytes);
int cpp_callback_elapsdAddKernelDataVolumes(elapsd *e, int KernelID, int DeviceID, int64_t inBytes, int64_t outBytes);

/**
 * Dumps the current content of the framework.
 *
 * @param[in] e Pointer to the instantiated elapsd object
 *
 * */
void elapsdPrintResults(elapsd *e);
void cpp_callback_elapsdPrintResults(elapsd *e);

/**
 * Write the results to a Berkeley DB. If the DB not exists, it is created.
 *
 * */
void elapsdCommitToDB(elapsd *e);
void cpp_callback_elapsdCommitToDB(elapsd *e);

#ifdef __cplusplus
}
#endif

#endif
