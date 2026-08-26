#pragma once

#include <memory>
#include <string>

#include <booster/robot/rpc/rpc_client.hpp>

#include "handeye_calib_api.hpp"

namespace booster {
namespace robot {
namespace vision {

/**
 * @brief Client for the optional auto hand-eye calibration service.
 * @note Supported model: K1 | T1 | T2
 * @note Requires a compatible camera.
 */
class HandEyeCalibClient {
public:
    HandEyeCalibClient() = default;
    ~HandEyeCalibClient() = default;

    /** @brief Connects to the default calibration service. */
    void Init();
    /** @brief Connects to a named robot's calibration service. */
    void Init(const std::string &robot_name);

    /** @brief Sends a calibration request without parsing a response body. */
    int32_t SendApiRequest(HandEyeCalibApiId api_id, const std::string &param);
    /** @brief Sends a calibration request and stores the raw response. */
    int32_t SendApiRequestWithResponse(
        HandEyeCalibApiId api_id,
        const std::string &param,
        Response &resp);

    /** @brief Starts a calibration job. */
    int32_t StartCalibration(const StartHandEyeCalibParameter &param);
    /** @brief Stops the active calibration job. */
    int32_t StopCalibration();
    /** @brief Gets the active job status. */
    int32_t GetStatus(HandEyeCalibStatus &status);
    /** @brief Gets the most recent calibration result. */
    int32_t GetResult(HandEyeCalibResult &result);
    /** @brief Applies the most recent result to camera extrinsics. */
    int32_t ApplyResult(HandEyeCalibApplyResult &result);

private:
    std::shared_ptr<RpcClient> rpc_client_;
};

}
}
} // namespace booster::robot::vision
