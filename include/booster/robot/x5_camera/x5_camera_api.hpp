#ifndef BOOSTER_ROBOTICS_SDK_X5_CAMERA_API_HPP
#define BOOSTER_ROBOTICS_SDK_X5_CAMERA_API_HPP

#include <string>
#include <booster/third_party/nlohmann_json/json.hpp>
#include <booster/robot/x5_camera/x5_camera_api_const.hpp>

namespace booster {
namespace robot {
namespace x5_camera {

/** @brief X5 camera control RPC identifiers. */
enum class X5CameraApiId{
    kChangeMode = 5001, ///< Change the X5 camera operating mode.
    kGetStatus = 5002,  ///< Query the X5 camera controller status.
};

/** @brief JSON parameter selecting an X5 camera mode. */
class ChangeModeParameter {
public:
    ChangeModeParameter() = default;
    ChangeModeParameter(CameraSetMode mode) :
        mode_(mode) {
    }

public:
    /** @brief Loads the mode field from JSON. */
    void FromJson(nlohmann::json &json) {
        mode_ = static_cast<CameraSetMode>(json["mode"]);
    }

    /** @brief Serializes the selected mode to JSON. */
    nlohmann::json ToJson() const {
        nlohmann::json json;
        json["mode"] = static_cast<int>(mode_);
        return json;
    }

public:
    CameraSetMode mode_;
};

/** @brief JSON response containing the current X5 camera status. */
class GetStatusResponse {
public:
    GetStatusResponse() = default;
    GetStatusResponse(CameraControlStatus status) :
        status_(status) {
    }

public:
    /** @brief Loads the status field from JSON. */
    void FromJson(nlohmann::json &json) {
        status_ = static_cast<CameraControlStatus>(json["status"]);
    }

    /** @brief Serializes the status to JSON. */
    nlohmann::json ToJson() const {
        nlohmann::json json;
        json["status"] = static_cast<int>(status_);
        return json;
    }

public:
    CameraControlStatus status_;
};

}
}
} // namespace booster::robot

#endif
