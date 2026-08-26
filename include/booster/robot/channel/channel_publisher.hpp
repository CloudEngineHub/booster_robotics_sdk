#ifndef __BOOSTER_ROBOTICS_SDK_CHANNEL_PUBLISHER_HPP__
#define __BOOSTER_ROBOTICS_SDK_CHANNEL_PUBLISHER_HPP__

#include <memory>
#include <string>

#include <booster/robot/channel/channel_factory.hpp>
#include <booster/robot/rpc/request.hpp>

namespace booster {
namespace robot {

/** @brief Typed DDS publisher wrapper. */
template <typename MSG>
class ChannelPublisher {
public:
    /** @brief Creates a publisher for @p channel_name. */
    explicit ChannelPublisher(const std::string &channel_name, bool reliable = false) :
        channel_name_(channel_name),
        reliable_(reliable) {
    }

    /** @brief Creates the underlying writer; ChannelFactory must be initialized first. */
    void InitChannel() {
        channel_ptr_ = ChannelFactory::Instance()->CreateSendChannel<MSG>(channel_name_, reliable_);
    }

    /** @brief Publishes @p msg. @return false when no writer is initialized or write fails. */
    bool Write(MSG *msg) {
        if (channel_ptr_) {
            return channel_ptr_->Write(msg);
        }
        return false;
    }

    /** @brief Closes the underlying writer. */
    void CloseChannel() {
        if (channel_ptr_) {
            ChannelFactory::Instance()->CloseWriter(channel_name_);
            channel_ptr_.reset();
        }
    }

    /** @brief Returns the DDS topic name. */
    const std::string &GetChannelName() const {
        return channel_name_;
    }

    /** @brief Returns the number of currently matched readers. */
    size_t GetMatchedSubscriptionsCount() const {
        if (channel_ptr_ == nullptr) {
            return 0;
        }
        return channel_ptr_->GetMatchedSubscriptionsCount();
    }

private:
    std::string channel_name_;
    bool reliable_{false};
    ChannelPtr<MSG> channel_ptr_;
};

template <typename MSG>
using ChannelPublisherPtr = std::shared_ptr<ChannelPublisher<MSG>>;

}
} // namespace booster::robot

#endif // __BOOSTER_ROBOTICS_SDK_CHANNEL_PUBLISHER_HPP__
