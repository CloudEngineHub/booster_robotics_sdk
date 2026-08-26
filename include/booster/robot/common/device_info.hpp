#pragma once

#include <booster/third_party/nlohmann_json/json.hpp>

namespace booster {
namespace robot {
namespace b1 {

/**
 * @brief Unified JSON-backed device/catalog info.
 * @note Supported model: K1 | T1 | T2
 *
 * The concrete device set depends on the services and hardware deployed on the
 * robot. ToJson() wraps the body as `{ "kind": int, "body": json }`.
 */
// Unified JSON-backed device / catalog info (IMU sensors, hands, robot model, and
// future kinds e.g. cameras). kind_ is set by B1LocoClient; wire RPC body lives in json_ only.
// ToJson() wraps as { "kind": int, "body": json_ }; FromJson accepts that or a bare RPC body.

/** @brief Identifies the concrete catalog represented by DeviceInfo::json_. */
enum class DeviceInfoKind {
    kUnknown = -1,  ///< Unclassified data, including a bare RPC body without a kind field.
    kSensors = 0,   ///< Sensor catalog information.
    kHands = 1,     ///< Hand-device catalog information.
    kRobotModel = 2, ///< Robot-model information.
    kCamera = 3,    ///< Reserved for future camera device-info RPCs.
};

/** @brief Stores a device-info RPC body together with its optional kind. */
class DeviceInfo {
public:
    /** @brief Constructs an unknown, empty value. */
    DeviceInfo() = default;

    /** @brief Loads a wrapped or bare RPC JSON body. */
    void FromJson(nlohmann::json &json) {
        if (json.contains("kind") && json.contains("body")) {
            kind_ = static_cast<DeviceInfoKind>(json.at("kind").get<int>());
            json_ = json.at("body");
        } else {
            kind_ = DeviceInfoKind::kUnknown;
            json_ = json;
        }
    }

    /** @brief Serializes this value as a kind/body wrapper. */
    nlohmann::json ToJson() const {
        nlohmann::json out;
        out["kind"] = static_cast<int>(kind_);
        out["body"] = json_;
        return out;
    }

public:
    DeviceInfoKind kind_ = DeviceInfoKind::kUnknown;
    nlohmann::json json_;
};

} // namespace b1
} // namespace robot
} // namespace booster
