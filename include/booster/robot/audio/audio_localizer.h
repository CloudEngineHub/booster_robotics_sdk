#pragma once

#include <cstdint>

namespace booster {
namespace robot {
namespace audio {

class AudioManager;

/**
 * @brief Provides microphone direction-of-arrival (DOA) estimates.
 * @note Supported only when the robot has a compatible microphone array and
 * localizer backend; otherwise the service returns kAudioErrNotSupported.
 */
class AudioLocalizer {
public:
    /** @brief Creates a localizer associated with @p audio_manager. */
    explicit AudioLocalizer(AudioManager &audio_manager);
    ~AudioLocalizer();

    /** @brief Gets the current DOA angle in degrees. */
    int32_t GetDoaAngle(int *angle_deg) const;

private:
    AudioManager *audio_manager_{nullptr};
};

} // namespace audio
} // namespace robot
} // namespace booster
