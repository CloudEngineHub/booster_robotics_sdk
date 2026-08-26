#pragma once

#include <string>

#include <booster/third_party/nlohmann_json/json.hpp>

namespace booster {
namespace robot {
namespace camera {

/**
 * @file camera_api.hpp
 * @brief RPC identifiers for querying the robot camera catalog.
 *
 * This header defines the wire-level operation identifiers accepted by the
 * camera catalog service. Applications normally use CameraClient instead of
 * constructing RPC requests directly.
 *
 * @par Model support
 * @note Supported model: K1 | T1 | T2
 * @note Runtime availability requires the `booster_camera` service to be
 * deployed and running on the target robot. The returned catalog reflects the
 * camera hardware and perception configuration installed on that robot.
 *
 * @note Camera catalog identifiers occupy a separate RPC number range from
 * the vision service identifiers.
 *
 * @see CameraClient
 */

/**
 * @brief Numeric identifiers for camera catalog RPC operations.
 *
 * These values are aligned with the camera service protocol. They are exposed
 * for users of CameraClient::SendApiRequest() and
 * CameraClient::SendApiRequestWithResponse(), and for protocol diagnostics.
 */
enum class CameraApiId : int64_t {
    kGetCameras = 3100, ///< Query the configured camera catalog; supported
                        ///< model: K1 | T1 | T2. Requires the `booster_camera`
                        ///< service on the target robot.
};

} // namespace camera
} // namespace robot
} // namespace booster
