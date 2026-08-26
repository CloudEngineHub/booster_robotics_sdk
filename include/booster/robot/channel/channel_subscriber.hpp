#ifndef __BOOSTER_ROBOTICS_SDK_CHANNEL_SUBSCRIBER_HPP__
#define __BOOSTER_ROBOTICS_SDK_CHANNEL_SUBSCRIBER_HPP__

#include <iostream>

#include <booster/robot/channel/channel_factory.hpp>
#include <booster/robot/rpc/response.hpp>

namespace booster {
namespace robot {

/** @brief Queue overflow policy exposed by the DDS executor. */
using ChannelSubscriberOverflowPolicy = common::DdsExecutorOverflowPolicy;
/** @brief Queue metrics exposed by the DDS executor. */
using ChannelSubscriberMetrics = common::DdsReaderExecutorMetrics;

/** @brief Reliability and callback queue options for a subscriber. */
struct ChannelSubscriberOptions {
    bool reliable{false};
    common::DdsReaderExecutorOptions executor_options{};
};

/** @brief Typed DDS subscriber wrapper. */
template <typename MSG>
class ChannelSubscriber {
public:
    /** @brief Creates a subscriber with optional reliability. */
    explicit ChannelSubscriber(const std::string &channel_name, bool reliable = false) :
        channel_name_(channel_name) {
        options_.reliable = reliable;
    }

    /** @brief Creates a subscriber with explicit executor options. */
    explicit ChannelSubscriber(
        const std::string &channel_name,
        const ChannelSubscriberOptions &options) :
        channel_name_(channel_name),
        options_(options) {
    }

    template <class F,
              std::enable_if_t<
                  std::is_invocable_r_v<void, F, const void *>, int> = 0>
    /** @brief Creates a subscriber and callback with optional reliability. */
    explicit ChannelSubscriber(const std::string &channel_name,
                               F &&handler,
                               bool reliable = false) :
        channel_name_(channel_name),
        handler_(std::forward<F>(handler)) {
        options_.reliable = reliable;
    }

    template <class F,
              std::enable_if_t<
                  std::is_invocable_r_v<void, F, const void *>, int> = 0>
    /** @brief Creates a subscriber, callback and executor options. */
    explicit ChannelSubscriber(
        const std::string &channel_name,
        F &&handler,
        const ChannelSubscriberOptions &options) :
        channel_name_(channel_name),
        handler_(std::forward<F>(handler)),
        options_(options) {
    }

    /** @brief Sets the callback and initializes the reader. */
    void InitChannel(const std::function<void(const void *)> &handler) {
        handler_ = handler;
        InitChannel();
    }

    /** @brief Initializes the reader using the configured callback. */
    void InitChannel() {
        if (handler_) {
            std::cout << "ChannelSubscriber::InitChannel: setting reliability: "
                      << options_.reliable << ", queue_capacity: "
                      << options_.executor_options.queue_capacity << std::endl;
            channel_ptr_ = ChannelFactory::Instance()->CreateRecvChannel<MSG>(
                channel_name_,
                handler_,
                options_.reliable,
                options_.executor_options);
        } else {
            std::cerr << "ChannelSubscriber::InitChannel: handler is not set" << std::endl;
        }
    }

    /** @brief Closes the underlying reader. */
    void CloseChannel() {
        if (channel_ptr_) {
            ChannelFactory::Instance()->CloseReader(channel_name_);
            channel_ptr_.reset();
        }
    }

    /** @brief Returns the DDS topic name. */
    const std::string &GetChannelName() const {
        return channel_name_;
    }

    /** @brief Returns callback queue metrics, or zero metrics when uninitialized. */
    ChannelSubscriberMetrics GetMetrics() const {
        if (channel_ptr_ == nullptr) {
            return ChannelSubscriberMetrics();
        }
        return channel_ptr_->GetReaderExecutorMetrics();
    }

    /** @brief Returns the number of currently matched writers. */
    size_t GetMatchedPublicationsCount() const {
        if (channel_ptr_ == nullptr) {
            return 0;
        }
        return channel_ptr_->GetMatchedPublicationsCount();
    }

    /** @brief Returns the configured subscriber options. */
    const ChannelSubscriberOptions &GetOptions() const {
        return options_;
    }

private:
    std::string channel_name_;
    ChannelPtr<MSG> channel_ptr_;
    std::function<void(const void *)> handler_;
    ChannelSubscriberOptions options_;
};

}
} // namespace booster::robot

#endif
