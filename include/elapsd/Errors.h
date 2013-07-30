#ifndef E_ELAPSD_H
#define E_ELAPSD_H

/**
 *
 * This header defines the possible errors.
 *
 * @author	Sebastian Dressler
 * @date	2012-05-25
 *
 * \addtogroup Celapsd
 * @{
 *
 * */

#ifdef __cplusplus
extern "C" {
#endif

enum ERR {
	E_OK,	///< No error, success
	E_DUPK,	///< Kernel already exists
	E_DUPD,	///< Device already exists
	E_NOK,	///< Kernel not found
	E_NOD,	///< Device not found
	E_TMR,	///< Timer error
	E_RES,	///< Error in results
	E_EQU,	///< Two given ID's are equal
    E_PMAP  ///< Error with params map 
};

#ifdef __cplusplus
}
#endif

#endif /* E_ELAPSD_H */

/** @} */
