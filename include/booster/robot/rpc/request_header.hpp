#ifndef __BOOSTER_ROBOTICS_SDK_REQUEST_HEADER_HPP__
#define __BOOSTER_ROBOTICS_SDK_REQUEST_HEADER_HPP__

#include <string>
#include <booster/third_party/nlohmann_json/json.hpp>

namespace booster {
namespace robot {

/** @brief Header identifying an RPC method and whether a response is required. */
class RequestHeader {
public:
    /** @brief Constructs a header with default API id 0 and response enabled. */
    RequestHeader() = default;
    /** @brief Constructs a header for @p api_id. */
    RequestHeader(int64_t api_id) :
        api_id_(api_id) {
    }

    /** @brief Sets the service API identifier. */
    void SetApiId(int64_t api_id) {
        api_id_ = api_id;
    }

    /** @brief Returns the service API identifier. */
    int64_t GetApiId() const {
        return api_id_;
    }

    /** @brief Sets whether the server should publish a response. */
    void SetExpectResponse(bool expect_response) {
        expect_response_ = expect_response;
    }

    /** @brief Returns the response expectation flag. */
    bool GetExpectResponse() const {
        return expect_response_;
    }

public:
    /** @brief Loads `api_id` and optional `expect_response` from JSON. */
    void FromJson(nlohmann::json &json) {
        api_id_ = json["api_id"];
        if (json.contains("expect_response")) {
            expect_response_ = json["expect_response"];
        } else {
            expect_response_ = true;
        }
    }

    /** @brief Serializes this header to JSON. */
    nlohmann::json ToJson() const {
        nlohmann::json json;
        json["api_id"] = api_id_;
        json["expect_response"] = expect_response_;
        return json;
    }

private:
    int64_t api_id_ = 0;
    bool expect_response_{true};
};

}
} // namespace booster::robot

#endif // __BOOSTER_ROBOTICS_SDK_REQUEST_HEADER_HPP__
