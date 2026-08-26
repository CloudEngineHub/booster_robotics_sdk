#ifndef __BOOSTER_ROBOTICS_SDK_ERROR_HPP__
#define __BOOSTER_ROBOTICS_SDK_ERROR_HPP__

#include <cstdint>
namespace booster {
namespace robot {

/**
 * @brief RPC status values shared by SDK services.
 * @note Supported model: K1 | T1 | T2
 */
const int64_t kRpcStatusCodeInvalid = -1;                ///< Request has not been sent or has no status.
const int64_t kRpcStatusCodeSuccess = 0;                 ///< Request completed successfully.
const int64_t kRpcStatusCodeTimeout = 100;               ///< Request or endpoint wait timed out.
const int64_t kRpcStatusCodeBadRequest = 400;            ///< Request parameters are invalid.
const int64_t kRpcStatusCodeConflict = 409;              ///< Request conflicts with current service state.
const int64_t kRpcStatusCodeRequestTooFrequent = 429;    ///< Request rate exceeded service limits.
const int64_t kRpcStatusCodeInternalServerError = 500;   ///< Service-side internal failure.
const int64_t kRpcStatusCodeServerRefused = 501;         ///< Service refused the request.
const int64_t kRpcStatusCodeStateTransitionFailed = 502; ///< Robot state-machine transition failed.

}
} // namespace booster::robot

#endif // __BOOSTER_ROBOTICS_SDK_ERROR_HPP__
