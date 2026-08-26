#pragma once

#include <memory>
#include <string>

#include <booster/robot/camera/camera_api.hpp>
#include <booster/robot/common/device_info.hpp>
#include <booster/robot/rpc/rpc_client.hpp>
#include <booster/robot/rpc/response.hpp>

namespace booster {
namespace robot {
namespace camera {

/**
 * @file camera_client.hpp
 * @brief Client interface for querying the robot camera catalog.
 *
 * CameraClient communicates with the camera catalog RPC service over DDS. Call
 * Init() before sending a request. GetCameras() returns the service JSON in a
 * b1::DeviceInfo object; CameraListFromDeviceInfo() can then convert it to the
 * strongly typed booster_interface::msg::CameraList representation.
 *
 * @par Model support
 * @note Supported model: K1 | T1 | T2
 * @note Runtime availability depends on the `booster_camera` service being
 * deployed and running. Catalog entries and camera capabilities vary with the
 * hardware and perception configuration installed on the target robot.
 *
 * @par Runtime data
 * The catalog combines static camera configuration with runtime camera status.
 * Fields such as connection state, frame ID, and stream dimensions may be
 * unknown or zero until the camera service receives current camera information.
 *
 * @see CameraApiId
 * @see booster::robot::CameraListFromDeviceInfo
 */

/**
 * @brief Synchronous DDS RPC client for the robot camera catalog service.
 *
 * A client instance must be initialized for either the default robot or a
 * named robot before use. The convenience method GetCameras() is preferred for
 * normal catalog queries; the generic request methods are provided for direct
 * protocol access and diagnostics.
 *
 * @note Supported model: K1 | T1 | T2
 * @note Requires the `booster_camera` service to be deployed on the target
 * robot.
 * @note This class does not start the camera driver or the camera catalog
 * service. Service and camera availability must be managed separately.
 */
class CameraClient {
public:
    /**
     * @brief Constructs an uninitialized camera client.
     *
     * Call Init() or Init(const std::string &) before sending requests.
     */
    CameraClient() = default;

    /** @brief Destroys the camera client and releases its RPC client. */
    ~CameraClient() = default;

    /**
     * @brief Initializes communication with the default robot.
     *
     * Requests are sent on the default `rt/CameraApiTopic` DDS RPC channel.
     * Calling this function again replaces the previously initialized RPC
     * client.
     *
     * @note Supported model: K1 | T1 | T2
     */
    void Init();

    /**
     * @brief Initializes communication with a named robot.
     * @note Supported model: K1 | T1 | T2
     *
     * Requests are sent on `rt/CameraApiTopic/<robot_name>`. Use this overload
     * when multiple robots share the same DDS domain and the service topics are
     * namespaced by robot name.
     *
     * @param robot_name Robot-name suffix used by the target camera service. An
     * empty string selects the default `rt/CameraApiTopic` channel.
     *
     */
    void Init(const std::string &robot_name);

    /**
     * @brief Sends a camera RPC request and returns only its status code.
     * @note Supported model: K1 | T1 | T2
     * @note This applies to the currently defined operation.
     *
     * This method waits synchronously for a service response and discards the
     * response body. Use SendApiRequestWithResponse() when the response payload
     * is required.
     *
     * @param api_id Camera operation identifier.
     * @param param Serialized request body expected by the selected operation.
     * CameraApiId::kGetCameras currently expects an empty body.
     * @return `booster::robot::kRpcStatusCodeSuccess` (0) on success; otherwise
     * an RPC transport or service status code.
     *
     * @pre Init() or Init(const std::string &) has been called successfully.
     * @see booster/robot/rpc/error.hpp
     */
    int32_t SendApiRequest(CameraApiId api_id, const std::string &param);

    /**
     * @brief Sends a camera RPC request and returns the complete response.
     * @note Supported model: K1 | T1 | T2
     * @note This applies to the currently defined operation.
     *
     * The response object is populated for both successful and failed RPC
     * responses. Inspect Response::GetHeader() and Response::GetBody() for
     * protocol diagnostics or custom parsing.
     *
     * @param api_id Camera operation identifier.
     * @param param Serialized request body expected by the selected operation.
     * CameraApiId::kGetCameras currently expects an empty body.
     * @param[out] resp Complete RPC response, including status header and body.
     * @return The status code stored in `resp`. Zero indicates success;
     * nonzero values report a transport or service error.
     *
     * @pre Init() or Init(const std::string &) has been called successfully.
     * @see booster::robot::Response
     * @see booster/robot/rpc/error.hpp
     */
    int32_t SendApiRequestWithResponse(CameraApiId api_id, const std::string &param,
                                       booster::robot::Response &resp);

    /**
     * @brief Queries the cameras configured on the target robot.
     * @note Supported model: K1 | T1 | T2
     * @note The `booster_camera` service must be deployed and running. Returned
     * entries depend on the installed camera hardware and robot perception
     * configuration.
     * @note Runtime-derived fields such as connection state, frame ID, and
     * stream dimensions may be unknown or zero until camera information is
     * available to the service.
     *
     * On success, `info.kind_` is set to b1::DeviceInfoKind::kCamera and
     * `info.json_` contains the RPC response body. The body has a top-level
     * `cameras` array whose entries describe camera identity, mounting
     * position, connection state, published topics, extrinsics, and stream
     * encodings. Convert the result to booster_interface::msg::CameraList with
     * booster::robot::CameraListFromDeviceInfo().
     *
     * When the RPC status is nonzero, this method returns immediately and does
     * not modify `info`.
     *
     * @param[out] info Camera catalog container populated on success.
     * @return `booster::robot::kRpcStatusCodeSuccess` (0) on success; otherwise
     * an RPC transport or camera-service status code.
     *
     * @pre Init() or Init(const std::string &) has been called successfully.
     * @throws nlohmann::json::exception If a successful service response does
     * not contain valid JSON in the expected representation.
     * @see booster::robot::CameraListFromDeviceInfo
     * @see booster::robot::b1::DeviceInfo
     */
    int32_t GetCameras(b1::DeviceInfo &info);

private:
    /** @brief Underlying synchronous DDS RPC transport, created by Init(). */
    std::shared_ptr<booster::robot::RpcClient> rpc_client_;
};

} // namespace camera
} // namespace robot
} // namespace booster
