#pragma once

/**
 * Parse Camera RPC JSON bodies into booster_interface::msg::Camera / CameraList.
 */

#include <booster/idl/camera/Camera.h>
#include <booster/third_party/nlohmann_json/json.hpp>

namespace booster {
namespace robot {

/**
 * @brief Converts one camera RPC JSON object to a Camera message.
 * @note Supported model: K1 | T1 | T2
 * @note Requires the camera service.
 */
void CameraFromRpcJson(booster_interface::msg::Camera &camera,
                      const nlohmann::json &j);

/** @brief Converts a camera-list RPC body (array or `{ "cameras": [...] }`). */
void CameraListFromRpcJson(booster_interface::msg::CameraList &list,
                           const nlohmann::json &json);

} // namespace robot
} // namespace booster
