#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <booster/robot/audio/audio_types.h>

namespace booster {
namespace robot {
namespace audio {

class AudioManager;

/**
 * @brief Streams microphone PCM frames through callbacks.
 *
 * @note Supported model: K1 | T1 | T2
 * @note Requires the capture backend. NAEC output is hardware/backend
 * dependent and may return kAudioErrNotSupported.
 */
class AudioCaptureStream {
public:
    /** @brief Creates a capture stream associated with @p audio_manager. */
    explicit AudioCaptureStream(AudioManager &audio_manager);
    ~AudioCaptureStream();

    AudioCaptureStream(const AudioCaptureStream &) = delete;
    AudioCaptureStream &operator=(const AudioCaptureStream &) = delete;

    /** @brief Negotiates requested raw/NAEC streams and formats. */
    int32_t Init(const AudioCaptureStreamOptions &options);
    /** @brief Starts frame delivery. */
    int32_t Start();
    /** @brief Pauses frame delivery. */
    int32_t Pause();
    /** @brief Stops frame delivery. */
    int32_t Stop();
    /** @brief Destroys the service-side stream. */
    int32_t Destroy();

    /** @brief Reads negotiated formats and counters. */
    int32_t GetInfo(AudioCaptureStreamInfo *out) const;
    /** @brief Selects an input endpoint by device identifier. */
    int32_t SetPreferredDevice(const std::string &device_id);

    /** @brief Returns the service session identifier. */
    int64_t GetSessionId() const;
    /** @brief Returns the most recently observed stream state. */
    AudioCaptureStreamState GetCachedState() const;

    /** @brief Registers a callback invoked for each captured frame. */
    void SetFrameCallback(AudioCaptureFrameCallback callback);
    /** @brief Registers a state-change callback. */
    void SetStateCallback(AudioCaptureStreamStateCallback callback);
    /** @brief Registers an error callback. */
    void SetErrorCallback(AudioCaptureStreamErrorCallback callback);

private:
    struct StateData;

    void EnsureErrorListenerRegistered();

    static AudioCaptureStreamState ParseCaptureStreamState(int32_t raw_state);

    AudioManager *audio_manager_{nullptr};
    int64_t error_listener_id_{0};
    std::shared_ptr<StateData> state_;
};

} // namespace audio
} // namespace robot
} // namespace booster
