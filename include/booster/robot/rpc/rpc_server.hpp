#ifndef __BOOSTER_ROBOTICS_SDK_B1_SERVER_HPP__
#define __BOOSTER_ROBOTICS_SDK_B1_SERVER_HPP__

#include <memory>

#include <booster/robot/channel/channel_publisher.hpp>
#include <booster/robot/channel/channel_subscriber.hpp>
#include <booster/robot/rpc/request.hpp>
#include <booster/robot/rpc/response.hpp>
#include <booster/idl/rpc/RpcReqMsg.h>
#include <booster/idl/rpc/RpcRespMsg.h>
namespace booster {
namespace robot {

/**
 * @brief Base class for DDS-backed SDK RPC services.
 * @note Supported model: K1 | T1 | T2
 * @note Derived services define their own robot and hardware support.
 */
class RpcServer {
public:
    RpcServer() = default;
    virtual ~RpcServer() = default;

    /** @brief Starts request/response channels for @p channel_name. */
    void Init(const std::string &channel_name);
    /** @brief Compatibility overload; reliability is selected by the implementation. */
    void Init(const std::string &channel_name, bool /*reliable*/) {
        Init(channel_name);
    }
    /** @brief Stops serving requests and closes DDS channels. */
    void Stop();

protected:
    /** @brief Handles one decoded request and returns the response to publish. */
    virtual Response HandleRequest(const Request &req) = 0;

private:
    void DdsReqMsgHandler(const void *msg);
    int32_t SendResponse(const std::string &uuid, const Response &resp);

    std::shared_ptr<ChannelPublisher<booster_msgs::msg::RpcRespMsg>> channel_publisher_;
    std::shared_ptr<ChannelSubscriber<booster_msgs::msg::RpcReqMsg>> channel_subscriber_;
};

}
} // namespace booster::robot

#endif // __BOOSTER_ROBOTICS_SDK_B1_SERVER_HPP__
