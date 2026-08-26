#ifndef __BOOSTER_ROBOTICS_SDK_RESPONSE_HEADER_HPP__
#define __BOOSTER_ROBOTICS_SDK_RESPONSE_HEADER_HPP__

#include <string>
#include <booster/third_party/nlohmann_json/json.hpp>

namespace booster {
namespace robot {

/** @brief Header carrying the integer status of an RPC response. */
class ResponseHeader {
public:
    /** @brief Constructs an invalid response header. */
    ResponseHeader() = default;
    /** @brief Constructs a response header with @p status. */
    ResponseHeader(int64_t status) :
        status_(status) {
    }

    /** @brief Sets the response status code. */
    void SetStatus(int64_t status) {
        status_ = status;
    }

    /** @brief Returns the response status code. */
    int64_t GetStatus() const {
        return status_;
    }

public:
    /** @brief Loads `status` from JSON. */
    void FromJson(nlohmann::json &json) {
        status_ = json["status"];
    }

    /** @brief Serializes this header to JSON. */
    nlohmann::json ToJson() const {
        nlohmann::json json;
        json["status"] = status_;
        return json;
    }

private:
    int64_t status_ = -1;

};

}
} // namespace booster::robot

#endif // __BOOSTER_ROBOTICS_SDK_RESPONSE_HEADER_HPP__
