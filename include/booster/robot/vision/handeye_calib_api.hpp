#pragma once

#include <exception>
#include <string>

#include <booster/third_party/nlohmann_json/json.hpp>

namespace booster {
namespace robot {
namespace vision {

/** @brief Hand-eye calibration RPC identifiers. */
enum class HandEyeCalibApiId {
    kStartCalibration = 3100, ///< Start a hand-eye calibration job.
    kStopCalibration = 3101,  ///< Stop the active calibration job.
    kGetStatus = 3102,        ///< Query calibration progress and state.
    kGetResult = 3103,        ///< Retrieve the calibration result.
    kApplyResult = 3104,      ///< Apply a completed calibration result.
};

/**
 * @brief Parameters for a camera/robot hand-eye calibration run.
 * @note Supported model: K1 | T1 | T2
 * @note Requires the auto-hand-eye service and a compatible head camera.
 */
class StartHandEyeCalibParameter {
public:
    StartHandEyeCalibParameter() = default;

    /** @brief Loads optional calibration settings from JSON. */
    void FromJson(const nlohmann::json &json) {
        if (json.contains("publish_feedback") && !json["publish_feedback"].is_null()) publish_feedback_ = json["publish_feedback"];
        if (json.contains("square_size_m") && !json["square_size_m"].is_null()) square_size_m_ = json["square_size_m"];
    }

    /** @brief Serializes calibration settings to JSON. */
    nlohmann::json ToJson() const {
        nlohmann::json json;
        json["publish_feedback"] = publish_feedback_;
        if (square_size_m_ > 0.0) json["square_size_m"] = square_size_m_;
        return json;
    }

public:
    bool publish_feedback_ = true;
    double square_size_m_ = 0.0;
};

/** @brief Progress and state snapshot for a calibration job. */
class HandEyeCalibStatus {
public:
    HandEyeCalibStatus() = default;

    /** @brief Loads status fields from a service response. */
    void FromJson(const nlohmann::json &json) {
        if (json.contains("status")) status_ = json["status"];
        if (json.contains("job_id") && !json["job_id"].is_null()) job_id_ = json["job_id"];
        if (json.contains("stage") && !json["stage"].is_null()) stage_ = json["stage"];
        if (json.contains("started_at") && !json["started_at"].is_null()) started_at_ = json["started_at"];
        if (json.contains("finished_at") && !json["finished_at"].is_null()) finished_at_ = json["finished_at"];
        if (json.contains("progress") && !json["progress"].is_null()) progress_ = json["progress"].get<double>();
        if (json.contains("stage2_done") && !json["stage2_done"].is_null()) stage2_done_ = json["stage2_done"].get<int>();
        if (json.contains("stage2_total") && !json["stage2_total"].is_null()) stage2_total_ = json["stage2_total"].get<int>();
        if (json.contains("stage3_done") && !json["stage3_done"].is_null()) stage3_done_ = json["stage3_done"].get<int>();
        if (json.contains("stage3_total") && !json["stage3_total"].is_null()) stage3_total_ = json["stage3_total"].get<int>();
        if (json.contains("error") && !json["error"].is_null()) error_json_ = json["error"].dump();
    }

    /** @brief Serializes status fields to JSON. */
    nlohmann::json ToJson() const {
        nlohmann::json json;
        json["status"] = status_;
        json["job_id"] = job_id_;
        json["stage"] = stage_;
        json["started_at"] = started_at_;
        json["finished_at"] = finished_at_;
        json["progress"] = progress_;
        json["stage2_done"] = stage2_done_;
        json["stage2_total"] = stage2_total_;
        json["stage3_done"] = stage3_done_;
        json["stage3_total"] = stage3_total_;
        if (!error_json_.empty()) json["error"] = nlohmann::json::parse(error_json_);
        return json;
    }

public:
    std::string status_;
    std::string job_id_;
    std::string stage_;
    std::string started_at_;
    std::string finished_at_;
    double progress_ = 0.0;
    int stage2_done_ = 0;
    int stage2_total_ = 0;
    int stage3_done_ = 0;
    int stage3_total_ = 0;
    std::string error_json_;
};

/** @brief Calibration result, quality metrics and optional error details. */
class HandEyeCalibResult {
public:
    HandEyeCalibResult() = default;

    /** @brief Loads a result response from JSON. */
    void FromJson(const nlohmann::json &json) {
        if (json.contains("status")) status_ = json["status"];
        if (json.contains("job_id") && !json["job_id"].is_null()) job_id_ = json["job_id"];
        if (json.contains("summary") && !json["summary"].is_null()) summary_ = json["summary"];
        if (json.contains("result") && !json["result"].is_null()) result_json_ = json["result"].dump();
        if (json.contains("result") && json["result"].is_object()
            && json["result"].contains("rms_px") && !json["result"]["rms_px"].is_null()) {
            reprojection_error_px_ = json["result"]["rms_px"].get<double>();
        }
        if (json.contains("reprojection_error_px") && !json["reprojection_error_px"].is_null()) {
            reprojection_error_px_ = json["reprojection_error_px"].get<double>();
        }
        if (json.contains("progress") && !json["progress"].is_null()) progress_ = json["progress"].get<double>();
        if (json.contains("stage2_done") && !json["stage2_done"].is_null()) stage2_done_ = json["stage2_done"].get<int>();
        if (json.contains("stage2_total") && !json["stage2_total"].is_null()) stage2_total_ = json["stage2_total"].get<int>();
        if (json.contains("stage3_done") && !json["stage3_done"].is_null()) stage3_done_ = json["stage3_done"].get<int>();
        if (json.contains("stage3_total") && !json["stage3_total"].is_null()) stage3_total_ = json["stage3_total"].get<int>();
        if (json.contains("error") && !json["error"].is_null()) error_json_ = json["error"].dump();
    }

    /** @brief Serializes the result to JSON. */
    nlohmann::json ToJson() const {
        nlohmann::json json;
        json["status"] = status_;
        json["job_id"] = job_id_;
        json["summary"] = summary_;
        json["reprojection_error_px"] = reprojection_error_px_;
        json["progress"] = progress_;
        json["stage2_done"] = stage2_done_;
        json["stage2_total"] = stage2_total_;
        json["stage3_done"] = stage3_done_;
        json["stage3_total"] = stage3_total_;
        if (!result_json_.empty()) json["result"] = nlohmann::json::parse(result_json_);
        if (!error_json_.empty()) json["error"] = nlohmann::json::parse(error_json_);
        return json;
    }

public:
    std::string status_;
    std::string job_id_;
    std::string summary_;
    std::string result_json_;
    double reprojection_error_px_ = 0.0;
    double progress_ = 0.0;
    int stage2_done_ = 0;
    int stage2_total_ = 0;
    int stage3_done_ = 0;
    int stage3_total_ = 0;
    std::string error_json_;
};

/** @brief Result of applying calibrated camera extrinsics. */
class HandEyeCalibApplyResult {
public:
    HandEyeCalibApplyResult() = default;

    /** @brief Loads apply status from JSON. */
    void FromJson(const nlohmann::json &json) {
        if (json.contains("status")) status_ = json["status"];
        if (json.contains("applied_path") && !json["applied_path"].is_null()) applied_path_ = json["applied_path"];
        if (json.contains("error") && !json["error"].is_null()) error_json_ = json["error"].dump();
    }

    /** @brief Serializes apply status to JSON. */
    nlohmann::json ToJson() const {
        nlohmann::json json;
        json["status"] = status_;
        json["applied_path"] = applied_path_;
        if (!error_json_.empty()) json["error"] = nlohmann::json::parse(error_json_);
        return json;
    }

public:
    std::string status_;
    std::string applied_path_;
    std::string error_json_;
};

}
}
} // namespace booster::robot::vision
