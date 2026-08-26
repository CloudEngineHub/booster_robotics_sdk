#ifndef __BOOSTER_ROBOTICS_SDK_RESPONSE_HPP__
#define __BOOSTER_ROBOTICS_SDK_RESPONSE_HPP__

#include <booster/robot/rpc/response_header.hpp>
#include <booster/robot/rpc/error.hpp>

namespace booster {
namespace robot {

/** @brief RPC response consisting of a ResponseHeader and serialized body. */
class Response {
public:
    /** @brief Constructs an empty response. */
    Response() = default;
    /** @brief Constructs a response from a header and body string. */
    Response(
        const ResponseHeader &header,
        const std::string &body) :
        header_(header),
        body_(body) {
    }

    /** @brief Creates a successful response with an empty body. */
    static Response OkResponse() {
        return Response(ResponseHeader(kRpcStatusCodeSuccess), "");
    }

    /** @brief Replaces the response header. */
    void SetHeader(const ResponseHeader &header) {
        header_ = header;
    }

    /** @brief Returns the response header. */
    const ResponseHeader &GetHeader() const {
        return header_;
    }

    /** @brief Replaces the serialized response body. */
    void SetBody(const std::string &body) {
        body_ = body;
    }

    /** @brief Returns the serialized response body. */
    std::string GetBody() const {
        return body_;
    }

private:
    ResponseHeader header_;
    std::string body_;
};
}
}

#endif // __BOOSTER_ROBOTICS_SDK_RESPONSE_HPP__
