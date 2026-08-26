#ifndef __BOOSTER_ROBOTICS_SDK_REQUEST_HPP__
#define __BOOSTER_ROBOTICS_SDK_REQUEST_HPP__

#include <booster/robot/rpc/request_header.hpp>

namespace booster {
namespace robot {

/** @brief RPC request consisting of a RequestHeader and serialized body. */
class Request {
public:
    /** @brief Constructs an empty request. */
    Request() = default;
    /** @brief Constructs a request from a header and body string. */
    Request(
        const RequestHeader &header,
        const std::string &body) :
        header_(header),
        body_(body) {
    }

    /** @brief Replaces the request header. */
    void SetHeader(const RequestHeader &header) {
        header_ = header;
    }

    /** @brief Returns a copy of the request header. */
    RequestHeader GetHeader() const {
        return header_;
    }

    /** @brief Replaces the serialized request body. */
    void SetBody(const std::string &body) {
        body_ = body;
    }

    /** @brief Returns the serialized request body. */
    std::string GetBody() const {
        return body_;
    }

private:
    RequestHeader header_;
    std::string body_;
};

}
} // namespace booster::robot

#endif // __BOOSTER_ROBOTICS_SDK_REQUEST_HPP__
