#ifndef BOOSTER_ROBOTICS_SDK_X5_CAMERA_API_CONST_HPP
#define BOOSTER_ROBOTICS_SDK_X5_CAMERA_API_CONST_HPP

#include <string>

namespace booster {
namespace robot {
namespace x5_camera {

/** @brief DDS topic used by the X5 camera control RPC. */
static const std::string kTopicX5CameraControlMode = "rt/X5CameraControl";

/**
 * @brief Requested X5 camera operating mode.
 * @note Supported model: K1 | T2
 * @note K1 support is limited to compatible camera editions. Current T1
 * platform profiles use the standard camera service instead.
 */
enum class CameraSetMode {
    kCameraModeNormal = 0,               ///< Select normal-resolution mode.
    kCameraModeHighResolution = 1,       ///< Select high-resolution mode.
    kCameraModeNormalEnable = 2,         ///< Enable normal-resolution mode.
    kCameraModeHighResolutionEnable = 3, ///< Enable high-resolution mode.
};

/** @brief Status reported by the X5 camera controller. */
enum class CameraControlStatus {
    kCameraStatusNormal = 0,         ///< Camera is operating at normal resolution.
    kCameraStatusHighResolution = 1, ///< Camera is operating at high resolution.
    kCameraStatusError = 2,          ///< Camera control reported an error.
    kCameraStatusNull = 3,           ///< Camera status is unavailable.
};

}
}
} // namespace booster::robot

#endif
