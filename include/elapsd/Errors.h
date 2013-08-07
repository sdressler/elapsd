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

#include <stdexcept>
#include <string>

struct param_mismatch_error : public std::runtime_error {
    param_mismatch_error(const std::string &m) : std::runtime_error(m) { }
};

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
    E_PMAP, ///< Error with params map
    E_PNTS  ///< Too few points for prediction
};

#ifdef __cplusplus
}
#endif

#endif /* E_ELAPSD_H */

/** @} */
