#pragma once

#include <memory>
#include <vector>
#include <string>

#include <booster/robot/rpc/rpc_client.hpp>
#include "vision_api.hpp"

namespace booster {
namespace robot {
namespace vision {

/**
 * @brief Client for the optional object/face detection service.
 * @note Supported model: K1 | T1
 * @note The detection service is limited to non-Geek K1 deployments and is
 * disabled on K1 Geek in the current platform configuration.
 */
class VisionClient {
public:
    VisionClient() = default;
    ~VisionClient() = default;

    /** @brief Connects to the default vision service. */
    void Init();
    /** @brief Connects to a named robot's vision service. */
    void Init(const std::string &robot_name);

    /** @brief Sends a vision request without returning a body. */
    int32_t SendApiRequest(VisionApiId api_id, const std::string &param);
    /** @brief Sends a vision request and stores the raw response. */
    int32_t SendApiRequestWithResponse(VisionApiId api_id, const std::string &param, Response &resp);

    /**
     * @brief Starts vision detection with the requested output features.
     */
    int32_t StartVisionService(bool enable_position, bool enable_color, bool enable_face_detection) {
        StartVisionServiceParameter param_obj(enable_position, enable_color, enable_face_detection);
        std::string param = param_obj.ToJson().dump();
        return SendApiRequest(VisionApiId::kStartVisionService, param);
    }

    /**
     * @brief Stops vision detection and releases its inference resources.
     */
    int32_t StopVisionService() {
        return SendApiRequest(VisionApiId::kStopVisionService, "{}");
    }

    /**
     * @brief Gets the latest detected objects.
     * @param objects Receives the parsed detection list.
     * @param ratio Focus-region ratio; defaults to 0.33.
     * @return 0 on success, a service status on RPC failure, or -1 on malformed JSON.
     */
    int32_t GetDetectionObject(std::vector<DetectResults> &objects, float ratio = 0.33f) {
        GetDetectionObjectParameter detect_param(ratio);
        std::string param = detect_param.ToJson().dump();

        Response resp;
        int32_t ret = SendApiRequestWithResponse(VisionApiId::kGetDetectionObject, param, resp);

        if (ret != 0) {
            return ret;
        }

        try {
            nlohmann::json body_json = nlohmann::json::parse(resp.GetBody());
            objects.clear();

            if (body_json.is_array()) {
                for (const auto &item_json : body_json) {
                    DetectResults item;
                    item.FromJson(item_json);
                    objects.push_back(item);
                }
            } else if (body_json.contains("objects") && body_json["objects"].is_array()) {
                for (const auto &item_json : body_json["objects"]) {
                    DetectResults item;
                    item.FromJson(item_json);
                    objects.push_back(item);
                }
            }
        } catch (const std::exception &e) {
            return -1;
        }

        return 0;
    }

private:
    std::shared_ptr<RpcClient> rpc_client_;
};

}
}
} // namespace booster::robot::vision
